#!/usr/bin/env python3
"""Reject wild level evolutions that are overdue at the encounter level."""

from __future__ import annotations

import json
import argparse
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPECIES_FILES = ROOT.glob("src/data/pokemon/species_info/*_families.h")
SPECIES_START = re.compile(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=")
LEVEL_EVOLUTION = re.compile(r"\{EVO_LEVEL,\s*(\d+),\s*(SPECIES_[A-Z0-9_]+)\}")


def level_evolutions() -> dict[str, list[tuple[int, str]]]:
    result: dict[str, list[tuple[int, str]]] = {}
    for path in SPECIES_FILES:
        species = None
        for line in path.read_text(encoding="utf-8").splitlines():
            start = SPECIES_START.match(line)
            if start:
                species = start.group(1)
            if species is None or line.rstrip().endswith("\\"):
                continue
            for level, target in LEVEL_EVOLUTION.findall(line):
                if int(level) > 0:
                    result.setdefault(species, []).append((int(level), target))
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--fix", action="store_true", help="replace unambiguous overdue level evolutions")
    args = parser.parse_args()
    evolutions = level_evolutions()
    spec_path = ROOT / "data/spec/encounters_standard.json"
    document = json.loads(spec_path.read_text(encoding="utf-8"))
    encounters = document["tables"]
    errors: list[str] = []
    changed = 0
    for table in encounters:
        for index, original_species in enumerate(table["species"]):
            species = original_species
            overdue = [(level, target) for level, target in evolutions.get(species, []) if table["min_level"] >= level]
            while args.fix and len(overdue) == 1:
                species = overdue[0][1]
                overdue = [(level, target) for level, target in evolutions.get(species, []) if table["min_level"] >= level]
            if species != original_species:
                table["species"][index] = species
                changed += 1
            if overdue:
                choices = ", ".join(f"{target} at {level}" for level, target in overdue)
                errors.append(
                    f"{table['map']} {table['method']} {table['time']}: {species} "
                    f"at {table['min_level']}-{table['max_level']} should have evolved ({choices})"
                )
    if args.fix and changed:
        spec_path.write_text(json.dumps(document, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        print(f"Tactica encounters: evolved {changed} overdue wild slots")
    if errors:
        for error in errors:
            print(f"ERROR: {error}", file=sys.stderr)
        return 1
    print("Tactica encounters: all deterministic level evolutions match wild levels")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
