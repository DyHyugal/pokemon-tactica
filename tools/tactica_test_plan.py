#!/usr/bin/env python3
"""Conservative CI scope: isolated tests are targeted; unknown game changes run all."""
import argparse
import json
import subprocess
from pathlib import Path

from run_tactica_tests import SUITES


def plan(paths, full=False):
    selected = set()
    build = full
    check_data = full
    for path in paths:
        if path.startswith(("docs/", "wiki/")) or path in ("README.md", "AGENTS.md"):
            continue
        # The publisher does not change gameplay; retain data validation.
        if path == ".github/workflows/candidate-release.yml":
            check_data = True
            continue
        check_data = True
        matches = {suite for suite, filters in SUITES.items() if path in filters}
        if path in ("test/tactica_opening_trainer_levels.c", "test/tactica_trainer_levels.c", "test/tactica_trainer_scaling_cases.h"):
            matches = {"progression"}
        if path == "test/battle/tactica_boss_mega_cases.h":
            matches = {"bosses"}
        if matches:
            selected.update(matches)
        elif path.startswith("tools/"):
            # Generators, audits and runner changes can invalidate multiple domains.
            selected.update(SUITES)
        elif path.startswith("test/"):
            selected.update(SUITES)  # Shared runner or unclassified upstream tests.
        else:
            selected.update(SUITES)  # Runtime, specs, assets, configuration or build system.
            build = True
    if full:
        selected.update(SUITES)
    return {"suites": [s for s in SUITES if s in selected], "build": build, "data": check_data}


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base")
    parser.add_argument("--full", action="store_true")
    parser.add_argument("--github-output", type=Path)
    args = parser.parse_args()
    paths = subprocess.check_output(["git", "diff", "--name-only", f"{args.base}...HEAD"], text=True).splitlines() if args.base else []
    result = plan(paths, full=args.full or not args.base)
    print(json.dumps({"changed": paths, **result}, indent=2))
    if args.github_output:
        with args.github_output.open("a") as stream:
            for key in ("build", "data"):
                stream.write(f"{key}={str(result[key]).lower()}\n")
            stream.write("suites=" + " ".join(result["suites"]) + "\n")
