#!/usr/bin/env python3
"""Check the authored Tactica V1 sources before changing engine data."""

import collections
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1] / "data" / "spec"
LEARNABLES = json.loads((ROOT.parents[1] / "src/data/pokemon/all_learnables.json").read_text())


def learnable(species, move):
    species_key = re.sub(r"[^A-Z0-9]+", "_", species.upper()).strip("_")
    species_key = {"TOXTRICITY": "TOXTRICITY_AMPED"}.get(species_key, species_key)
    move_key = "MOVE_" + re.sub(r"[^A-Z0-9]+", "_", move.upper()).strip("_")
    return move_key in LEARNABLES.get(species_key, ())


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

access = read("encounter_access_caps.json")
access_caps = {
    (entry["map"], entry["method"]): entry["earliest_access_cap"]
    for entry in access["tables"]
}
standard_pairs = {(entry["map"], entry["method"]) for entry in standard}
require(len(access_caps) == access["table_count"] == len(access["tables"]),
        "duplicate or inconsistent earliest_access_cap table count")
require(set(access_caps) == standard_pairs,
        "earliest_access_cap must cover every standard map/method pair exactly once")
for entry in standard:
    key = (entry["map"], entry["method"])
    require(entry["max_level"] <= access_caps[key],
            f"max level {entry['max_level']} exceeds first-access cap {access_caps[key]}: "
            f"{entry['map']} {entry['method']} {entry.get('time', 'Any')}")

encounter_species = {species for table in standard for species in table["species"]}
required_starter_families = {
    "Bulbasaur": ("BULBASAUR", "IVYSAUR", "VENUSAUR"),
    "Charmander": ("CHARMANDER", "CHARMELEON", "CHARIZARD"),
    "Squirtle": ("SQUIRTLE", "WARTORTLE", "BLASTOISE"),
    "Chikorita": ("CHIKORITA", "BAYLEEF", "MEGANIUM"),
    "Cyndaquil": ("CYNDAQUIL", "QUILAVA", "TYPHLOSION"),
    "Totodile": ("TOTODILE", "CROCONAW", "FERALIGATR"),
    "Treecko": ("TREECKO", "GROVYLE", "SCEPTILE"),
    "Torchic": ("TORCHIC", "COMBUSKEN", "BLAZIKEN"),
    "Mudkip": ("MUDKIP", "MARSHTOMP", "SWAMPERT"),
    "Turtwig": ("TURTWIG", "GROTLE", "TORTERRA"),
    "Chimchar": ("CHIMCHAR", "MONFERNO", "INFERNAPE"),
    "Piplup": ("PIPLUP", "PRINPLUP", "EMPOLEON"),
    "Snivy": ("SNIVY", "SERVINE", "SERPERIOR"),
    "Tepig": ("TEPIG", "PIGNITE", "EMBOAR"),
    "Oshawott": ("OSHAWOTT", "DEWOTT", "SAMUROTT"),
    "Chespin": ("CHESPIN", "QUILLADIN", "CHESNAUGHT"),
    "Fennekin": ("FENNEKIN", "BRAIXEN", "DELPHOX"),
    "Froakie": ("FROAKIE", "FROGADIER", "GRENINJA"),
    "Rowlet": ("ROWLET", "DARTRIX", "DECIDUEYE"),
    "Litten": ("LITTEN", "TORRACAT", "INCINEROAR"),
    "Popplio": ("POPPLIO", "BRIONNE", "PRIMARINA"),
    "Grookey": ("GROOKEY", "THWACKEY", "RILLABOOM"),
    "Scorbunny": ("SCORBUNNY", "RABOOT", "CINDERACE"),
    "Sobble": ("SOBBLE", "DRIZZILE", "INTELEON"),
    "Sprigatito": ("SPRIGATITO", "FLORAGATO", "MEOWSCARADA"),
    "Fuecoco": ("FUECOCO", "CROCALOR", "SKELEDIRGE"),
    "Quaxly": ("QUAXLY", "QUAXWELL", "QUAQUAVAL"),
    "Charcadet": ("CHARCADET", "ARMAROUGE", "CERULEDGE"),
    "Horsea": ("HORSEA", "SEADRA", "KINGDRA"),
    "Elekid": ("ELEKID", "ELECTABUZZ", "ELECTIVIRE"),
    "Magnemite": ("MAGNEMITE", "MAGNETON", "MAGNEZONE"),
    "Pawmi": ("PAWMI", "PAWMO", "PAWMOT"),
    "Wattrel": ("WATTREL", "KILOWATTREL"),
    "Pichu": ("PICHU", "PIKACHU", "RAICHU"),
    "Gligar": ("GLIGAR", "GLISCOR"),
    "Drilbur": ("DRILBUR", "EXCADRILL"),
    "Sandile": ("SANDILE", "KROKOROK", "KROOKODILE"),
    "Golett": ("GOLETT", "GOLURK"),
    "Sandygast": ("SANDYGAST", "PALOSSAND"),
    "Alolan Vulpix": ("VULPIX_ALOLA", "NINETALES_ALOLA"),
    "Snorunt": ("SNORUNT", "GLALIE", "FROSLASS"),
    "Swinub": ("SWINUB", "PILOSWINE", "MAMOSWINE"),
    "Galarian Darumaka": ("DARUMAKA_GALAR", "DARMANITAN_GALAR"),
    "Vanillite": ("VANILLITE", "VANILLISH", "VANILLUXE"),
}
for family, members in required_starter_families.items():
    require(any(any(species.startswith(f"SPECIES_{member}") for member in members)
                for species in encounter_species), f"missing starter family {family}")
    for table in standard:
        if access_caps[(table["map"], table["method"])] >= 32:
            continue
        require(not any(any(species.startswith(f"SPECIES_{member}") for member in members)
                        for species in table["species"]),
                f"starter family {family} available before badge 2: "
                f"{table['map']} {table['method']} {table.get('time', 'Any')}")

