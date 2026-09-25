#!/usr/bin/env python3
"""Validate Family Remix data that is otherwise only checked by generators."""

import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def fail(message):
    raise SystemExit(f"Family Remix validation failed: {message}")


def held_items(block):
    return re.findall(r"^[^\n@]+ @ ([^\n]+)$", block, re.M)


def validate_bosses():
    text = (ROOT / "src/data/trainers_hns.party").read_text()
    marker = "/* ========== Family Remix FINAL hard boss parties ========== */"
    if text.count(marker) != 1:
        fail("missing or duplicated FINAL hard boss marker")
    normal, final_parties = text.split(marker, 1)
    rocket_marker = "/* ========== Family Remix FINAL Rocket parties ========== */"
    if final_parties.count(rocket_marker) != 1:
        fail("missing or duplicated FINAL Rocket marker")
    hard, _ = final_parties.split(rocket_marker, 1)
    blocks = re.findall(r"^=== ([A-Z0-9_]+) ===\n(.*?)(?=^=== |\Z)", hard, re.M | re.S)
    if len(blocks) != 22:
        fail(f"expected 22 fixed HARD boss teams, got {len(blocks)}")

    required_ai = ("Basic Trainer", "Try To 2HKO", "Smart Switching", "HP Aware",
                   "PP Stall Prevention", "Assumptions")
    mon_count = 0
    for trainer_id, block in blocks:
        normal_matches = re.findall(
            rf"^=== {re.escape(trainer_id)} ===\n(.*?)(?=^=== |\Z)",
            normal,
            re.M | re.S,
        )
        if len(normal_matches) != 1:
            fail(f"{trainer_id} must have exactly one NORMAL Family Remix roster")
        normal_block = normal_matches[0]

        header = block.split("\n\n", 1)[0]
        if "Difficulty: Hard" not in header:
            fail(f"{trainer_id} is not marked HARD")
        ai_match = re.search(r"^AI: (.+)$", header, re.M)
        if not ai_match or any(flag not in ai_match.group(1) for flag in required_ai):
            fail(f"{trainer_id} is missing the fair strategic AI baseline")
        if any(flag in ai_match.group(1) for flag in ("Smart Trainer", "Omniscient", "Prediction")):
            fail(f"{trainer_id} uses forbidden hidden-information AI")
        items_match = re.search(r"^Items: (.+)$", header, re.M)
        if items_match and len([x for x in items_match.group(1).split(" / ") if x]) > 2:
            fail(f"{trainer_id} has more than two healing items")

        normal_header, normal_party = normal_block.split("\n\n", 1)
        hard_header, hard_party = block.split("\n\n", 1)
        normalize_header = lambda value: "\n".join(
            line for line in value.splitlines()
            if not line.startswith(("AI: ", "Difficulty: "))
        )
        normalize_party = lambda value: re.sub(
            r"^(?:IVs|EVs): .+\n?", "", value, flags=re.M
        ).strip()
        if normalize_header(normal_header) != normalize_header(hard_header):
            fail(f"{trainer_id} NORMAL/HARD trainer content differs")
        if normalize_party(normal_party) != normalize_party(hard_party):
            fail(f"{trainer_id} NORMAL/HARD species, levels, moves or held content differs")
        normal_ai = re.search(r"^AI: (.+)$", normal_header, re.M)
        if not normal_ai or normal_ai.group(1) != "Basic Trainer":
            fail(f"{trainer_id} NORMAL must retain native Basic Trainer AI")
        if re.search(r"^EVs:", normal_party, re.M):
            fail(f"{trainer_id} NORMAL received HARD optimized EVs")
        items = held_items(hard_party)
        if len(items) != len(set(items)):
            fail(f"{trainer_id} contains duplicate held items")

        levels = re.findall(r"^Level: (\d+)$", block, re.M)
        iv_lines = re.findall(r"^IVs: (.+)$", block, re.M)
        ev_lines = re.findall(r"^EVs: (.+)$", block, re.M)
        if not (len(levels) == len(iv_lines) == len(ev_lines)):
            fail(f"{trainer_id} has incomplete level/IV/EV data")
        for level in map(int, levels):
            if not 1 <= level <= 100:
                fail(f"{trainer_id} has invalid level {level}")
        for line in iv_lines:
            values = [int(x) for x in re.findall(r"(\d+) (?:HP|Atk|Def|SpA|SpD|Spe)", line)]
            if len(values) != 6 or any(value > 31 for value in values):
                fail(f"{trainer_id} has invalid IVs: {line}")
        for line in ev_lines:
            values = [int(x) for x in re.findall(r"(\d+) (?:HP|Atk|Def|SpA|SpD|Spe)", line)]
            if not values or any(value > 252 for value in values) or sum(values) > 510:
                fail(f"{trainer_id} has illegal EVs: {line}")
        mon_count += len(levels)

    if mon_count != 127:
        fail(f"expected 127 fixed-team Pokémon, got {mon_count}")

    locked = (
        "Jolteon @ Focus Sash", "Kingdra @ Scope Lens", "Honchkrow @ Life Orb",
        "Magnezone @ Choice Specs", "Exeggutor @ Choice Specs", "- Ancient Power",
    )
    for value in locked:
        if value not in hard:
            fail(f"owner-locked boss datum is missing: {value}")

    hard_by_trainer = dict(blocks)
    jasmine = hard_by_trainer["TRAINER_JASMINE_1_HNS"]
    jasmine_ace = (
        "Aggron @ Aggronite", "Ability: Filter", "Nature: Careful",
        "EVs: 252 HP / 4 Def / 252 SpD", "- Heavy Slam", "- Curse", "- Rest", "- Sleep Talk",
    )
    if any(value not in jasmine for value in jasmine_ace):
        fail("Jasmine does not use the canonical Mega Aggron Curse set")
    if "Steelixite" in jasmine:
        fail("Jasmine still contains the obsolete Mega Steelix ace")


