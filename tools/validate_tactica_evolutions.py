#!/usr/bin/env python3
"""Validate Pokémon Tactica's canonical evolution substitutions.

This validator protects docs/spec/EVOLUTIONS.md at source level.  It intentionally
checks generic invariants instead of generated wiki output.
"""

from __future__ import annotations

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SPECIES_FILES = sorted(ROOT.glob("src/data/pokemon/species_info/gen_*_families.h"))

SPECIES_START = re.compile(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=")
ROUTE_START = re.compile(
    r"\{(EVO_[A-Z0-9_]+),\s*([^,}]+),\s*(SPECIES_[A-Z0-9_]+)"
)

SIMPLE_TRADE = {
    ("SPECIES_KADABRA", "SPECIES_ALAKAZAM"),
    ("SPECIES_MACHOKE", "SPECIES_MACHAMP"),
    ("SPECIES_GRAVELER", "SPECIES_GOLEM"),
    ("SPECIES_GRAVELER_ALOLA", "SPECIES_GOLEM_ALOLA"),
    ("SPECIES_HAUNTER", "SPECIES_GENGAR"),
    ("SPECIES_BOLDORE", "SPECIES_GIGALITH"),
    ("SPECIES_GURDURR", "SPECIES_CONKELDURR"),
    ("SPECIES_KARRABLAST", "SPECIES_ESCAVALIER"),
    ("SPECIES_SHELMET", "SPECIES_ACCELGOR"),
    ("SPECIES_PHANTUMP", "SPECIES_TREVENANT"),
    ("SPECIES_PUMPKABOO_AVERAGE", "SPECIES_GOURGEIST_AVERAGE"),
    ("SPECIES_PUMPKABOO_SMALL", "SPECIES_GOURGEIST_SMALL"),
    ("SPECIES_PUMPKABOO_LARGE", "SPECIES_GOURGEIST_LARGE"),
    ("SPECIES_PUMPKABOO_SUPER", "SPECIES_GOURGEIST_SUPER"),
}

HELD_TRADE = {
    ("SPECIES_POLIWHIRL", "SPECIES_POLITOED"): "ITEM_KINGS_ROCK",
    ("SPECIES_SLOWPOKE", "SPECIES_SLOWKING"): "ITEM_KINGS_ROCK",
    ("SPECIES_ONIX", "SPECIES_STEELIX"): "ITEM_METAL_COAT",
    ("SPECIES_RHYDON", "SPECIES_RHYPERIOR"): "ITEM_PROTECTOR",
    ("SPECIES_SEADRA", "SPECIES_KINGDRA"): "ITEM_DRAGON_SCALE",
    ("SPECIES_SCYTHER", "SPECIES_SCIZOR"): "ITEM_METAL_COAT",
    ("SPECIES_ELECTABUZZ", "SPECIES_ELECTIVIRE"): "ITEM_ELECTIRIZER",
    ("SPECIES_MAGMAR", "SPECIES_MAGMORTAR"): "ITEM_MAGMARIZER",
    ("SPECIES_PORYGON", "SPECIES_PORYGON2"): "ITEM_UPGRADE",
    ("SPECIES_PORYGON2", "SPECIES_PORYGON_Z"): "ITEM_DUBIOUS_DISC",
    ("SPECIES_FEEBAS", "SPECIES_MILOTIC"): "ITEM_PRISM_SCALE",
    ("SPECIES_DUSCLOPS", "SPECIES_DUSKNOIR"): "ITEM_REAPER_CLOTH",
    ("SPECIES_CLAMPERL", "SPECIES_HUNTAIL"): "ITEM_DEEP_SEA_TOOTH",
    ("SPECIES_CLAMPERL", "SPECIES_GOREBYSS"): "ITEM_DEEP_SEA_SCALE",
    ("SPECIES_SPRITZEE", "SPECIES_AROMATISSE"): "ITEM_SACHET",
    ("SPECIES_SWIRLIX", "SPECIES_SLURPUFF"): "ITEM_WHIPPED_DREAM",
}

IMPOSSIBLE_MECHANICS = {
    ("SPECIES_INKAY", "SPECIES_MALAMAR"),
    ("SPECIES_FINIZEN", "SPECIES_PALAFIN_ZERO"),
    ("SPECIES_YAMASK_GALAR", "SPECIES_RUNERIGUS"),
    ("SPECIES_URSARING", "SPECIES_URSALUNA"),
    ("SPECIES_MELTAN", "SPECIES_MELMETAL"),
}

APPROVED_LINKING_CORD = SIMPLE_TRADE | set(HELD_TRADE) | IMPOSSIBLE_MECHANICS


def load_routes():
    routes = []
    for path in SPECIES_FILES:
        source = None
        for line_no, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
            start = SPECIES_START.match(line)
            if start:
                source = start.group(1)
            if source is None:
                continue
            match = ROUTE_START.search(line)
            if match:
                routes.append(
                    {
                        "source": source,
                        "method": match.group(1),
                        "param": match.group(2).strip(),
                        "target": match.group(3),
                        "text": line.strip(),
                        "path": path,
                        "line": line_no,
                    }
                )
    return routes


def fail(errors, route, message):
    errors.append(f"{route['path'].relative_to(ROOT)}:{route['line']}: {message}: {route['text']}")


def main() -> int:
    routes = load_routes()
    errors = []

    for route in routes:
        pair = (route["source"], route["target"])

        if route["method"] == "EVO_TRADE":
            fail(errors, route, "legacy EVO_TRADE route is forbidden in Tactica")

        if route["method"] == "EVO_ITEM" and "IF_MIN_LEVEL" in route["text"]:
            fail(errors, route, "artificial IF_MIN_LEVEL on EVO_ITEM is forbidden")

        if route["method"] == "EVO_ITEM" and route["param"] == "ITEM_LINKING_CORD":
            if pair not in APPROVED_LINKING_CORD:
                fail(errors, route, "Linking Cord used outside the approved canonical substitution set")
                continue

            held_item = HELD_TRADE.get(pair)
            if held_item is None:
                if "IF_HOLD_ITEM" in route["text"]:
                    fail(errors, route, "simple/impossible Linking Cord route unexpectedly requires a held item")
            elif f"IF_HOLD_ITEM, {held_item}" not in route["text"]:
                fail(errors, route, f"Linking Cord route must require held item {held_item}")

    for pair in sorted(APPROVED_LINKING_CORD):
        source, target = pair
        matches = [
            route for route in routes
            if route["source"] == source and route["target"] == target
        ]
        linking = [
            route for route in matches
            if route["method"] == "EVO_ITEM" and route["param"] == "ITEM_LINKING_CORD"
        ]
        if len(linking) != 1:
            errors.append(
                f"{source} -> {target}: expected exactly one Linking Cord route, got {len(linking)}"
            )
            continue

        for route in matches:
            if route is linking[0]:
                continue
            fail(errors, route, f"obsolete alternate route remains for {source} -> {target}")

    for pair, held_item in sorted(HELD_TRADE.items()):
        source, target = pair
        for route in routes:
            if (
                route["source"] == source
                and route["target"] == target
                and route["method"] == "EVO_ITEM"
                and route["param"] == held_item
            ):
                fail(errors, route, f"held trade item {held_item} must not be directly usable")

    if errors:
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "Tactica evolutions: canonical Linking Cord substitutions and item constraints valid "
        f"({len(APPROVED_LINKING_CORD)} protected routes)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
