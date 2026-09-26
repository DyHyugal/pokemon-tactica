#!/usr/bin/env python3
"""Apply and verify the canonical first-access caps for Tactica encounters."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ACCESS_PATH = ROOT / "data/spec/encounter_access_caps.json"
ENCOUNTERS_PATH = ROOT / "data/spec/encounters_standard.json"


def synchronized_text() -> tuple[str, int]:
    access = json.loads(ACCESS_PATH.read_text(encoding="utf-8"))
    encounters = json.loads(ENCOUNTERS_PATH.read_text(encoding="utf-8"))
    caps = {
        (entry["map"], entry["method"]): entry["earliest_access_cap"]
        for entry in access["tables"]
    }
    if len(caps) != len(access["tables"]):
        raise ValueError("Duplicate map/method in encounter_access_caps.json")

    # A method gate controls when a table can be used, not the level of an
    # already accessible map. Every method on the same map therefore shares
    # the map's first-story-access cap.
    map_caps: dict[str, set[int]] = {}
    for entry in access["tables"]:
        map_caps.setdefault(entry["map"], set()).add(entry["earliest_access_cap"])
        if any(str(item).endswith("_unlock") for item in entry.get("basis", [])):
            raise ValueError(
                f"Method unlock leaked into level scaling for {entry['map']} {entry['method']}"
            )
    inconsistent = {map_name: sorted(values) for map_name, values in map_caps.items() if len(values) != 1}
    if inconsistent:
        raise ValueError(f"Methods on the same map must share one first-access cap: {inconsistent}")
    route36_caps = map_caps.get("MAP_ROUTE36_HNS", set())
    if route36_caps != {17}:
        raise ValueError(f"Route 36 must use the pre-Albert cap 17, found {sorted(route36_caps)}")

    canonical_pairs = {(entry["map"], entry["method"]) for entry in encounters["tables"]}
    if set(caps) != canonical_pairs:
        missing = sorted(canonical_pairs - set(caps))
        extra = sorted(set(caps) - canonical_pairs)
        raise ValueError(f"Access-cap coverage mismatch; missing={missing}, extra={extra}")

    corrected = 0
    for entry in encounters["tables"]:
        cap = caps[(entry["map"], entry["method"])]
        if entry["max_level"] <= cap:
            continue
        difference = entry["max_level"] - cap
        entry["min_level"] = max(1, entry["min_level"] - difference)
        entry["max_level"] = cap
        corrected += 1

    return json.dumps(encounters, ensure_ascii=False, indent=2) + "\n", corrected


def synchronize(check: bool = False) -> None:
    expected, corrected = synchronized_text()
    current = ENCOUNTERS_PATH.read_text(encoding="utf-8")
    if check:
        if expected != current:
            raise SystemExit(
                f"{corrected} encounter tables exceed their first-access cap; "
                "run tools/sync_tactica_encounter_access.py"
            )
        return
    ENCOUNTERS_PATH.write_text(expected, encoding="utf-8")
    print(f"Tactica encounter access synchronized: {corrected} table(s) adjusted")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    synchronize(parser.parse_args().check)
