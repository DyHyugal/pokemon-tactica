#!/usr/bin/env python3
"""Synchronize fixed NORMAL/HARD boss parties with data/spec/bosses.json."""

from __future__ import annotations

import json
import re
import argparse
from collections import OrderedDict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PARTY_PATH = ROOT / "src/data/trainers_hns.party"
SPEC_PATH = ROOT / "data/spec/bosses.json"
HARD_MARKER = "/* ========== Family Remix FINAL hard boss parties ========== */"
ROCKET_MARKER = "/* ========== Family Remix FINAL Rocket parties ========== */"

TRAINERS = {
    ("Johto", "Gym", "Albert"): "TRAINER_FALKNER_1_HNS",
    ("Johto", "Gym", "Hector"): "TRAINER_BUGSY_1_HNS",
    ("Johto", "Gym", "Blanche"): "TRAINER_WHITNEY_1_HNS",
    ("Johto", "Gym", "Mortimer"): "TRAINER_MORTY_1_HNS",
    ("Johto", "Gym", "Chuck"): "TRAINER_CHUCK_1_HNS",
    ("Johto", "Gym", "Jasmine"): "TRAINER_JASMINE_1_HNS",
    ("Johto", "Gym", "Frédo"): "TRAINER_PRYCE_1_HNS",
    ("Johto", "Gym", "Sandra"): "TRAINER_CLAIR_1_HNS",
    ("Johto", "Elite Four", "Clément"): "TRAINER_WILL_1_HNS",
    ("Johto", "Elite Four", "Koga"): "TRAINER_KOGA_1_HNS",
    ("Johto", "Elite Four", "Aldo"): "TRAINER_BRUNO_1_HNS",
    ("Johto", "Elite Four", "Marion"): "TRAINER_KAREN_1_HNS",
    ("Johto", "Champion", "Maître"): "TRAINER_LANCE_1_HNS",
    ("Kanto", "Gym", "Pierre"): "TRAINER_BROCK_HNS",
    ("Kanto", "Gym", "Ondine"): "TRAINER_MISTY_HNS",
    ("Kanto", "Gym", "Major Bob"): "TRAINER_LTSURGE_HNS",
    ("Kanto", "Gym", "Erika"): "TRAINER_ERIKA_HNS",
    ("Kanto", "Gym", "Morgane"): "TRAINER_SABRINA_HNS",
    ("Kanto", "Gym", "Jeannine"): "TRAINER_JANINE_HNS",
    ("Kanto", "Gym", "Auguste"): "TRAINER_BLAINE_HNS",
    ("Kanto", "Gym", "Blue"): "TRAINER_BLUE_HNS",
    ("Kanto", "Champion Rematch", "Maître"): "TRAINER_LANCE_2_HNS",
}

ROCKET_TRAINERS = {
    ("Johto", "Rocket Executive", "Proton"): ("TRAINER_PROTON_1_HNS", "TRAINER_PROTON_2_HNS"),
    ("Johto", "Rocket Executive", "Petrel"): ("TRAINER_PETREL_1_HNS", "TRAINER_PETREL_2_HNS"),
    ("Johto", "Rocket Executive", "Ariana"): ("TRAINER_ARIANA_1_HNS", "TRAINER_ARIANA_2_HNS"),
    ("Johto", "Rocket Executive", "Archer"): ("TRAINER_ARCHER_HNS",),
}

SPECIES_ALIASES = {
    "Alolan Ninetales": "Ninetales-Alola",
    "Arctozolt (Galvagla)": "Arctozolt",
    "Galarian Weezing": "Weezing-Galar",
    "Galarian Slowking": "Slowking-Galar",
    "Alolan Muk": "Muk-Alola",
    "Hisuian Zoroark": "Zoroark-Hisui",
}

# These final forms are not legal at the authored level. Their current
# pre-evolution blocks retain a compatible ability and set while the canonical
# held-item assignment still applies.
LEGAL_PRE_EVOLUTIONS = {
    ("Ursaring", 29): "Teddiursa",
    ("Hydreigon", 61): "Zweilous",
}

EV_SPREADS = {
    "EV:SPE": "252 SpA / 252 Spe / 4 HP",
    "SpA / Speed": "252 SpA / 252 Spe / 4 HP",
    "252 SpA / 252 Spe": "252 SpA / 252 Spe / 4 HP",
    "Atk / Speed": "252 Atk / 252 Spe / 4 HP",
    "252 Atk / 252 Spe": "252 Atk / 252 Spe / 4 HP",
    "Atk / HP": "252 HP / 252 Atk / 4 SpD",
    "HP / Atk": "252 HP / 252 Atk / 4 SpD",
    "SpA / HP": "252 HP / 252 SpA / 4 SpD",
    "HP / SpA": "252 HP / 252 SpA / 4 SpD",
    "HP / Def": "252 HP / 252 Def / 4 SpD",
    "HP / SpD": "252 HP / 252 SpD / 4 Def",
    "HP / Speed": "252 HP / 252 Spe / 4 SpD",
    "EV:TANK": "252 HP / 128 Def / 128 SpD",
    "252 HP / 4 Def / 252 SpD": "252 HP / 4 Def / 252 SpD",
}


