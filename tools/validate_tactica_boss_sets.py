#!/usr/bin/env python3
"""Audit every protected HnS boss roster against active species/form data."""
from __future__ import annotations

import argparse
import json
import re
import subprocess
from functools import lru_cache
from pathlib import Path

from generate_tactica_rival import constants, normalize
from sync_tactica_bosses import MEGA_BASE_ABILITIES, TRAINERS, ROCKET_TRAINERS
from sync_tactica_species_evolutions import species_blocks, runtime_evolutions

ROOT = Path(__file__).resolve().parents[1]


def species_aliases() -> dict[str, str]:
    return dict(re.findall(r'^#define\s+(SPECIES_\w+)\s+(SPECIES_\w+)\b',
                           (ROOT / 'include/constants/species.h').read_text(), re.M))


def resolve(name: str, aliases: dict[str, str]) -> str:
    seen = set()
    while name in aliases and name not in seen:
        seen.add(name)
        name = aliases[name]
    return name


def abilities(block: str) -> list[str]:
    match = re.search(r'\.abilities\s*=\s*\{([^}]+)\}', block)
    return re.findall(r'ABILITY_\w+', match[1]) if match else []


def boss_parties(text: str, protected: set[str]):
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    text = '\n'.join(line.rstrip() for line in text.splitlines())
    for trainer, body in re.findall(r'^=== (TRAINER_\w+) ===\n(.*?)(?=^=== |\Z)', text, re.M | re.S):
        match = re.search(r'^Class: (.*)$', body, re.M)
        if not match or match[1].strip() not in protected:
            continue
        header, party = body.strip().split('\n\n', 1)
        difficulty = re.search(r'^Difficulty: (.*)$', header, re.M)
        mons = []
        for slot, block in enumerate(re.split(r'\n\s*\n', party.strip()), 1):
            lines = block.splitlines()
            species, _, item = lines[0].partition(' @ ')
            species = re.sub(r'\s+\([MF]\)$', '', species)
            fields = dict(re.findall(r'^([A-Za-z ]+): (.*)$', block, re.M))
            mons.append(dict(fields, species=species, item=item,
                             moves=re.findall(r'^- (.+)$', block, re.M), slot=slot))
        yield trainer, difficulty[1] if difficulty else 'Normal', mons


def species_types(block: str) -> list[str]:
    match = re.search(r'\.types\s*=\s*MON_TYPES\(([^)]+)', block)
    result = re.findall(r'TYPE_\w+', match[1])
    return result if len(result) == 2 else result * 2


def runtime_move_tables() -> dict[str, set[str]]:
    config = (ROOT / 'include/config/pokemon.h').read_text()
    generation = re.search(r'^#define P_LVL_UP_LEARNSETS\s+GEN_(\d+)', config, re.M)[1]
    preamble = ('#include "config/general.h"\n#include "gba/defines.h"\n'
                '#include "constants/global.h"\n#include "constants/pokemon.h"\n')
    source = preamble + f'#include "src/data/pokemon/level_up_learnsets/gen_{generation}.h"\n'
    # Teachable tables are generated from all_learnables.json during the ROM
    # build. Read that authored JSON directly; the audit must work pre-build.
    source += '#include "src/data/pokemon/egg_moves.h"\n'
    result = subprocess.run(['cpp', '-Iinclude', '-I.', '-DPOKEMON_HNS', '-DMODERN=1', '-DTESTING=0', '-P', '-'],
                            input=source, cwd=ROOT, capture_output=True, text=True, check=True).stdout
    return {name: set(re.findall(r'MOVE_\w+', body)) - {'MOVE_UNAVAILABLE'}
            for name, body in re.findall(r'static const (?:struct LevelUpMove|u16) (\w+)\[\]\s*=\s*\{(.*?)\n\};', result, re.S)}