def validate_rockets():
    from sync_tactica_bosses import engine_species
    progression = json.loads((ROOT / "data/spec/rocket_progression.json").read_text())
    final_rows = json.loads((ROOT / "data/spec/bosses.json").read_text())["teams"]
    final_by_boss = {}
    for row in final_rows:
        if row["category"] == "Rocket Executive":
            final_by_boss.setdefault(row["boss"], []).append(row)
    text = (ROOT / "src/data/trainers_hns.party").read_text()
    hard_marker = "/* ========== Family Remix FINAL hard boss parties ========== */"
    rocket_marker = "/* ========== Family Remix FINAL Rocket parties ========== */"
    normal = text.split(hard_marker, 1)[0]
    rocket = text.split(rocket_marker, 1)[1]
    hard_blocks = dict(re.findall(
        r"^=== ([A-Z0-9_]+) ===\n(.*?)(?=^=== |\Z)", rocket, re.M | re.S
    ))
    expected = {}
    for trainer_id, (boss, phase) in progression["active_fights"].items():
        rows = (final_by_boss[boss] if phase == "final" else
                progression["rosters"][boss][phase])
        expected[trainer_id] = tuple(engine_species(row["species"]) for row in rows)
    if set(hard_blocks) != set(expected):
        fail(f"expected seven final HARD Rocket variants, got {sorted(hard_blocks)}")

    required_ai = ("Basic Trainer", "Try To 2HKO", "Smart Switching", "HP Aware",
                   "PP Stall Prevention", "Assumptions")
    for trainer_id, roster in expected.items():
        normal_matches = re.findall(
            rf"^=== {re.escape(trainer_id)} ===\n(.*?)(?=^=== |\Z)", normal, re.M | re.S
        )
        if len(normal_matches) != 1:
            fail(f"{trainer_id} must have exactly one NORMAL Rocket roster")
        normal_block = normal_matches[0]
        hard_block = hard_blocks[trainer_id]
        normal_header, normal_party = normal_block.split("\n\n", 1)
        hard_header, hard_party = hard_block.split("\n\n", 1)
        normalize_header = lambda value: "\n".join(
            line for line in value.splitlines()
            if not line.startswith(("AI: ", "Difficulty: "))
        )
        normalize_party = lambda value: re.sub(
            r"^(?:IVs|EVs): .+\n?", "", value, flags=re.M
        ).strip()
        if normalize_header(normal_header) != normalize_header(hard_header):
            fail(f"{trainer_id} NORMAL/HARD Rocket metadata differs")
        if normalize_party(normal_party) != normalize_party(hard_party):
            fail(f"{trainer_id} NORMAL/HARD Rocket content differs")
        if re.search(r"^EVs:", normal_party, re.M):
            fail(f"{trainer_id} NORMAL received HARD Rocket EVs")
        normal_ai = re.search(r"^AI: (.+)$", normal_header, re.M).group(1)
        if any(flag in normal_ai for flag in required_ai[1:]):
            fail(f"{trainer_id} NORMAL received HARD Rocket AI")
        hard_ai = re.search(r"^AI: (.+)$", hard_header, re.M).group(1)
        if any(flag not in hard_ai for flag in required_ai):
            fail(f"{trainer_id} HARD is missing fair strategic Rocket AI")
        if any(flag in hard_ai for flag in ("Smart Trainer", "Omniscient", "Prediction")):
            fail(f"{trainer_id} HARD uses forbidden hidden-information AI")

        species = tuple(re.findall(
            r"^(?!Level:|Ability:|Nature:|IVs:|EVs:|-)([^\n@]+?)(?: @ .+)?$",
            hard_party,
            re.M,
        ))
        if species != roster:
            fail(f"{trainer_id} Rocket {progression['active_fights'][trainer_id][1]} roster differs: {species}")
        items = held_items(hard_party)
        if len(items) != len(set(items)):
            fail(f"{trainer_id} contains duplicate held items")
        if set(re.findall(r"^Level: (\d+)$", hard_party, re.M)) != {"1"}:
            fail(f"{trainer_id} Rocket source levels must remain runtime placeholders")
        iv_lines = re.findall(r"^IVs: (.+)$", hard_party, re.M)
        ev_lines = re.findall(r"^EVs: (.+)$", hard_party, re.M)
        if len(iv_lines) != len(roster) or any(set(map(int, re.findall(r"\d+", line))) != {31} for line in iv_lines):
            fail(f"{trainer_id} HARD Rocket IVs are not all 31")
        if len(ev_lines) != len(roster):
            fail(f"{trainer_id} HARD Rocket EV data is incomplete")
        for line in ev_lines:
            values = [int(value) for value in re.findall(r"(\d+) (?:HP|Atk|Def|SpA|SpD|Spe)", line)]
            if any(value > 252 for value in values) or sum(values) > 510:
                fail(f"{trainer_id} has illegal HARD Rocket EVs: {line}")


