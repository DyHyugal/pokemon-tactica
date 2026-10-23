"""Evolution data must include templates, multiline rules and active HnS branches."""
import json
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from sync_tactica_species_evolutions import runtime_evolutions


class WikiEvolutionTests(unittest.TestCase):
    def test_multiline_spins_keep_sweets_and_time(self):
        routes = runtime_evolutions()["SPECIES_MILCERY"]
        self.assertEqual(len(routes), 63)
        self.assertTrue(all(r["method"] == "EVO_SPIN" and len(r["conditions"]) == 2 for r in routes))
        self.assertIn(["IF_HOLD_ITEM", "ITEM_STRAWBERRY_SWEET"], routes[0]["conditions"])

    def test_shared_templates_and_active_shedinja_branch(self):
        r = runtime_evolutions()
        self.assertEqual(r["SPECIES_FLABEBE_RED"][0]["target"], "SPECIES_FLOETTE_RED")
        self.assertEqual(r["SPECIES_SCATTERBUG_POLAR"][0]["target"], "SPECIES_SPEWPA_POLAR")
        self.assertEqual(r["SPECIES_SPEWPA_POLAR"][0]["target"], "SPECIES_VIVILLON_POLAR")
        shedinja = [e for e in r["SPECIES_NINCADA"] if e["method"] == "EVO_SPLIT_FROM_EVO"]
        self.assertEqual(len(shedinja), 1)
        self.assertIn(["IF_BAG_ITEM_COUNT", "ITEM_POKE_BALL, 1"], shedinja[0]["conditions"])

    def test_held_trade_item_and_impossible_mechanics_remain_canonical(self):
        r = runtime_evolutions()
        for species, item in [("SEADRA", "DRAGON_SCALE"), ("ELECTABUZZ", "ELECTIRIZER"), ("RHYDON", "PROTECTOR")]:
            route = r["SPECIES_" + species][0]
            self.assertEqual(route["param"], "ITEM_LINKING_CORD")
            self.assertEqual(route["conditions"], [["IF_HOLD_ITEM", "ITEM_" + item]])
        for species in ["INKAY", "FINIZEN", "YAMASK_GALAR", "URSARING", "MELTAN"]:
            self.assertEqual(r["SPECIES_" + species][0]["param"], "ITEM_LINKING_CORD")

    def test_rendered_conditions_are_readable_in_both_languages(self):
        subprocess.run(["node", str(ROOT / "tools/tests/tactica_wiki_evolutions.cjs")], check=True, cwd=ROOT)

    def test_all_evolution_nodes_have_a_wiki_entry_and_moves(self):
        from sync_tactica_species_evolutions import ASSETS, PREFIX
        entries = {}
        for path in ASSETS:
            entries.update(json.loads(path.read_text().strip()[len(PREFIX):-2]))
        required = {s for s, routes in runtime_evolutions().items() if routes}
        required.update(e["target"] for routes in runtime_evolutions().values() for e in routes)
        self.assertFalse(required - entries.keys())
        text = (ROOT / "docs/assets/tactica-learnsets.js").read_text().strip()
        moves = json.loads(text[len("window.TacticaDexLearnsets="):-1])
        for species in required:
            self.assertIn(entries[species]["learnset"], moves, species)
        self.assertEqual(entries["SPECIES_ALCREMIE_STRAWBERRY_VANILLA_CREAM"]["stats"]["hp"], 65)


if __name__ == "__main__":
    unittest.main()
