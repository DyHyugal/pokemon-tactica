#!/usr/bin/env python3
"""Synchronize compiled HnS encounter data with the Tactica source of truth."""

from __future__ import annotations

import json
import argparse
from collections import OrderedDict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC_PATH = ROOT / "data/spec/encounters_standard.json"
ENGINE_PATH = ROOT / "src/data/wild_encounters.json"
METHODS = ("land_mons", "water_mons", "rock_smash_mons", "fishing_mons")


def _preferred_rate(entries: list[dict], table: dict, time: str) -> int:
    candidates = [entry for entry in entries if entry["map"] == table["map"] and table["method"] in entry]
    if not candidates:
        raise ValueError(f"No inherited encounter rate for {table['map']} {table['method']}")

    timed = [entry for entry in candidates if entry["base_label"].endswith(f"_{time}")]
    if timed:
        candidates = timed

    rates = {entry[table["method"]]["encounter_rate"] for entry in candidates}
    if len(rates) != 1:
        raise ValueError(
            f"Ambiguous inherited encounter rate for {table['map']} {table['method']}: {sorted(rates)}"
        )
    return rates.pop()


def _engine_times(table: dict) -> tuple[str, ...]:
    # Keep both inherited encounter frequencies for an Any pool. Species and
    # levels remain identical; only the map's encounter cadence may differ.
    return ("Day", "Night") if table["time"] == "Any" else (table["time"],)


def _base_label(map_name: str, time: str) -> str:
    return f"gTactica_{map_name.removeprefix('MAP_')}_hns_{time}"


def synchronize(check: bool = False) -> None:
    engine = json.loads(ENGINE_PATH.read_text(encoding="utf-8"))
    spec = json.loads(SPEC_PATH.read_text(encoding="utf-8"))
    tables = spec["tables"]
    if len(tables) != 405:
        raise ValueError(f"Expected 405 standard tables, found {len(tables)}")

    map_group = engine["wild_encounter_groups"][0]
    existing = map_group["encounters"]
    inherited_hns = [entry for entry in existing if entry["map"].endswith("_HNS")]
    non_hns = [entry for entry in existing if not entry["map"].endswith("_HNS")]

    generated: OrderedDict[tuple[str, str], dict] = OrderedDict()
    logical_keys: set[tuple[str, str, str]] = set()
    for table in tables:
        key = (table["map"], table["method"], table["time"])
        if key in logical_keys:
            raise ValueError(f"Duplicate canonical table: {key}")
        logical_keys.add(key)

        if table["rates"] != [30, 30, 30, 10] or len(table["species"]) != 4:
            raise ValueError(f"Invalid four-slot contract in canonical table: {key}")

        for time in _engine_times(table):
            record_key = (table["map"], time)
            if record_key not in generated:
                generated[record_key] = {
                    "map": table["map"],
                    "base_label": _base_label(table["map"], time),
                }

            record = generated[record_key]
            method = table["method"]
            if method in record:
                raise ValueError(f"Duplicate physical table: {table['map']} {method} {time}")
            record[method] = {
                "encounter_rate": _preferred_rate(inherited_hns, table, time),
                "mons": [
                    {
                        "min_level": table["min_level"],
                        "max_level": table["max_level"],
                        "species": species,
                    }
                    for species in table["species"]
                ],
            }

    physical_tables = sum(sum(method in record for method in METHODS) for record in generated.values())
    expected_physical_tables = sum(2 if table["time"] == "Any" else 1 for table in tables)
    if physical_tables != expected_physical_tables:
        raise ValueError(
            f"Expected {expected_physical_tables} generated method tables, found {physical_tables}"
        )

    map_group["encounters"] = non_hns + list(generated.values())
    engine.pop("family_remix_encounter_audit", None)
    engine["tactica_encounter_audit"] = {
        "dataset_version": 4,
        "standard_tables": 405,
        "physical_time_records": physical_tables,
        "real_slots_per_table": 4,
        "slot_rates": [30, 30, 30, 10],
        "headbutt_tables": 4,
        "safari_pools": 53,
        "note": "Fishing rods gate access to one authored four-slot pool; encounter frequency is preserved from the imported HnS maps.",
    }
    synchronized = json.dumps(engine, ensure_ascii=False, indent=2)
    if check:
        if synchronized != ENGINE_PATH.read_text(encoding="utf-8"):
            raise SystemExit("Tactica encounters are not synchronized; run tools/sync_tactica_encounters.py")
        return
    ENGINE_PATH.write_text(synchronized, encoding="utf-8")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    synchronize(parser.parse_args().check)
