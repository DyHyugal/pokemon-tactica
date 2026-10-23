"""Ordinary-trainer scope and area inheritance must preserve boss contracts."""
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from sync_tactica_trainer_levels import map_floors, trainer_ids


class TrainerLevelTests(unittest.TestCase):
    def test_exact_area_precedes_interior_fallback(self):
        maps = [{"id": "MAP_A", "region_map_section": "AREA"},
                {"id": "MAP_B", "region_map_section": "AREA"},
                {"id": "MAP_ROOM", "region_map_section": "AREA"}]
        tables = [{"map": "MAP_A", "min_level": 7}, {"map": "MAP_A", "min_level": 10},
                  {"map": "MAP_B", "min_level": 20}]
        self.assertEqual(map_floors(tables, maps), {"MAP_A": 10, "MAP_B": 20, "MAP_ROOM": 10})

    def test_unknown_area_is_not_assigned_an_invented_level(self):
        self.assertEqual(map_floors([], [{"id": "MAP_ROOM", "region_map_section": "OTHER"}]), {})

    def test_real_bosses_and_unrelated_trainers_stay_out(self):
        spec = json.loads((ROOT / "data/spec/trainer_levels.json").read_text())
        ids = trainer_ids((ROOT / "src/data/trainers_hns.party").read_text(), set(spec["protected_classes"]))
        self.assertIn("TRAINER_JOEY_HNS", ids)
        self.assertIn("TRAINER_MIKEY_HNS", ids)
        for name in ["TRAINER_FALKNER_1_HNS", "TRAINER_PROTON_1_HNS", "TRAINER_LANCE_1_HNS", "TRAINER_RED_HNS", "TRAINER_RIVAL_CHIKORITA_1_HNS", "TRAINER_SAWYER_1"]:
            self.assertNotIn(name, ids)


if __name__ == "__main__":
    unittest.main()
