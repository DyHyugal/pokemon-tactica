#!/usr/bin/env python3
"""Synchronize localized encounter level ranges with the canonical specs."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
STANDARD_SPEC = ROOT / "data/spec/encounters_standard.json"
MARKDOWN_FILES = (
    ROOT / "wiki/FR/Localisations.md",
    ROOT / "wiki/EN/Locations.md",
    ROOT / "docs/FR/Localisations.md",
    ROOT / "docs/EN/Locations.md",
)
HTML_FILES = (
    ROOT / "docs/FR/Localisations.html",
    ROOT / "docs/EN/Locations.html",
)
HTML_STANDARD_ROW = re.compile(r'<tr data-kind="standard".*?</tr>')
HTML_CELL = re.compile(r"<td>.*?</td>")


def load_ranges() -> list[str]:
    data = json.loads(STANDARD_SPEC.read_text(encoding="utf-8"))
    tables = data["tables"]
    if len(tables) != data["standard_table_count"]:
        raise ValueError("encounters_standard.json has an inconsistent table count")
    return [format_range(table["min_level"], table["max_level"]) for table in tables]


def format_range(min_level: int, max_level: int) -> str:
    return str(min_level) if min_level == max_level else f"{min_level}–{max_level}"


def update_markdown(text: str, ranges: list[str], path: Path) -> str:
    newline = "\r\n" if "\r\n" in text else "\n"
    lines = text.split(newline)
    data_rows: list[int] = []
    for index, line in enumerate(lines):
        if not line.startswith("| ") or line.startswith("|---"):
            continue
        cells = line.split("|")
        if len(cells) == 7 and cells[1].strip() not in {"Zone", "Area"}:
            data_rows.append(index)

    if len(data_rows) != 462:
        raise ValueError(f"{path.relative_to(ROOT)}: expected 462 encounter rows, got {len(data_rows)}")

    for index, level_range in zip(data_rows[: len(ranges)], ranges):
        cells = lines[index].split("|")
        if cells[4].strip() != level_range:
            cells[4] = f" {level_range} "
            lines[index] = "|".join(cells)
    return newline.join(lines)


def update_html(text: str, ranges: list[str], path: Path) -> str:
    rows = list(HTML_STANDARD_ROW.finditer(text))
    if len(rows) != len(ranges):
        raise ValueError(
            f"{path.relative_to(ROOT)}: expected {len(ranges)} standard rows, got {len(rows)}"
        )

    replacements: list[str] = []
    for row_match, level_range in zip(rows, ranges):
        row = row_match.group(0)
        cells = list(HTML_CELL.finditer(row))
        if len(cells) != 5:
            raise ValueError(f"{path.relative_to(ROOT)}: malformed encounter row")
        level_cell = cells[3]
        replacements.append(
            row[: level_cell.start()] + f"<td>{level_range}</td>" + row[level_cell.end() :]
        )

    parts: list[str] = []
    cursor = 0
    for row_match, replacement in zip(rows, replacements):
        parts.append(text[cursor : row_match.start()])
        parts.append(replacement)
        cursor = row_match.end()
    parts.append(text[cursor:])
    return "".join(parts)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail instead of updating stale files")
    args = parser.parse_args()

    ranges = load_ranges()
    stale: list[Path] = []
    for path in MARKDOWN_FILES + HTML_FILES:
        current = path.read_bytes().decode("utf-8")
        expected = (
            update_markdown(current, ranges, path)
            if path.suffix == ".md"
            else update_html(current, ranges, path)
        )
        if expected == current:
            continue
        stale.append(path)
        if not args.check:
            path.write_bytes(expected.encode("utf-8"))

    if stale and args.check:
        for path in stale:
            print(f"out of date: {path.relative_to(ROOT)}", file=sys.stderr)
        return 1

    action = "checked" if args.check else "updated"
    print(f"Tactica localization: {action} {len(MARKDOWN_FILES) + len(HTML_FILES)} files")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
