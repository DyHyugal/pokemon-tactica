#!/usr/bin/env python3
"""Check the authored Tactica V1 sources before changing engine data."""

import collections
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1] / "data" / "spec"


def read(name):
    return json.loads((ROOT / name).read_text(encoding="utf-8"))


def require(test, message):
    if not test:
        raise SystemExit(f"Tactica spec: {message}")


standard = read("encounters_standard.json")["tables"]
special = read("encounters_special.json")["tables"]
require(len(standard) == 405, "expected 405 standard tables")
require(len(special) == 57, "expected 57 special tables")
require(sum(x["method"] == "headbutt_mons" for x in special) == 4, "Headbutt table count")
require(sum(x["method"].startswith("safari_pool_") for x in special) == 53, "Safari pool count")
for entry in standard + special:
    require(len(entry["species"]) == 4 and entry["rates"] == [30, 30, 30, 10],
            f"four-slot contract: {entry['map']} {entry['method']}")
unique_standard = {(x["map"], x["method"], x.get("time", "Any")) for x in standard}
require(len(unique_standard) == 405, "duplicate standard map/method/time table")

starter = read("starters.json")
require(len(starter["categories"]) == 6, "six starter categories")
require(all(len(x["species"]) == 5 for x in starter["categories"].values()),
        "five starters per category")
rival = read("rival.json")
require(set(rival["selection"]["counter_categories"]) == set(starter["categories"]),
        "rival categories must match starters")
require(rival["selection"]["counter_categories"]["fire"] == ["water", "ground"],
        "Fire counter categories")
require(rival["selection"]["counter_categories"]["electric"] == ["ground"],
        "Electric counter category")

balance = read("pokemon_balance.json")
require(len(balance["species_changes"]) == 26, "expected 26 custom species entries")
require(len(balance["learnset_changes"]) == 15, "expected 15 custom learnsets")
stats = {x["species"]: x["after"] for x in balance["species_changes"]}
require(stats["SPECIES_CHARIZARD"]["atk"] == 110, "Charizard Atk 110")
require(stats["SPECIES_CHARIZARD_MEGA_X"]["atk"] == 156, "Mega Charizard X Atk 156")
require(stats["SPECIES_CHARIZARD_MEGA_Y"]["atk"] == 120, "Mega Charizard Y unchanged by latest +10")

teams = read("bosses.json")["teams"]
bosses = collections.defaultdict(list)
for row in teams:
    bosses[(row["region"], row["category"], row["boss"])].append(row)
for name, expected in [("Albert", 3), ("Hector", 4), ("Blanche", 6)]:
    require(len(next(v for k, v in bosses.items() if k[2] == name)) == expected, f"{name} roster size")
megas = collections.defaultdict(set)
for (region, category, boss), roster in bosses.items():
    for row in roster:
        if row["species"].startswith("Mega "):
            megas[row["species"]].add((region, category, boss))
require(all(len(users) == 1 for users in megas.values()),
        f"duplicate boss mega: {dict((m, list(u)) for m, u in megas.items() if len(u)>1)}")
jasmine = next(v for k, v in bosses.items() if k[2] == "Jasmine")
require(any(x["species"] == "Mega Aggron" for x in jasmine), "Jasmine Mega Aggron")
require(not any(x["species"] == "Mega Steelix" for x in jasmine), "Jasmine duplicate Mega Steelix")
print("Tactica authored sources valid: 405 standard + 57 special tables, starters, rival, balance, unique Megas")
