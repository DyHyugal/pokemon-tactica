#!/usr/bin/env python3
"""Synchronize generated Pokédex evolution metadata from runtime species data."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SPECIES_FILES = tuple(sorted(ROOT.glob("src/data/pokemon/species_info/gen_*_families.h")))
ASSETS = tuple(ROOT / f"docs/assets/tactica-species-{index}.js" for index in (1, 2, 3))

SPECIES_START = re.compile(r"^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=")
ROUTE = re.compile(
    r"\{(EVO_[A-Z0-9_]+),\s*([^,}]+),\s*(SPECIES_[A-Z0-9_]+)"
    r"(?:,\s*CONDITIONS\((.*)\))?\}"
)
CONDITION = re.compile(r"\{(IF_[A-Z0-9_]+),\s*([^}]+)\}")

PREFIX = "Object.assign(window.TacticaDexSpecies ||= {}, "


def runtime_evolutions() -> dict[str, list[dict]]:
    result: dict[str, list[dict]] = {}
    for path in SPECIES_FILES:
        current = None
        for line in path.read_text(encoding="utf-8").splitlines():
            start = SPECIES_START.match(line)
            if start:
                current = start.group(1)
                result.setdefault(current, [])
            if current is None or "{EVO_" not in line:
                continue
            route = ROUTE.search(line)
            if route is None:
                continue
            conditions = []
            if route.group(4):
                conditions = [
                    [match.group(1), match.group(2).strip()]
                    for match in CONDITION.finditer(route.group(4))
                ]
            result[current].append(
                {
                    "method": route.group(1),
                    "param": route.group(2).strip(),
                    "target": route.group(3),
                    "conditions": conditions,
                }
            )
    return result


def expected_asset(path: Path, evolutions: dict[str, list[dict]]) -> str:
    text = path.read_text(encoding="utf-8").strip()
    if not text.startswith(PREFIX) or not text.endswith(");"):
        raise ValueError(f"unexpected species asset wrapper: {path.relative_to(ROOT)}")
    payload = json.loads(text[len(PREFIX):-2])
    for species, data in payload.items():
        data["evolutions"] = evolutions.get(species, [])
    return PREFIX + json.dumps(payload, separators=(",", ":")) + ");\n"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()

    evolutions = runtime_evolutions()
    stale = []
    for path in ASSETS:
        expected = expected_asset(path, evolutions)
        current = path.read_text(encoding="utf-8")
        if current != expected:
            stale.append(path)
            if not args.check:
                path.write_text(expected, encoding="utf-8")

    if stale and args.check:
        for path in stale:
            print(f"out of date: {path.relative_to(ROOT)}", file=sys.stderr)
        return 1

    action = "checked" if args.check else "updated"
    print(f"Tactica species evolutions: {action} {len(ASSETS)} generated assets")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
