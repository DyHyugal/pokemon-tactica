#!/usr/bin/env python3
"""Synchronize the wiki's custom base stats/types with the canonical balance."""

import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
PREFIX = "Object.assign(window.TacticaDexSpecies ||= {}, "


def expected_asset(path: Path, changes: dict[str, dict]) -> str:
    text = path.read_text().strip()
    if not text.startswith(PREFIX) or not text.endswith(");"):
        raise ValueError(f"Unexpected species asset: {path}")
    payload = json.loads(text[len(PREFIX):-2])
    for species, entry in payload.items():
        after = changes.get(species, {})
        for stat in ("hp", "atk", "def", "spa", "spd", "spe"):
            if stat in after:
                entry["stats"][stat] = after[stat]
        if "types" in after:
            entry["types"] = after["types"]
    return PREFIX + json.dumps(payload, separators=(",", ":")) + ");\n"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    changes = {e["species"]: e["after"] for e in json.loads((ROOT / "data/spec/pokemon_balance.json").read_text())["species_changes"]}
    for number in (1, 2, 3):
        path = ROOT / f"docs/assets/tactica-species-{number}.js"
        expected = expected_asset(path, changes)
        if args.check:
            if path.read_text() != expected:
                raise SystemExit(f"Stale wiki balance: {path.relative_to(ROOT)}")
        else:
            path.write_text(expected)
    print("Tactica wiki balance: " + ("checked" if args.check else "synchronized"))


if __name__ == "__main__":
    main()
