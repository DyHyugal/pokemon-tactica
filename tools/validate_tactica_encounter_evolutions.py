#!/usr/bin/env python3
"""Validate deterministic level stages in all standard, Headbutt and Safari pools."""

from __future__ import annotations
import argparse
import json
import re
import sys
from pathlib import Path
from generate_tactica_rival import constants, normalize

ROOT = Path(__file__).resolve().parents[1]
SPECIES_START = re.compile(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=")
LEVEL_EVOLUTION = re.compile(
    r"\{EVO_LEVEL,\s*(\d+),\s*(SPECIES_[A-Z0-9_]+)(?:,\s*CONDITIONS\(\{IF_NOT_REGION,\s*REGION_HISUI\}\))?\}"
)


def level_evolutions():
    result = {}
    for path in ROOT.glob("src/data/pokemon/species_info/*_families.h"):
        species = None
        for line in path.read_text().splitlines():
            start = SPECIES_START.match(line)
            if start:
                species = start.group(1)
            if species is None or line.rstrip().endswith("\\"):
                continue
            for level, target in LEVEL_EVOLUTION.findall(line):
                if int(level) > 0:
                    route = (int(level), target)
                    if route not in result.setdefault(species, []):
                        result[species].append(route)
    return result


def legal_stage(species, level, evolutions):
    parents = {}
    for source, routes in evolutions.items():
        for threshold, target in routes:
            parents.setdefault(target, []).append((threshold, source))
    for _ in range(3):
        incoming = parents.get(species, [])
        if len(incoming) != 1 or level >= incoming[0][0]:
            break
        species = incoming[0][1]
    for _ in range(3):
        routes = [
            (threshold, target)
            for threshold, target in evolutions.get(species, [])
            if threshold <= level
        ]
        if len(routes) != 1:
            break
        species = routes[0][1]
    return species


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--fix",
        action="store_true",
        help="normalize stages at the minimum level; runtime handles thresholds inside a range",
    )
    args = parser.parse_args()
    evolutions = level_evolutions()
    ids = constants(ROOT / "include/constants/species.h", "SPECIES_")
    errors = []
    total = 0
    for kind in ("standard", "special"):
        path = ROOT / f"data/spec/encounters_{kind}.json"
        doc = json.loads(path.read_text())
        changed = 0
        for table in doc["tables"]:
            for i, authored in enumerate(table["species"]):
                species = (
                    authored
                    if authored.startswith("SPECIES_")
                    else ids.get(
                        normalize(
                            authored.replace(" (Spring Form)", "-Spring")
                            .replace(" (Plant Cloak)", "-Plant")
                            .replace(" (Natural Form)", "")
                        )
                    )
                )
                if species is None:
                    raise ValueError(f"unknown species {authored}")
                expected = legal_stage(species, table["min_level"], evolutions)
                if expected == species:
                    continue
                label = f"{kind}: {table['map']} {table['method']} {table.get('time',table.get('pool'))}"
                if args.fix:
                    table["species"][i] = (
                        expected
                        if kind == "standard"
                        else expected.removeprefix("SPECIES_").replace("_", " ").title()
                    )
                    changed += 1
                    print(
                        f"{label}: {authored} -> {table['species'][i]} ({table['min_level']}-{table['max_level']})"
                    )
                else:
                    errors.append(
                        f"{label}: {authored} should be {expected} at {table['min_level']}-{table['max_level']}"
                    )
        if changed:
            path.write_text(json.dumps(doc, ensure_ascii=False, indent=2) + "\n")
        total += changed
    if errors:
        for error in errors:
            print("ERROR: " + error, file=sys.stderr)
        return 1
    print(
        f"Tactica encounters: all 462 tables have legal deterministic level stages ({total} changed)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
