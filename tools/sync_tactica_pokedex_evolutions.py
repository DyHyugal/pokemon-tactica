#!/usr/bin/env python3
"""Synchronize Pokédex evolution data from the canonical runtime sources."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPECIES_FILES = sorted(ROOT.glob("src/data/pokemon/species_info/gen_*_families.h"))
ASSETS = tuple(ROOT / f"docs/assets/tactica-species-{index}.js" for index in (1, 2, 3))
PREFIX = "Object.assign(window.TacticaDexSpecies ||= {}, "
SPECIES_START = re.compile(r"(?m)^\s*\[(SPECIES_[A-Z0-9_]+)\]\s*=")


def balanced(text: str, start: int, opening: str, closing: str) -> str:
    depth = 0
    for index in range(start, len(text)):
        if text[index] == opening:
            depth += 1
        elif text[index] == closing:
            depth -= 1
            if depth == 0:
                return text[start : index + 1]
    raise ValueError(f"unterminated {opening}{closing} block")


def split_top_level(text: str) -> list[str]:
    result: list[str] = []
    start = 0
    depth = 0
    for index, char in enumerate(text):
        if char in "({[":
            depth += 1
        elif char in ")} ]".replace(" ", ""):
            depth -= 1
        elif char == "," and depth == 0:
            result.append(text[start:index].strip())
            start = index + 1
    result.append(text[start:].strip())
    return result


def parse_conditions(field: str) -> list[list[str]]:
    result: list[list[str]] = []
    for match in re.finditer(r"\{(IF_[A-Z0-9_]+)\s*,\s*([^{}]+)\}", field):
        result.append([match.group(1), match.group(2).strip()])
    return result


def runtime_evolutions() -> dict[str, list[dict]]:
    result: dict[str, list[dict]] = {}
    for path in SPECIES_FILES:
        text = path.read_text(encoding="utf-8")
        starts = list(SPECIES_START.finditer(text))
        for index, match in enumerate(starts):
            species = match.group(1)
            section = text[match.end() : starts[index + 1].start() if index + 1 < len(starts) else len(text)]
            evolutions: list[dict] = []
            cursor = 0
            while True:
                route_start = section.find("{EVO_", cursor)
                if route_start < 0:
                    break
                route = balanced(section, route_start, "{", "}")
                cursor = route_start + len(route)
                fields = split_top_level(route[1:-1])
                if len(fields) < 3 or not fields[2].startswith("SPECIES_"):
                    continue
                parsed = {
                    "method": fields[0],
                    "param": fields[1],
                    "target": fields[2],
                    "conditions": parse_conditions(",".join(fields[3:])),
                }
                if parsed not in evolutions:
                    evolutions.append(parsed)
            result[species] = evolutions
    return result


def render_asset(path: Path, canonical: dict[str, list[dict]]) -> str:
    text = path.read_text(encoding="utf-8").strip()
    if not text.startswith(PREFIX) or not text.endswith(");"):
        raise ValueError(f"unexpected asset format: {path.relative_to(ROOT)}")
    data = json.loads(text[len(PREFIX) : -2])
    updated = 0
    for species, entry in data.items():
        if species in canonical and entry.get("evolutions", []) != canonical[species]:
            entry["evolutions"] = canonical[species]
            updated += 1
    rendered = PREFIX + json.dumps(data, ensure_ascii=False, separators=(",", ":")) + ");\n"
    return rendered, updated


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    canonical = runtime_evolutions()
    total = 0
    stale: list[str] = []
    for path in ASSETS:
        rendered, updated = render_asset(path, canonical)
        total += updated
        if rendered != path.read_text(encoding="utf-8"):
            if args.check:
                stale.append(str(path.relative_to(ROOT)))
            else:
                path.write_text(rendered, encoding="utf-8", newline="\n")
    if stale:
        raise SystemExit("Pokédex evolution assets are stale: " + ", ".join(stale))
    print(f"Pokédex evolution assets synchronized ({total} species changed)")


if __name__ == "__main__":
    main()
