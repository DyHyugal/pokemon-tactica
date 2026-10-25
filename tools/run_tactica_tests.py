#!/usr/bin/env python3
"""Run reviewed native suites; zero tests, skipped tests and known failures fail CI."""
import argparse
import json
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SUITES = {
    "progression": ["test/level_caps.c", "Tactica ordinary trainers:"],
    "bosses": ["test/tactica_boss_legality.c", "test/battle/tactica_boss_mega.c", "test/battle/ai/gimmick_mega.c"],
    "ai": ["test/battle/ai/ai_tactica_setup.c", "test/battle/ai/ai_tactica_field_support.c"],
    "starters": ["test/family_starter.c", "test/family_safari.c"],
    "evolutions": ["test/evolution_requirements.c"],
    "training": ["test/training_npc.c"],
    "encounters": ["test/tactica_encounters.c"],
    "traversal": ["test/tactica_field_moves.c"],
    "shops": ["test/tactica_shops.c"],
    "ui": ["test/settings_menu.c", "test/tactica_config_menu.c", "test/tactica_battle_menu.c"],
    "messages": ["test/tactica_battle_messages.c"],
    "audio": ["test/audio_volume.c"],
    "speed": ["test/native_speed.c"],
    "balance": ["test/tactica_balance.c"],
    "fixtures": ["test/battle/trainer_control.c"],
}
ANSI = re.compile(r"\x1b\[[0-9;]*[A-Za-z]")


def check_summary(output, returncode):
    """Hydra can exit successfully after zero tests or failed ASSUME conditions."""
    output = ANSI.sub("", output)
    totals = re.findall(r"^- Tests TOTAL:\s+(\d+)\s*$", output, re.M)
    passes = re.findall(r"^- Tests PASSED:\s+(\d+)\s*$", output, re.M)
    if returncode or len(totals) != 1 or len(passes) != 1:
        raise ValueError("native command failed or lacks a unique complete summary")
    total, passed = int(totals[0]), int(passes[0])
    if total == 0 or total != passed:
        raise ValueError(f"not all selected tests passed: passed={passed}, total={total}")
    return total


def run(suites, jobs, output_dir, exhaustive=False):
    output_dir.mkdir(parents=True, exist_ok=True)
    filters = list(dict.fromkeys(f for s in suites for f in SUITES[s]))
    if exhaustive:
        filters.append("Tactica exhaustive trainer audit:")
    evidence = []
    for index, test_filter in enumerate(filters):
        # argv, not a shell command: the literal filename/prefix reaches make.
        command = ["make", "check", "BUILD=hns", f"TESTS={test_filter}", f"-j{jobs}"]
        print(f"Running {test_filter}", flush=True)
        started = time.monotonic()
        result = subprocess.run(command, cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        log = output_dir / f"{index:02d}.log"
        log.write_text(result.stdout)
        try:
            total = check_summary(result.stdout, result.returncode)
        except ValueError as exc:
            print(result.stdout[-16000:], file=sys.stderr)
            raise RuntimeError(f"{test_filter}: {exc}; full log: {log}") from exc
        evidence.append({"filter": test_filter, "passed": total, "seconds": round(time.monotonic()-started, 2), "log": str(log)})
        print(f"  {total} groups passed ({evidence[-1]['seconds']}s)", flush=True)
        (output_dir / "results.json").write_text(json.dumps(evidence, indent=2) + "\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--suites", nargs="+", choices=list(SUITES) + ["all"], default=["all"])
    parser.add_argument("--jobs", type=int, default=2)
    parser.add_argument("--output-dir", type=Path, default=Path("test-results"))
    parser.add_argument("--exhaustive", action="store_true")
    args = parser.parse_args()
    run(list(SUITES) if "all" in args.suites else args.suites, args.jobs, args.output_dir, args.exhaustive)
