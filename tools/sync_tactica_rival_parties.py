#!/usr/bin/env python3
"""Synchronize the real early rival fights with the canonical progression."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = ROOT / "data/spec/rival.json"
PARTIES = ROOT / "src/data/trainers_hns.party"
STARTERS = ("CHIKORITA", "CYNDAQUIL", "TOTODILE")
SECTION_RE = re.compile(
    r"(?ms)^=== TRAINER_RIVAL_(CHIKORITA|CYNDAQUIL|TOTODILE)_([1-7])_HNS ===\n(.*?)(?=^===|\Z)"
)


def split_section(body: str) -> tuple[str, list[str]]:
    parts = re.split(r"\n\n+", body.strip())
    return parts[0], parts[1:]


def set_level(block: str, level: int) -> str:
    updated, count = re.subn(r"(?m)^Level: \d+$", f"Level: {level}", block, count=1)
    if count != 1:
        raise SystemExit(f"missing Level field in rival block: {block.splitlines()[0]}")
    return updated


def render(source: str) -> str:
    canonical = json.loads(SPEC.read_text(encoding="utf-8"))["fight_rosters"]["runtime_fights"]
    levels = {
        1: canonical["1_cherrygrove_before_badge_1"]["levels"],
        2: canonical["2_azalea_after_badge_2"]["levels"],
        3: canonical["3_burned_tower_after_badge_3"]["levels"],
    }
    levels.update({
        int(name.split("_", 1)[0]): fight_levels
        for name, fight_levels in canonical["4_and_later_after_badge_4"]["levels_by_fight"].items()
    })
    sections: dict[tuple[str, int], tuple[str, list[str]]] = {}
    for match in SECTION_RE.finditer(source):
        sections[(match.group(1), int(match.group(2)))] = split_section(match.group(3))

    # These historical placeholders are used only when the family-starter
    # replacement is disabled. Normal Tactica play replaces every slot from
    # rival.json in FamilyStarter_ResolveRivalMon.
    fight3_extra_from_fight4 = {
        "CHIKORITA": (1, 4),
        "CYNDAQUIL": (2, 4),
        "TOTODILE": (1, 4),
    }

    replacements: dict[tuple[str, int], str] = {}
    for starter in STARTERS:
        for fight in range(1, 8):
            metadata, mons = sections[(starter, fight)]
            needed = len(levels[fight])
            if len(mons) < needed and fight == 2:
                mons.append(sections[(starter, 3)][1][1])
            if len(mons) < needed and fight == 3:
                fight4 = sections[(starter, 4)][1]
                mons.extend(fight4[index] for index in fight3_extra_from_fight4[starter])
            if len(mons) < needed:
                raise SystemExit(f"not enough authored slots for {starter} rival fight {fight}")
            mons = [set_level(block, level) for block, level in zip(mons[:needed], levels[fight])]
            replacements[(starter, fight)] = metadata + "\n\n" + "\n\n".join(mons) + "\n\n"

    def replace(match: re.Match[str]) -> str:
        key = (match.group(1), int(match.group(2)))
        body = replacements.get(key, match.group(3))
        return f"=== TRAINER_RIVAL_{key[0]}_{key[1]}_HNS ===\n{body}"

    return SECTION_RE.sub(replace, source)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    raw = PARTIES.read_bytes()
    newline = "\r\n" if b"\r\n" in raw else "\n"
    source = raw.decode("utf-8").replace("\r\n", "\n")
    rendered = render(source)
    if args.check:
        if rendered != source:
            raise SystemExit("Tactica rival parties are stale; run tools/sync_tactica_rival_parties.py")
        print("Tactica rival parties are synchronized")
        return
    PARTIES.write_bytes(rendered.replace("\n", newline).encode("utf-8"))


if __name__ == "__main__":
    main()
