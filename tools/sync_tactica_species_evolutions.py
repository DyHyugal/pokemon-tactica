#!/usr/bin/env python3
"""Synchronize generated Pokédex evolution metadata from runtime species data."""

from __future__ import annotations

import argparse
import json
import re
import shutil
import subprocess
import sys
from functools import lru_cache
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SPECIES_FILES = tuple(sorted(ROOT.glob("src/data/pokemon/species_info/gen_*_families.h")))
ASSETS = tuple(ROOT / f"docs/assets/tactica-species-{index}.js" for index in (1, 2, 3))

SPECIES_START = re.compile(r"\[(SPECIES_[A-Z0-9_]+)\]\s*=")
ROUTE = re.compile(
    r"\{\s*(EVO_[A-Z0-9_]+),\s*([^,}]+),\s*(SPECIES_[A-Z0-9_]+)"
    r"\s*(?:,\s*CONDITIONS\((.*?)\))?\s*\}", re.S
)
CONDITION = re.compile(r"\{(IF_[A-Z0-9_]+),\s*([^}]+)\}")

PREFIX = "Object.assign(window.TacticaDexSpecies ||= {}, "


@lru_cache(maxsize=1)
def preprocessed_species() -> str:
    # Use the active HnS configuration and expand shared family templates. Keep
    # evolution tokens symbolic so wiki records retain names, not numeric IDs.
    cpp = shutil.which("cpp")
    if cpp is None:
        raise RuntimeError("The C preprocessor (cpp) is required to audit HnS evolutions")
    command = [cpp, "-Iinclude", "-I.", "-DPOKEMON_HNS", "-DMODERN=1", "-DTESTING=0"]
    # Configuration/constants only: global.h/pokemon.h require generated map
    # headers and must not make a wiki audit depend on a previous ROM build.
    preamble = ('#include "config/general.h"\n#include "gba/defines.h"\n'
                '#include "constants/global.h"\n#include "constants/pokemon.h"\n')
    macros = subprocess.run(command + ["-dM", "-"], input=preamble, cwd=ROOT,
                            capture_output=True, text=True, check=True).stdout
    names = re.findall(r"^#define (\w+)", macros, re.M)
    prefixes = ("SPECIES_", "ITEM_", "MOVE_", "TYPE_", "TIME_", "REGION_", "MAP_", "MAPSEC_", "SPIN_")
    kept = {"EVOLUTION", "CONDITIONS", "MON_MALE", "MON_FEMALE", "FRIENDSHIP_EVO_THRESHOLD"}
    source = preamble + "\n".join(f"#undef {n}" for n in names if n.startswith(prefixes) or n in kept)
    source += "\n" + "\n".join(f'#include "{p.relative_to(ROOT)}"' for p in SPECIES_FILES) + "\n"
    return subprocess.run(command + ["-P", "-"], input=source, cwd=ROOT,
                          capture_output=True, text=True, check=True).stdout


@lru_cache(maxsize=1)
def species_blocks() -> dict[str, str]:
    text = preprocessed_species()
    starts = list(SPECIES_START.finditer(text))
    return {m.group(1): text[m.end():starts[i + 1].start() if i + 1 < len(starts) else len(text)]
            for i, m in enumerate(starts)}


def missing_species_entry(species: str) -> dict:
    block = species_blocks()[species]
    def field(pattern: str) -> str:
        match = re.search(pattern, block)
        if not match:
            raise ValueError(f"Missing wiki species field in {species}: {pattern}")
        return match.group(1)
    stats = {name: int(field(r"\." + source + r"\s*=\s*(\d+)\s*,"))
             for name, source in {"hp": "baseHP", "atk": "baseAttack", "def": "baseDefense",
                                  "spa": "baseSpAttack", "spd": "baseSpDefense", "spe": "baseSpeed"}.items()}
    return {
        "name": field(r'\.speciesName\s*=\s*_\("([^"\n]+)"\)'),
        "types": re.findall(r"TYPE_(\w+)", field(r"\.types\s*=\s*MON_TYPES\(([^)]+)\)")),
        "abilities": [a if a != "NONE" else None for a in re.findall(
            r"ABILITY_(\w+)", field(r"\.abilities\s*=\s*\{([^}]+)\}"))],
        "stats": stats,
        "learnset": field(r"\.levelUpLearnset\s*=\s*(\w+)"),
        "evolutions": runtime_evolutions().get(species, []),
    }


@lru_cache(maxsize=1)
def runtime_evolutions() -> dict[str, list[dict]]:
    result: dict[str, list[dict]] = {}
    for current, block in species_blocks().items():
        result.setdefault(current, [])
        routes = list(ROUTE.finditer(block))
        if len(routes) != len(re.findall(r"\{\s*EVO_", block)):
            raise ValueError(f"Unparsed evolution route in {current}")
        for route in routes:
            conditions = []
            if route.group(4):
                conditions = [
                    [match.group(1), " ".join(match.group(2).split())]
                    for match in CONDITION.finditer(route.group(4))
                ]
            result[current].append({"method": route.group(1), "param": route.group(2).strip(),
                                    "target": route.group(3), "conditions": conditions})
    return result


def expected_asset(path: Path, evolutions: dict[str, list[dict]], additions: dict | None = None) -> str:
    text = path.read_text(encoding="utf-8").strip()
    if not text.startswith(PREFIX) or not text.endswith(");"):
        raise ValueError(f"unexpected species asset wrapper: {path.relative_to(ROOT)}")
    payload = json.loads(text[len(PREFIX):-2])
    payload.update(additions or {})
    for species, data in payload.items():
        data["evolutions"] = evolutions.get(species, [])
    return PREFIX + json.dumps(payload, separators=(",", ":")) + ");\n"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()

    evolutions = runtime_evolutions()
    existing = set()
    for path in ASSETS:
        text = path.read_text().strip()
        existing.update(json.loads(text[len(PREFIX):-2]))
    required = {s for s, routes in evolutions.items() if routes}
    required.update(e["target"] for routes in evolutions.values() for e in routes)
    additions = {s: missing_species_entry(s) for s in sorted(required - existing)}
    stale = []
    for path in ASSETS:
        expected = expected_asset(path, evolutions, additions if path == ASSETS[-1] else None)
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
