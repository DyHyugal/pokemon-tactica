#!/usr/bin/env python3
"""Generate the six category teams used by the existing Johto rival battles."""

import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "data/spec/rival.json"
OUTPUT = ROOT / "src/data/tactica_rival_rosters.h"
CATEGORIES = ("fire", "water", "grass", "electric", "ground", "ice")
PHASES = ("early", "mid", "final")
STATS = ("HP", "Atk", "Def", "SpA", "SpD", "Spe")


def symbol(prefix, name):
    return prefix + re.sub(r"[^A-Z0-9]+", "_", name.upper()).strip("_")


def evs(value):
    spread = dict.fromkeys(STATS, 0)
    for segment in value.split(" / "):
        count, stat = segment.strip().split(" ", 1)
        if stat not in spread:
            raise ValueError(f"unknown EV stat: {stat}")
        spread[stat] = int(count)
    if sum(spread.values()) > 510 or any(n > 252 for n in spread.values()):
        raise ValueError(f"invalid EV spread: {value}")
    return "{" + ", ".join(str(spread[stat]) for stat in STATS) + "}"


def generate():
    rows = json.loads(SOURCE.read_text(encoding="utf-8"))["fight_rosters"]["categories"]
    result = ["// Generated from data/spec/rival.json by tools/sync_tactica_rival.py.",
              "// The starter slot retains the species saved in VAR_FAMILY_RIVAL_SPECIES.",
              "static const struct FamilyRivalRosterMon sFamilyRivalRosters[6][PARTY_SIZE] = {"]
    for category in CATEGORIES:
        result.append("    { // " + category)
        for mon in rows[category]:
            if mon["family"] == "saved starter":
                result.append("        {0}, // starter: chosen once and saved")
                continue
            moves = []
            for phase in PHASES:
                phase_moves = mon["phase_moves"][phase]
                moves.append("{" + ", ".join(symbol("MOVE_", move) for move in phase_moves) + "}")
            result.append("        {%s, %s, %s, {%s}, %s}," % (
                symbol("SPECIES_", mon["family"]),
                symbol("ITEM_", mon["final_item"]),
                symbol("NATURE_", mon["nature"]),
                ", ".join(moves),
                evs(mon["hard_evs"])))
        result.append("    },")
    result.append("};")
    return "\n".join(result) + "\n"


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    expected = generate()
    if args.check:
        if not OUTPUT.exists() or OUTPUT.read_text() != expected:
            raise SystemExit("Tactica rival header is out of sync")
    else:
        OUTPUT.write_text(expected)


if __name__ == "__main__":
    main()
