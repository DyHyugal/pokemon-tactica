"""Boss data regressions: Mega-only talents must never leak into base sets."""
import copy
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from validate_tactica_boss_sets import Catalog, audit, mega_fixture
from sync_tactica_bosses import render_mon


class BossSetTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.catalog = Catalog()

    def check_mon(self, species, ability="None", item="", moves=None, **fields):
        mon = dict(species=species, Ability=ability, item=item, moves=moves or [], slot=1, **fields)
        return audit(self.catalog, [("TRAINER_TEST", "Normal", [mon])])

    def test_actual_full_scope_and_frozen_mega_cases(self):
        result = audit(self.catalog)
        self.assertEqual(result["errors"], [])
        self.assertGreaterEqual(result["trainer_ids"], 117)
        self.assertGreaterEqual(result["authored_slots"], 843)
        self.assertEqual(mega_fixture(result), (ROOT / "test/battle/tactica_boss_mega_cases.h").read_text())

    def test_mega_only_abilities_are_invalid_before_transformation(self):
        for species, ability, item in [("Manectric", "Intimidate", "Expert Belt"),
                                       ("Manectric", "Intimidate", "Manectite"),
                                       ("Gengar", "Shadow Tag", "Gengarite"),
                                       ("Aggron", "Filter", "Aggronite")]:
            with self.subTest(species=species, ability=ability, item=item):
                errors = self.check_mon(species, ability, item)["errors"]
                self.assertTrue(any("illegal base ability" in e for e in errors), errors)

    def test_mega_stone_and_form_pairing(self):
        result = self.check_mon("Aggron", "Sturdy", "Aggronite")
        self.assertEqual(result["errors"], [])
        self.assertEqual(result["mega_sets"][0]["mega_ability"], "ABILITY_FILTER")
        self.assertEqual(result["mega_sets"][0]["base_types"], ["TYPE_STEEL", "TYPE_ROCK"])
        self.assertEqual(result["mega_sets"][0]["mega_types"], ["TYPE_STEEL", "TYPE_STEEL"])
        errors = self.check_mon("Raichu", "Lightning Rod", "Manectite")["errors"]
        self.assertTrue(any("Mega target" in e for e in errors), errors)
        errors = self.check_mon("Gengar Mega", "Shadow Tag", "Gengarite")["errors"]
        self.assertTrue(any("enters already Mega" in e for e in errors), errors)

    def test_multiple_mega_stones_and_illegal_spreads(self):
        mon = dict(species="Aggron", Ability="Sturdy", item="Aggronite", moves=[], slot=1)
        second = dict(mon, slot=2)
        errors = audit(self.catalog, [("TRAINER_TEST", "Normal", [mon, second])])["errors"]
        self.assertTrue(any("2 Mega stones" in e for e in errors), errors)
        errors = self.check_mon("Noivern", EVs="255 SpA / 255 Spe", IVs="32 HP")["errors"]
        self.assertTrue(any("illegal EV" in e for e in errors), errors)
        self.assertTrue(any("illegal IV" in e for e in errors), errors)

    def test_compatible_historical_native_inherited_and_form_moves(self):
        for species, moves in [("Honchkrow", ["Mimic"]), ("Umbreon", ["Growth"]),
                               ("Nidoking", ["Lovely Kiss"]), ("Sylveon", ["Sing"]),
                               ("Rotom Wash", ["Hydro Pump"]), ("Pikachu Starter", ["Thunderbolt", "Surf"]),
                               ("Murkrow", ["Faint Attack"]), ("Mimikyu", ["Swords Dance", "Drain Punch"]),
                               ("Darmanitan Galar", ["Earthquake"])]:
            with self.subTest(species=species):
                self.assertEqual(self.check_mon(species, moves=moves)["errors"], [])
        errors = self.check_mon("Pidgeot", moves=["Extreme Speed"])["errors"]
        self.assertTrue(any("incompatible move" in e for e in errors), errors)

    def test_unknown_names_and_duplicate_moves_are_rejected(self):
        result = self.check_mon("Manectric", "Intimidat", "Expert Bel", ["Thunderbolt", "Thunderbolt"])
        for expected in ["unknown ability", "unknown item", "duplicate moves"]:
            self.assertTrue(any(expected in e for e in result["errors"]), result)

    def test_typed_hidden_power_is_not_silently_rendered_with_arbitrary_ivs(self):
        row = copy.deepcopy(next(r for r in json.loads((ROOT / "data/spec/bosses.json").read_text())["teams"]
                                 if r["boss"] == "Major Bob" and r["species"] == "Manectric"))
        row["moves"][-1] = "Hidden Power Ice"
        with self.assertRaisesRegex(ValueError, "Hidden Power"):
            render_mon(row, {"IVs": "31 HP / 31 Atk / 31 Def / 31 SpA / 31 SpD / 31 Spe"}, True)


if __name__ == "__main__":
    unittest.main()