def engine_species(name: str) -> str:
    name = name.removeprefix("Mega ")
    if name.startswith("Raichu "):
        return "Raichu"
    if name.startswith("Charizard "):
        return "Charizard"
    return SPECIES_ALIASES.get(name, name)


def parse_mon(block: str) -> dict:
    lines = block.splitlines()
    first = lines[0]
    species, item = (first.split(" @ ", 1) + [None])[:2] if " @ " in first else (first, None)
    fields = {}
    for key in ("Level", "Ability", "Nature", "IVs", "EVs"):
        fields[key] = next((line.split(": ", 1)[1] for line in lines if line.startswith(f"{key}: ")), None)
    fields.update(species=species, item=item, moves=[line[2:] for line in lines if line.startswith("- ")])
    return fields


def render_mon(canonical: dict, current: dict, hard: bool) -> str:
    level = canonical["level"] if canonical["level"] is not None else int(current["Level"])
    desired_species = engine_species(canonical["species"])
    legal_species = LEGAL_PRE_EVOLUTIONS.get((desired_species, level))
    if legal_species is not None:
        if current["species"] != legal_species:
            raise ValueError(f"Expected legal {legal_species} fallback, found {current['species']}")
        return "\n".join(
            [current["species"] + (f" @ {canonical['item']}" if canonical["item"] else "")]
            + [f"{key}: {current[key]}" for key in ("Level", "Ability", "Nature", "IVs", "EVs") if current[key]]
            + [f"- {move}" for move in current["moves"]]
        )

    first = desired_species + (f" @ {canonical['item']}" if canonical["item"] else "")
    moves = [
        "Hidden Power" if move == "Hidden Power Ice" else move
        for move in canonical["moves"]
        if move not in {"—", "�"}
    ]
    lines = [
        first,
        f"Level: {level}",
        f"Ability: {canonical['ability']}",
        f"Nature: {canonical['nature']}",
        f"IVs: {current['IVs']}",
    ]
    if hard:
        evs = current["EVs"] if current["species"] == desired_species else EV_SPREADS.get(canonical["evs"])
        if evs is None:
            raise ValueError(f"No HARD EV mapping for {canonical['boss']} slot {canonical['slot']}: {canonical['evs']}")
        lines.append(f"EVs: {evs}")
    lines.extend(f"- {move}" for move in moves)
    return "\n".join(lines)


def replace_party(section: str, trainer_id: str, canonical: list[dict], hard: bool) -> str:
    pattern = re.compile(rf"(?m)^=== {re.escape(trainer_id)} ===\n(.*?)(?=^=== |\Z)", re.S)
    matches = list(pattern.finditer(section))
    if len(matches) != 1:
        raise ValueError(f"Expected one {trainer_id} block, found {len(matches)}")
    match = matches[0]
    block = match.group(1).rstrip()
    header, party = block.split("\n\n", 1)
    current = [parse_mon(mon) for mon in party.split("\n\n")]
    if len(current) != len(canonical):
        raise ValueError(f"{trainer_id}: expected {len(canonical)} slots, found {len(current)}")
    rendered = "\n\n".join(render_mon(row, current[row["slot"] - 1], hard) for row in canonical)
    replacement = f"=== {trainer_id} ===\n{header}\n\n{rendered}\n\n"
    return section[:match.start()] + replacement + section[match.end():]


def synchronize(check: bool = False) -> None:
    spec = json.loads(SPEC_PATH.read_text(encoding="utf-8"))
    groups: OrderedDict[tuple[str, str, str], list[dict]] = OrderedDict()
    for row in spec["teams"]:
        groups.setdefault((row["region"], row["category"], row["boss"]), []).append(row)
    if set(groups) != set(TRAINERS) | set(ROCKET_TRAINERS):
        raise ValueError("Boss mapping does not match the canonical fixed-team groups")

    text = PARTY_PATH.read_text(encoding="utf-8")
    normal, remainder = text.split(HARD_MARKER, 1)
    hard, rocket = remainder.split(ROCKET_MARKER, 1)
    for key, trainer_id in TRAINERS.items():
        canonical = groups[key]
        normal = replace_party(normal, trainer_id, canonical, False)
        hard = replace_party(hard, trainer_id, canonical, True)
    for key, trainer_ids in ROCKET_TRAINERS.items():
        canonical = groups[key]
        for trainer_id in trainer_ids:
            normal = replace_party(normal, trainer_id, canonical, False)
            rocket = replace_party(rocket, trainer_id, canonical, True)
    synchronized = (normal + HARD_MARKER + hard + ROCKET_MARKER + rocket).rstrip() + "\n"
    if check:
        if synchronized != text:
            raise SystemExit("Tactica boss parties are not synchronized; run tools/sync_tactica_bosses.py")
        return
    PARTY_PATH.write_text(synchronized, encoding="utf-8")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    synchronize(parser.parse_args().check)