tables_by_key = {(x["map"], x["method"], x.get("time", "Any")): x for x in standard}
route36_day = tables_by_key[("MAP_ROUTE36_HNS", "land_mons", "Day")]
route36_night = tables_by_key[("MAP_ROUTE36_HNS", "land_mons", "Night")]
require("SPECIES_CHARMANDER" not in route36_day["species"], "Charmander remains on early Route 36")
require("SPECIES_ELEKID" not in route36_night["species"], "Elekid remains on early Route 36")
required_rare_slots = {
    ("MAP_ROUTE37_HNS", "land_mons", "Day"): "SPECIES_CHARMANDER",
    ("MAP_ROUTE42_HNS", "land_mons", "Day"): "SPECIES_ELEKID",
    ("MAP_ILEX_FOREST_HNS", "land_mons", "Night"): "SPECIES_CHIKORITA",
    ("MAP_ICE_PATH_1F_HNS", "land_mons", "Any"): "SPECIES_NINETALES_ALOLA",
    ("MAP_ICE_PATH_B2F_HNS", "land_mons", "Any"): "SPECIES_DARMANITAN_GALAR_STANDARD",
}
for key, species in required_rare_slots.items():
    require(tables_by_key[key]["species"][3] == species, f"rare starter-family slot {key}")

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
rosters = rival["fight_rosters"]
require(set(rosters["categories"]) == set(starter["categories"]),
        "six rival roster categories")
require(rosters["party_sizes"] == {"before_first_badge": 1, "after_badge_1": 3,
        "after_badge_2": 4, "after_badge_3_and_later": 6}, "rival party progression")
require(rosters["phase_slots"] == {"early": [1, 2, 3], "mid": [1, 2, 3, 4],
        "final": [1, 2, 3, 4, 5, 6]}, "persistent rival slots")
for category, party in rosters["categories"].items():
    require(len(party) == 6 and party[1]["species_source"] == "VAR_FAMILY_RIVAL_SPECIES",
            f"{category}: six members with saved starter in slot 2")
    require(len({row["family"] for row in party}) == 6, f"{category}: repeated family")
    items = [row["final_item"] for row in party if row.get("final_item")]
    require(len(items) == len(set(items)), f"{category}: duplicate rival items")
    for row in party:
        if row["family"] != "saved starter":
            require(all(1 <= len(row["phase_moves"][phase]) <= 4
                        for phase in ("early", "mid", "final")),
                    f"{category}/{row['family']}: incomplete phase sets")
            require(all(len(row["phase_moves"][phase]) == len(set(row["phase_moves"][phase]))
                        for phase in ("early", "mid", "final")),
                    f"{category}/{row['family']}: duplicate move in phase set")
            for phase, species in (("early", row["family"]),
                                   ("mid", row["family"]),
                                   ("final", row["target_final_species"])):
                require(all(learnable(species, move) for move in row["phase_moves"][phase]),
                        f"{category}/{species}/{phase}: move unavailable in build")

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
rocket = read("rocket_progression.json")
require(set(rocket["rosters"]) == {"Proton", "Petrel", "Ariana", "Archer"},
        "four Rocket executive progressions")
