"""Regression checks for actual false-green conditions and CI scope gaps."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from run_tactica_tests import check_summary, SUITES
from tactica_test_plan import plan


class NativeResultTests(unittest.TestCase):
    def test_accepts_complete_colored_summary(self):
        self.assertEqual(check_summary("- Tests \x1b[32mPASSED\x1b[0m: 2\n- Tests TOTAL: 2\n", 0), 2)

    def test_rejects_zero_skipped_known_fail_crash_and_missing_summary(self):
        for text, code in [
            ("No tests found.\n", 0),
            ("- Tests PASSED: 0\n- Tests TOTAL: 0\n", 0),
            ("- ASSUMPTIONS_FAILED: 1\n- Tests PASSED: 1\n- Tests TOTAL: 2\n", 0),
            ("- Tests KNOWN_FAILING: 1\n- Tests PASSED: 1\n- Tests TOTAL: 2\n", 0),
            ("- Tests PASSED: 2\n- Tests TOTAL: 2\n", 2),
            ("- Tests PASSED: 2\n", 0),
        ]:
            with self.subTest(text=text, code=code), self.assertRaises(ValueError):
                check_summary(text, code)

    def test_targeted_test_edit_keeps_its_domain(self):
        self.assertEqual(plan(["test/tactica_opening_trainer_levels.c"]), {"suites": ["progression"], "build": False, "data": True})

    def test_shared_game_changes_never_silently_skip_other_domains(self):
        for path in ["src/battle_main.c", "include/data.h", "data/spec/trainer_levels.json", "graphics/new.png", "Makefile"]:
            with self.subTest(path=path):
                result = plan([path])
                self.assertEqual(result["suites"], list(SUITES))
                self.assertTrue(result["build"])

    def test_docs_and_publisher_do_not_rebuild_the_rom(self):
        self.assertEqual(plan(["docs/spec/PLAYTEST_STATUS.md"]), {"suites": [], "build": False, "data": False})
        self.assertEqual(plan([".github/workflows/candidate-release.yml"]), {"suites": [], "build": False, "data": True})

    def test_manual_full_overrides_scope(self):
        self.assertEqual(plan(["README.md"], full=True)["suites"], list(SUITES))


if __name__ == "__main__":
    unittest.main()