def validate_encounters():
    wild = json.loads((ROOT / "src/data/wild_encounters.json").read_text())
    audit = wild.get("tactica_encounter_audit", {})
    expected = {
        "dataset_version": 4,
        "standard_tables": 405,
        "physical_time_records": 624,
        "real_slots_per_table": 4,
        "slot_rates": [30, 30, 30, 10],
        "headbutt_tables": 4,
        "safari_pools": 53,
    }
    for key, value in expected.items():
        if audit.get(key) != value:
            fail(f"encounter audit {key}: expected {value}, got {audit.get(key)}")

    canonical = json.loads((ROOT / "data/spec/encounters_standard.json").read_text())["tables"]
    canonical_by_key = {
        (table["map"], table["method"], table["time"]): table
        for table in canonical
    }
    if len(canonical_by_key) != 405:
        fail("canonical standard encounter keys are not unique")

    map_group = next(group for group in wild["wild_encounter_groups"] if group.get("for_maps"))
    compiled = {}
    for encounter in map_group.get("encounters", []):
        map_name = encounter.get("map", "")
        if not map_name.endswith("_HNS"):
            continue
        label = encounter.get("base_label", "")
        time = "Night" if label.endswith("_Night") else "Day"
        for method in ("land_mons", "water_mons", "rock_smash_mons", "fishing_mons"):
            if method not in encounter:
                continue
            mons = encounter[method]["mons"]
            if len(mons) != 4:
                fail(f"{label} {method}: expected four real engine slots, got {len(mons)}")
            for mon in mons:
                if not (1 <= mon["min_level"] <= mon["max_level"] <= 100):
                    fail(f"invalid wild level range in {map_name}")
                if not mon["species"].startswith("SPECIES_"):
                    fail("wild species constant is malformed")
            key = (map_name, method, time)
            if key in compiled:
                fail(f"duplicate compiled encounter table: {key}")
            compiled[key] = mons

    expected_compiled = {}
    for table in canonical:
        times = ("Day", "Night") if table["time"] == "Any" else (table["time"],)
        for time in times:
            expected_compiled[(table["map"], table["method"], time)] = table
    if set(compiled) != set(expected_compiled):
        missing = sorted(set(expected_compiled) - set(compiled))
        extra = sorted(set(compiled) - set(expected_compiled))
        fail(f"compiled standard encounter keys differ; missing={missing[:3]}, extra={extra[:3]}")

    for key, table in expected_compiled.items():
        mons = compiled[key]
        species = [mon["species"] for mon in mons]
        if species != table["species"]:
            fail(f"compiled species differ for {key}: {species}")
        levels = {(mon["min_level"], mon["max_level"]) for mon in mons}
        if levels != {(table["min_level"], table["max_level"])}:
            fail(f"compiled levels differ for {key}: {sorted(levels)}")

    constants = (ROOT / "include/constants/wild_encounter.h").read_text()
    if not all(re.search(rf"^#define {name}_WILD_COUNT\s+4$", constants, re.M) for name in ("LAND", "WATER", "ROCK", "FISH")):
        fail("HnS encounter constants do not expose four real slots for every method")
    engine = (ROOT / "src/wild_encounter.c").read_text()
    if engine.count("return ChooseTacticaEncounterSlot(Random());") != 4:
        fail("not every standard HnS encounter selector uses the Tactica four-slot picker")

    special = json.loads((ROOT / "data/family_remix/special_encounter_pools.json").read_text())
    headbutt = [table for table in special["tables"] if table["method"] == "headbutt_mons"]
    safari = [table for table in special["tables"] if table["method"] != "headbutt_mons"]
    if len(headbutt) != 4 or len(safari) != 53:
        fail(f"expected 4 Headbutt and 53 Safari tables, got {len(headbutt)} and {len(safari)}")
    for table in headbutt:
        if table["rates"] != [30, 30, 30, 10] or len(table["species"]) != 4:
            fail(f"invalid Headbutt distribution for {table['map']}")

    headbutt_c = (ROOT / "src/data/family_remix_headbutt.h").read_text()
    if len(re.findall(r"\{MAP_GROUP\(MAP_", headbutt_c)) != 4:
        fail("dedicated Headbutt engine table does not contain four maps")
    headbutt_arrays = re.findall(
        r"static const struct WildPokemon sFamilyHeadbutt\w+\[\]\s*=\s*\{(.*?)\};",
        headbutt_c,
        re.S,
    )
    if len(headbutt_arrays) != 4:
        fail("dedicated Headbutt engine data does not contain four tables")
    for body in headbutt_arrays:
        if len(re.findall(r"\{\d+, \d+, SPECIES_[A-Z0-9_]+\}", body)) != 4:
            fail("Headbutt tables must contain four real engine slots")

    if special.get("status") != "INTEGRATED" or special.get("engine_status", {}).get("safari") != "INTEGRATED_SESSION_ROTATION":
        fail("Safari metadata is not marked integrated")
    safari_by_map = {}
    for table in safari:
        safari_by_map.setdefault(table["map"], []).append(table)
        if table["rates"] != [30, 30, 30, 10] or len(table["species"]) != 4:
            fail(f"invalid Safari distribution for {table['map']} {table['method']}")
        expected_range = (37, 44) if table["region"] == "Johto" else (68, 76)
        if not (expected_range[0] <= table["min_level"] <= table["max_level"] <= expected_range[1]):
            fail(f"invalid Safari level range for {table['map']} {table['method']}")
    if len(safari_by_map) != 10:
        fail(f"expected 10 Safari sectors, got {len(safari_by_map)}")
    for map_name, pools in safari_by_map.items():
        expected_pools = 6 if map_name in {
            "MAP_FUCHSIA_CITY_SAFARI_ZONE_BEACH_HNS",
            "MAP_FUCHSIA_CITY_SAFARI_ZONE_CAVE_HNS",
            "MAP_FUCHSIA_CITY_SAFARI_ZONE_MOUNTAIN_HNS",
        } else 5
        if len(pools) != expected_pools:
            fail(f"{map_name}: expected {expected_pools} rotating pools, got {len(pools)}")

    safari_c = (ROOT / "src/data/family_remix_safari.h").read_text()
    if safari_c.count(".minLevel =") != 53:
        fail("generated Safari engine data does not contain 53 separate pools")
    if len(re.findall(r"\{MAP_GROUP\(MAP_", safari_c)) != 10:
        fail("generated Safari engine data does not contain 10 sectors")


