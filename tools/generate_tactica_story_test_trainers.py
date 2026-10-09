#!/usr/bin/env python3
"""Load production story rosters alongside the battle runner's reserved fixtures."""

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
source = (ROOT / "src/data/trainers_hns.h").read_text()
ids = {
    name: int(value)
    for name, value in re.findall(
        r"^#define\s+(TRAINER_\w+)\s+(\d+)\b",
        (ROOT / "include/constants/opponents_hns.h").read_text(),
        re.M,
    )
}
starts = list(re.finditer(r"^\s*\[DIFFICULTY_\w+\]\[(TRAINER_\w+)\]\s*=", source, re.M))
blocks = []
for index, match in enumerate(starts):
    # IDs 0-14 belong to test/battle/trainer_control.party. Retain those
    # fixtures for battle tests; all story boss IDs are outside this range.
    if ids.get(match.group(1), 0) <= 14:
        continue
    end = starts[index + 1].start() if index + 1 < len(starts) else len(source)
    blocks.append(source[match.start() : end])
if not blocks:
    raise SystemExit("No production story trainers found")
(ROOT / "test/battle/tactica_story_trainers.h").write_text(
    "// Generated from production trainers_hns.h; no authored test levels.\n"
    + "".join(blocks)
)
