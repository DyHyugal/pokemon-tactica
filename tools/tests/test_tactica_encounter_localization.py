"""Wiki encounter results must describe the Pokémon the ROM can actually create."""

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from sync_tactica_localization import (
    encounter_counts,
    possible_stages,
    update_pokedex_markdown,
)


class EncounterLocalizationTests(unittest.TestCase):
    def test_threshold_inside_a_range_and_player_item_decision(self):
        self.assertEqual(possible_stages("SPECIES_TYNAMO", 38), {"SPECIES_TYNAMO"})
        self.assertEqual(possible_stages("SPECIES_TYNAMO", 39), {"SPECIES_EELEKTRIK"})
        self.assertEqual(possible_stages("SPECIES_TYNAMO", 60), {"SPECIES_EELEKTRIK"})
        self.assertEqual(possible_stages("SPECIES_SEADRA", 60), {"SPECIES_SEADRA"})

    def test_conditional_branches_preserve_gender_pid_and_time(self):
        self.assertEqual(
            possible_stages("SPECIES_COMBEE", 37),
            {"SPECIES_COMBEE", "SPECIES_VESPIQUEN"},
        )
        self.assertEqual(
            possible_stages("SPECIES_WURMPLE", 10),
            {"SPECIES_BEAUTIFLY", "SPECIES_DUSTOX"},
        )
        self.assertEqual(
            possible_stages("SPECIES_AMAURA", 41, "Day"), {"SPECIES_AMAURA"}
        )
        self.assertEqual(
            possible_stages("SPECIES_AMAURA", 41, "Night"), {"SPECIES_AURORUS"}
        )

    def test_a_duplicate_slot_counts_as_one_table_for_each_actual_stage(self):
        table = {"species": ["SPECIES_TYNAMO"] * 4, "min_level": 37, "max_level": 40}
        display = {"en": {"SPECIES_TYNAMO": "Tynamo", "SPECIES_EELEKTRIK": "Eelektrik"}}
        counts = encounter_counts([table], "en", display, {}, {})
        self.assertEqual(dict(counts), {"Tynamo": 1, "Eelektrik": 1})

    def test_removed_encounters_do_not_leave_a_stale_nonzero_count(self):
        from collections import Counter

        text = "| Pokémon | Tables |\n| Bidoof | 7 |\n"
        self.assertIn("| Bidoof | 0 |", update_pokedex_markdown(text, Counter()))


if __name__ == "__main__":
    unittest.main()
