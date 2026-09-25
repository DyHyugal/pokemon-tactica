#!/usr/bin/env python3
"""Validate Tactica species and active level-up learnsets against the canonical JSON."""

from __future__ import annotations

import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC_PATH = ROOT / "data/spec/pokemon_balance.json"
CONFIG_PATH = ROOT / "include/config/pokemon.h"

STAT_FIELDS = {
    "hp": "baseHP",
    "atk": "baseAttack",
    "def": "baseDefense",
    "spa": "baseSpAttack",
    "spd": "baseSpDefense",
    "spe": "baseSpeed",
}


def fail(message: str) -> None:
    raise SystemExit(f"Tactica balance validation failed: {message}")


def extract_block(text: str, opening: str) -> str:
    match = re.search(rf"{re.escape(opening)}\s*=\s*\{{(.*?)^\s*\}},", text, re.M | re.S)
    if not match:
        fail(f"missing block {opening}")
    return match.group(1)


def resolve_number(raw: str, source: str) -> int:
    raw = raw.strip()
    if raw.isdigit():
        return int(raw)
    macro = re.search(rf"^\s*#define\s+{re.escape(raw)}\s+(.+)$", source, re.M)
    if not macro:
        fail(f"cannot resolve numeric expression {raw}")
    expression = macro.group(1)
    current_value = re.search(r"\?\s*(\d+)\s*:", expression)
    if current_value:
        return int(current_value.group(1))
    value = re.search(r"\b(\d+)\b", expression)
    if not value:
        fail(f"cannot resolve macro {raw}: {expression}")
    return int(value.group(1))


def validate_species(spec: dict) -> None:
    changes = spec["species_changes"]
    if len(changes) != 26:
        fail(f"expected 26 species changes, got {len(changes)}")
    if len({row["species"] for row in changes}) != len(changes):
        fail("species changes contain duplicate species")

    sources: dict[str, str] = {}
    for row in changes:
        path = row["source_file"]
        source = sources.setdefault(path, (ROOT / path).read_text(encoding="utf-8"))
        block = extract_block(source, f"[{row['species']}]")
        expected = row["after"]

        for key, field in STAT_FIELDS.items():
            if key not in expected:
                continue
            match = re.search(rf"\.{field}\s*=\s*([^,]+),", block)
            if not match:
                fail(f"{row['species']} is missing {field}")
            actual = resolve_number(match.group(1), source)
            if actual != expected[key]:
                fail(f"{row['species']} {key}: expected {expected[key]}, got {actual}")

        if "types" in expected:
            match = re.search(r"\.types\s*=\s*MON_TYPES\(([^)]+)\)", block)
            if not match:
                fail(f"{row['species']} is missing types")
            actual = [value.strip().removeprefix("TYPE_") for value in match.group(1).split(",")]
            if actual != expected["types"]:
                fail(f"{row['species']} types: expected {expected['types']}, got {actual}")

        if "hidden_ability" in expected or "ability" in expected:
            match = re.search(r"\.abilities\s*=\s*\{([^}]+)\}", block)
            if not match:
                fail(f"{row['species']} is missing abilities")
            abilities = [value.strip().removeprefix("ABILITY_") for value in match.group(1).split(",")]
            if "hidden_ability" in expected and abilities[2] != expected["hidden_ability"]:
                fail(f"{row['species']} hidden ability: expected {expected['hidden_ability']}, got {abilities[2]}")
            if "ability" in expected and any(value != expected["ability"] for value in abilities):
                fail(f"{row['species']} ability: expected only {expected['ability']}, got {abilities}")


def validate_learnsets(spec: dict) -> None:
    changes = spec["learnset_changes"]
    if len(changes) != 15:
        fail(f"expected 15 learnset changes, got {len(changes)}")

    config = CONFIG_PATH.read_text(encoding="utf-8")
    generation = re.search(r"^#define P_LVL_UP_LEARNSETS\s+GEN_(\d+)", config, re.M)
    if not generation:
        fail("cannot determine the active level-up learnset generation")
    active_path = ROOT / f"src/data/pokemon/level_up_learnsets/gen_{generation.group(1)}.h"
    active = active_path.read_text(encoding="utf-8")

    blocks: dict[str, str] = {}
    for row in changes:
        name = row["learnset"]
        if name not in blocks:
            match = re.search(
                rf"static const struct LevelUpMove {re.escape(name)}\[\]\s*=\s*\{{(.*?)\n\}};",
                active,
                re.S,
            )
            if not match:
                fail(f"active learnsets are missing {name}")
            blocks[name] = match.group(1)
        block = blocks[name]
        move = row.get("move_after", row.get("move"))
        desired = rf"LEVEL_UP_MOVE\(\s*{row['level']},\s*{re.escape(move)}\)"
        if len(re.findall(desired, block)) != 1:
            fail(f"{name} must contain exactly one level {row['level']} {move}")
        if row["operation"] == "replace":
            old = rf"LEVEL_UP_MOVE\(\s*{row['level']},\s*{re.escape(row['move_before'])}\)"
            if re.search(old, block):
                fail(f"{name} still contains replaced move {row['move_before']} at level {row['level']}")


def main() -> None:
    spec = json.loads(SPEC_PATH.read_text(encoding="utf-8"))
    validate_species(spec)
    validate_learnsets(spec)
    print("Tactica balance validation passed: 26 species and 15 active learnset changes")


if __name__ == "__main__":
    main()
