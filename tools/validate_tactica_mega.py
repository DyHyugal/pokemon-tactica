#!/usr/bin/env python3
"""Static script-order check only; not a runtime Mega-unlock test."""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

def main():
    source = (ROOT / "data/maps/EcruteakCity_Gym_hns/scripts.inc").read_text()
    # Ignore comments instead of treating text in comments as an instruction.
    source = re.sub(r"/\*.*?\*/", "", source, flags=re.S)
    source = re.sub(r"(?m)@.*$|//.*$", "", source)
    badge = re.search(r"(?m)^\s*setflag\s+FLAG_BADGE04_GET\s*$", source)
    ring = re.search(r"(?m)^\s*giveitem\s+ITEM_MEGA_RING(?:\s|$)", source)
    if not badge or not ring or ring.start() <= badge.start():
        raise SystemExit("Morty script must contain the Mega Ring award after badge 4")
    print("Tactica Mega static check: award instruction follows badge 4; execution, ring gating and START input are not certified")

if __name__ == '__main__':
    main()