class Catalog:
    """Enabled HnS species and historical/native move pools (not a PvP ruleset)."""

    def __init__(self):
        self.blocks = species_blocks()
        self.aliases = species_aliases()
        for name in ('moves', 'abilities', 'items'):
            self.aliases.update(dict(re.findall(r'(?:^#define\s+|^\s*)((?:MOVE|ABILITY|ITEM)_\w+)\s*(?:=\s*|\s+)((?:MOVE|ABILITY|ITEM)_\w+)\b', (ROOT / f'include/constants/{name}.h').read_text(), re.M)))
        self.maps = {prefix: constants(ROOT / path, prefix) for prefix, path in (
            ('SPECIES_', 'include/constants/species.h'), ('ABILITY_', 'include/constants/abilities.h'),
            ('ITEM_', 'include/constants/items.h'), ('MOVE_', 'include/constants/moves.h'))}
        self.learnables = {s: {resolve(m, self.aliases) for m in moves} for s, moves in
                      json.loads((ROOT / 'src/data/pokemon/all_learnables.json').read_text()).items()}
        self.learnable_names = {normalize(s): moves for s, moves in self.learnables.items()}
        self.native = runtime_move_tables()
        self.parents = {}
        for parent, routes in runtime_evolutions().items():
            for route in routes:
                if route['method'] not in {'EVO_NONE', 'EVO_SPLIT_FROM_EVO'}:
                    self.parents.setdefault(resolve(route['target'], self.aliases), set()).add(resolve(parent, self.aliases))
        policy = json.loads((ROOT / "data/spec/boss_set_legality.json").read_text())
        self.historical = {r["species"]: r["moves"] for r in policy["historical_moves"]}

    @lru_cache(maxsize=None)
    def move_pool(self, species):
        result = set()
        pending = [species]
        seen = set()
        while pending:
            current = pending.pop()
            if current in seen:
                continue
            seen.add(current)
            result.update(self.learnables.get(current.removeprefix('SPECIES_'), []))
            for pointer in re.findall(r'\.(?:levelUpLearnset|teachableLearnset|eggMoveLearnset)\s*=\s*(\w+)', self.blocks.get(current, '')):
                result.update(resolve(m, self.aliases) for m in self.native.get(pointer, []))
                shared = re.fullmatch(r's(.+)TeachableLearnset', pointer)
                if shared:
                    # Some forms share a table keyed by the family (Mimikyu,
                    # Galar Darmanitan, partner Pikachu), not their species ID.
                    result.update(self.learnable_names.get(normalize(shared[1]), []))
            result.update(self.historical.get(current, []))
            pending.extend(self.parents.get(current, []))
        return result


def audit(catalog: Catalog | None = None, parties=None) -> dict:
    catalog = catalog or Catalog()
    blocks, aliases, maps = catalog.blocks, catalog.aliases, catalog.maps
    protected = set(json.loads((ROOT / 'data/spec/trainer_levels.json').read_text())['protected_classes'])
    form_text = (ROOT / 'src/data/pokemon/form_change_tables.h').read_text()
    tables = dict(re.findall(r'static const struct FormChange (\w+)\[\]\s*=\s*\{(.*?)\n\};', form_text, re.S))
    stones = set(re.findall(r'FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,\s*SPECIES_\w+,\s*(ITEM_\w+)', form_text))
    errors = []
    teams = []
    mega_sets = []
    include_dynamic = parties is None
    if parties is None:
        parties = boss_parties((ROOT / 'src/data/trainers_hns.party').read_text(), protected)
    for trainer, difficulty, mons in parties:
        team = {'trainer': trainer, 'difficulty': difficulty, 'slots': len(mons)}
        teams.append(team)
        mega_count = 0
        for mon in mons:
            label = f"{trainer}/{difficulty}/slot {mon['slot']} ({mon['species']})"
            species = resolve(maps['SPECIES_'].get(normalize(mon['species']), ''), aliases)
            block = blocks.get(species)
            if block is None:
                errors.append(f'{label}: unknown or disabled species')
                continue
            legal = abilities(block)
            ability = resolve(maps['ABILITY_'].get(normalize(mon.get('Ability', 'None')), ''), aliases)
            if not ability:
                errors.append(f'{label}: unknown ability {mon.get("Ability")}')
            if ability != 'ABILITY_NONE' and ability not in legal:
                errors.append(f'{label}: illegal base ability {ability}; legal: {legal}')
            if re.search(r'\.isMegaEvolution\s*=\s*(?:1|TRUE)', block):
                errors.append(f'{label}: enters already Mega-evolved')
            key = species.removeprefix('SPECIES_')
            available = catalog.move_pool(species)
            form_moves = {'ROTOM_HEAT': 'MOVE_OVERHEAT', 'ROTOM_WASH': 'MOVE_HYDRO_PUMP',
                          'ROTOM_FROST': 'MOVE_BLIZZARD', 'ROTOM_FAN': 'MOVE_AIR_SLASH', 'ROTOM_MOW': 'MOVE_LEAF_STORM'}
            for move in mon['moves']:
                token = resolve(maps['MOVE_'].get(normalize(move), ''), aliases)
                if token not in available and token != form_moves.get(key):
                    errors.append(f'{label}: incompatible move {move}')
            if len(mon['moves']) > 4 or len(set(mon['moves'])) != len(mon['moves']):
                errors.append(f'{label}: too many or duplicate moves')
            evs = [int(n) for n in re.findall(r'(\d+) (?:HP|Atk|Def|SpA|SpD|Spe)', mon.get('EVs', ''))]
            if any(e > 252 for e in evs) or sum(evs) > 510:
                errors.append(f'{label}: illegal EV spread {evs}')
            ivs = [int(n) for n in re.findall(r'(\d+) (?:HP|Atk|Def|SpA|SpD|Spe)', mon.get('IVs', ''))]
            if any(i > 31 for i in ivs):
                errors.append(f'{label}: illegal IV spread {ivs}')
            item = resolve(maps['ITEM_'].get(normalize(mon['item'] or 'None'), ''), aliases)
            if not item:
                errors.append(f'{label}: unknown item {mon["item"]}')
            if item not in stones:
                continue
            mega_count += 1
            table = re.search(r'\.formChangeTable\s*=\s*(\w+)', block)
            routes = re.findall(r'FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,\s*(SPECIES_\w+),\s*'+re.escape(item)+r'\b', tables.get(table[1], '') if table else '')
            routes = [resolve(s, aliases) for s in routes if resolve(s, aliases) in blocks]
            if len(routes) != 1:
                errors.append(f'{label}: stone {item} has no unique enabled Mega target')
                continue
            target = routes[0]
            ability_slot = legal.index(ability) if ability in legal else 0
            target_abilities = abilities(blocks[target])
            target_ability = target_abilities[ability_slot]
            if target_ability == 'ABILITY_NONE':
                target_ability = next(a for a in target_abilities if a != 'ABILITY_NONE')
            mega_sets.append(dict(team, slot=mon['slot'], base_species=species, base_ability=ability, item=item,
                                  mega_species=target, mega_abilities=target_abilities, mega_ability=target_ability,
                                  base_types=species_types(block), mega_types=species_types(blocks[target])))
        if mega_count > 1:
            errors.append(f'{trainer}/{difficulty}: {mega_count} Mega stones in one team')
    canonical = json.loads((ROOT / 'data/spec/bosses.json').read_text())['teams']
    for row in canonical:
        if row['species'] not in MEGA_BASE_ABILITIES:
            continue
        group = (row['region'], row['category'], row['boss'])
        trainers = (TRAINERS[group],) if group in TRAINERS else ROCKET_TRAINERS.get(group, ())
        expected = maps['ABILITY_'].get(normalize(row['ability']))
        for mega in mega_sets:
            if mega['trainer'] in trainers and mega['slot'] == row['slot'] and mega['mega_ability'] != expected:
                errors.append(f"{mega['trainer']}/{mega['difficulty']}: Mega ability {mega['mega_ability']} != canonical {expected}")
    result = {'team_variants': len(teams), 'trainer_ids': len({t['trainer'] for t in teams}),
              'authored_slots': sum(t['slots'] for t in teams), 'mega_sets': mega_sets, 'errors': errors}
    if include_dynamic:
        rival = json.loads((ROOT / 'data/spec/rival.json').read_text())
        parties = []
        for category, rows in rival['fight_rosters']['categories'].items():
            for difficulty in ('Normal', 'Hard'):
                mons = [dict(species=row['target_final_species'], item=row['final_item'],
                             Ability=row['ability'], EVs=row['hard_evs'] if difficulty == 'Hard' else '',
                             moves=row['phase_moves']['final'], slot=i + 1) for i, row in enumerate(rows)]
                parties.append((f'DYNAMIC_RIVAL_{category.upper()}', difficulty, mons))
        dynamic = audit(catalog, parties)
        result['dynamic_rival'] = dynamic
        result['errors'].extend(dynamic['errors'])
    return result