def parse_shop_items(path):
    return re.findall(r"^\s*\.2byte (ITEM_[A-Z0-9_]+)$", path.read_text(), re.M)


def validate_shops():
    tms = parse_shop_items(ROOT / "data/scripts/tm_shop.inc")
    if len(tms) != 92 or len(set(tms)) != 92 or any(not item.startswith("ITEM_TM_") for item in tms):
        fail("TM shop must contain each of the 92 active HnS TMs exactly once")
    tm_shop_script = (ROOT / "data/scripts/tm_shop.inc").read_text(encoding="utf-8")
    for required in (
        "MOVE_RELEARNER_TACTICA_SHOP_MOVES",
        "TeachMoveRelearnerMove",
        "Each lesson costs ¥3,000.",
    ):
        if required not in tm_shop_script:
            fail(f"Tactica move shop is not fully wired: missing {required}")

    items = parse_shop_items(ROOT / "data/scripts/item_shop.inc")
    if len(items) != 259 or len(set(items)) != 259:
        fail("item shop stock is incomplete or contains duplicates")
    safe_ball_names = {"ITEM_AIR_BALLOON", "ITEM_IRON_BALL", "ITEM_LIGHT_BALL", "ITEM_SMOKE_BALL"}
    forbidden_words = ("POTION", "REVIVE", "HEAL", "REPEL", "ESCAPE_ROPE")
    for item in items:
        if (any(word in item for word in forbidden_words)
         or (item.endswith("_BALL") and item not in safe_ball_names)):
            fail(f"progression-bypassing item in special shop: {item}")

    normal_shop_items = set()
    item_shop_path = ROOT / "data/scripts/item_shop.inc"
    for path in (ROOT / "data").rglob("*.inc"):
        if path == item_shop_path or path.name in {"tm_shop.inc", "mega_shop.inc"}:
            continue
        text = path.read_text(encoding="utf-8")
        for match in re.finditer(
            r"^[A-Za-z0-9_]+:\s*\n((?:\s*\.2byte\s+ITEM_[A-Z0-9_]+\s*\n)+)\s*\tpokemartlistend",
            text,
            re.MULTILINE,
        ):
            normal_shop_items.update(re.findall(r"ITEM_[A-Z0-9_]+", match.group(1)))
    duplicates = sorted(set(items) & normal_shop_items)
    if duplicates:
        fail(f"special item shop duplicates normal mart stock: {', '.join(duplicates)}")

    required_strategic_items = {
        "ITEM_FLAME_PLATE", "ITEM_FIRE_MEMORY", "ITEM_NORMAL_GEM",
        "ITEM_LIGHT_BALL", "ITEM_ELECTRIC_SEED", "ITEM_ABSORB_BULB",
        "ITEM_GRIP_CLAW", "ITEM_CHERI_BERRY", "ITEM_MICLE_BERRY",
        "ITEM_KEE_BERRY", "ITEM_MARANGA_BERRY",
    }
    missing = sorted(required_strategic_items - set(items))
    if missing:
        fail(f"special item shop is missing strategic inventory: {', '.join(missing)}")

    form_changes = (ROOT / "src/data/pokemon/form_change_tables.h").read_text()
    playable_mega_stones = set(re.findall(
        r"FORM_CHANGE_BATTLE_MEGA_EVOLUTION_ITEM,\s+SPECIES_[A-Z0-9_]+,\s+(ITEM_[A-Z0-9_]+)",
        form_changes,
    ))
    mega_shop = parse_shop_items(ROOT / "data/scripts/mega_shop.inc")
    if len(playable_mega_stones) != 92:
        fail(f"expected 92 playable Mega Stones, got {len(playable_mega_stones)}")
    if len(mega_shop) != len(set(mega_shop)) or set(mega_shop) != playable_mega_stones:
        fail("Mega Stone shop does not exactly match playable item-based Mega Evolutions")

    item_data = (ROOT / "src/data/items.h").read_text(encoding="utf-8")
    for item in sorted(playable_mega_stones):
        match = re.search(
            rf"\[{item}\]\s*=\s*\{{(?:(?!\n\s*\[ITEM_).)*?\.price\s*=\s*(\d+),",
            item_data,
            re.DOTALL,
        )
        if match is None or int(match.group(1)) != 3000:
            fail(f"{item} must cost exactly 3000 Pokédollars")

    events = (ROOT / "data/event_scripts.s").read_text(encoding="utf-8")
    if events.count('.include "data/scripts/mega_shop.inc"') != 1:
        fail("Mega Stone shop script is not included exactly once")
    goldenrod = json.loads((ROOT / "data/maps/GoldenrodCity_DepartmentStore_5F_hns/map.json").read_text())
    vendors = [event for event in goldenrod["object_events"] if event["script"] == "MegaShop_EventScript"]
    if len(vendors) != 1 or (vendors[0]["x"], vendors[0]["y"]) != (18, 11):
        fail("dedicated Mega Stone vendor is missing beside the Goldenrod TM clerk")


def main():
    validate_bosses()
    validate_rockets()
    validate_encounters()
    validate_shops()
    print("Tactica engine data validation passed: bosses, Rockets, EVs, encounters, Safari and shops")


if __name__ == "__main__":
    main()