require(len(rocket["active_fights"]) == 7, "seven existing Rocket fights")
for name, phases in rocket["rosters"].items():
    final = bosses[("Johto", "Rocket Executive", name)]
    require(len(final) == 6, f"{name}: final party size")
    for phase, size in (("early", 3), ("mid", 4)):
        party = phases[phase]
        require(len(party) == size and len({row["slot"] for row in party}) == size,
                f"{name}/{phase}: unique persistent slots")
        require(all(row["slot"] in {member["slot"] for member in final} for row in party),
                f"{name}/{phase}: unknown final slot")
        items = [row["item"] for row in party if row["item"]]
        require(len(items) == len(set(items)), f"{name}/{phase}: duplicate items")
        require(all(1 <= len(row["moves"]) <= 4 for row in party),
                f"{name}/{phase}: incomplete moves")
        require(all(learnable(row["species"], move) for row in party for move in row["moves"]),
                f"{name}/{phase}: move unavailable in build")
    require([row["slot"] for row in phases["early"]] ==
            [row["slot"] for row in phases["mid"][:3]],
            f"{name}: early members do not persist")
    require({row["slot"] for row in phases["mid"]} <= {row["slot"] for row in final},
            f"{name}: mid members do not persist")
for trainer, (boss, phase) in rocket["active_fights"].items():
    require(trainer.startswith(f"TRAINER_{boss.upper()}") and
            phase in ("early", "mid", "final"), f"{trainer}: invalid Rocket mapping")
for name, expected in [("Albert", 3), ("Hector", 4), ("Blanche", 6)]:
    require(len(next(v for k, v in bosses.items() if k[2] == name)) == expected, f"{name} roster size")
early_items = {
    "Albert": {"Murkrow": "Focus Sash"},
    "Hector": {"Pineco": "Sitrus Berry", "Scyther": "Eviolite"},
    "Blanche": {"Porygon2": "Eviolite", "Ursaring": "Flame Orb", "Maushold": "Wide Lens"},
}
for name, expected in early_items.items():
    roster = next(v for k, v in bosses.items() if k[2] == name)
    actual = {row["species"]: row["item"] for row in roster if row["item"] is not None}
    require(actual == expected, f"{name} early held items: {actual}")

required_items = {
    ("Jasmine", "Corviknight"): "Leftovers", ("Jasmine", "Archaludon"): "Sitrus Berry",
    ("Clément", "Xatu"): "Life Orb", ("Clément", "Gallade"): "Expert Belt",
    ("Marion", "Honchkrow"): "Life Orb", ("Marion", "Weavile"): "Expert Belt",
    ("Petrel", "Muk"): "Black Sludge", ("Petrel", "Weezing"): "Sitrus Berry",
    ("Ariana", "Salazzle"): "Focus Sash", ("Ariana", "Grafaiai"): "Sitrus Berry",
    ("Ariana", "Nidoqueen"): "Life Orb", ("Ariana", "Honchkrow"): "Sharp Beak",
    ("Archer", "Nidoking"): "Life Orb", ("Archer", "Houndoom"): "Focus Sash",
    ("Archer", "Weavile"): "Expert Belt", ("Pierre", "Garganacl"): "Leftovers",
    ("Pierre", "Cradily"): "Sitrus Berry", ("Jeannine", "Toxapex"): "Black Sludge",
    ("Jeannine", "Galarian Weezing"): "Sitrus Berry", ("Jeannine", "Venomoth"): "Focus Sash",
    ("Auguste", "Torkoal"): "Heat Rock", ("Auguste", "Ninetales"): "Leftovers",
}
actual_items = {(row["boss"], row["species"]): row["item"] for row in teams}
for key, expected in required_items.items():
    require(actual_items.get(key) == expected, f"canonical held item {key}: {actual_items.get(key)}")

megas = collections.defaultdict(set)
for (region, category, boss), roster in bosses.items():
    items = [row["item"] for row in roster if row["item"] is not None]
    require(len(items) == len(set(items)), f"duplicate held item in {region}/{category}/{boss}")
    for row in roster:
        if row["species"].startswith("Mega "):
            megas[row["species"]].add((region, category, boss))
require(all(len(users) == 1 for users in megas.values()),
        f"duplicate boss mega: {dict((m, list(u)) for m, u in megas.items() if len(u)>1)}")
jasmine = next(v for k, v in bosses.items() if k[2] == "Jasmine")
require(any(x["species"] == "Mega Aggron" for x in jasmine), "Jasmine Mega Aggron")
require(not any(x["species"] == "Mega Steelix" for x in jasmine), "Jasmine duplicate Mega Steelix")
print("Tactica authored sources valid: encounters/access caps, starters, rival, balance, unique held items and Megas")