def mega_fixture(result: dict) -> str:
    rows = sorted({(m['base_species'], m['base_ability'], m['item'], m['mega_species'],
                    m['mega_ability'], *m['mega_types']) for m in result['mega_sets'] + result.get('dynamic_rival', {}).get('mega_sets', [])})
    return ('// Generated by tools/validate_tactica_boss_sets.py --write-mega-fixture.\n'
            '// Frozen production boss expectations; exercised by the battle engine.\n'
            + ''.join('    PARAMETRIZE { species = %s; ability = %s; item = %s; mega = %s; '
                      'megaAbility = %s; type1 = %s; type2 = %s; }\n' % row for row in rows))


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('--report', type=Path)
    parser.add_argument('--write-mega-fixture', action='store_true')
    args = parser.parse_args()
    result = audit()
    if args.report:
        args.report.write_text(json.dumps(result, ensure_ascii=False, indent=2)+'\n')
    print(f"Boss set audit: {result['trainer_ids']} trainer IDs, {result['team_variants']} variants, {result['authored_slots']} slots, {len(result['mega_sets'])} Mega sets")
    print(f"Dynamic rival: {result['dynamic_rival']['team_variants']} variants, {result['dynamic_rival']['authored_slots']} final slots, {len(result['dynamic_rival']['mega_sets'])} Mega sets")
    for error in result['errors']:
        print(error)
    if result['errors']:
        raise SystemExit(1)
    fixture = ROOT / 'test/battle/tactica_boss_mega_cases.h'
    expected = mega_fixture(result)
    if args.write_mega_fixture:
        fixture.write_text(expected)
    elif not fixture.exists() or fixture.read_text() != expected:
        raise SystemExit('Boss Mega battle cases stale: run --write-mega-fixture')


if __name__ == '__main__':
    main()
