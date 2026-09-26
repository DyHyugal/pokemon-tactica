#!/usr/bin/env python3
"""Synchronize localized encounter data and Pokédex counts with canonical specs."""

from __future__ import annotations

import argparse
import collections
import html
import json
import re
import sys
import unicodedata
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
STANDARD_SPEC = ROOT / "data/spec/encounters_standard.json"
SPECIAL_SPEC = ROOT / "data/spec/encounters_special.json"
SPECIES_ASSETS = tuple(ROOT / f"docs/assets/tactica-species-{index}.js" for index in (1, 2, 3))
LOCALIZATION_ASSET = ROOT / "docs/assets/tactica-localization.js"

MARKDOWN_FILES = (
    (ROOT / "wiki/FR/Localisations.md", "fr"),
    (ROOT / "wiki/EN/Locations.md", "en"),
    (ROOT / "docs/FR/Localisations.md", "fr"),
    (ROOT / "docs/EN/Locations.md", "en"),
)
HTML_FILES = (
    (ROOT / "docs/FR/Localisations.html", "fr"),
    (ROOT / "docs/EN/Locations.html", "en"),
)
POKEDEX_MARKDOWN_FILES = (
    (ROOT / "wiki/FR/Pokedex.md", "fr"),
    (ROOT / "wiki/EN/Pokedex.md", "en"),
    (ROOT / "docs/FR/Pokedex.md", "fr"),
    (ROOT / "docs/EN/Pokedex.md", "en"),
)
POKEDEX_HTML_FILES = (
    (ROOT / "docs/FR/Pokedex.html", "fr"),
    (ROOT / "docs/EN/Pokedex.html", "en"),
)

HTML_STANDARD_ROW = re.compile(r'<tr data-kind="standard".*?</tr>')
HTML_CELL = re.compile(r"<td>.*?</td>")
DEX_CARD = re.compile(r'<article class="card dex-card".*?</article>')
DEX_CARD_NAME = re.compile(r"<strong>([^<]+)</strong>")
DEX_CARD_SPRITE = re.compile(r'/([^/"?]+)\.png(?:\?[^"]*)?"')
DEX_CARD_COUNT = re.compile(r"<small>\d+ tables?</small>")
POKEDEX_MD_ROW = re.compile(r"^\| (.+?) \| (\d+) \|$")

FORM_FALLBACKS = {
    "en": {
        "SPECIES_NINETALES_ALOLA": "Alolan Ninetales",
        "SPECIES_DARMANITAN_GALAR_STANDARD": "Galarian Darmanitan",
    },
    "fr": {
        "SPECIES_NINETALES_ALOLA": "Feunard d’Alola",
        "SPECIES_DARMANITAN_GALAR_STANDARD": "Darumacho de Galar",
    },
}


def normalize(value: str) -> str:
    value = unicodedata.normalize("NFD", value or "")
    value = "".join(char for char in value if not unicodedata.combining(char))
    return re.sub(r"[^a-z0-9♀♂]+", "", value.lower())


def title_name(value: str) -> str:
    return " ".join(part[:1].upper() + part[1:].lower() for part in value.replace("_", " ").split())


def load_standard() -> list[dict]:
    data = json.loads(STANDARD_SPEC.read_text(encoding="utf-8"))
    tables = data["tables"]
    if len(tables) != data["standard_table_count"] or len(tables) != 405:
        raise ValueError("encounters_standard.json has an inconsistent table count")
    return tables


def load_special() -> list[dict]:
    data = json.loads(SPECIAL_SPEC.read_text(encoding="utf-8"))
    tables = data["tables"]
    if len(tables) != 57:
        raise ValueError("encounters_special.json must contain 57 tables")
    return tables


def load_species_metadata() -> dict[str, dict]:
    result: dict[str, dict] = {}
    marker = "}, {"
    for path in SPECIES_ASSETS:
        text = path.read_text(encoding="utf-8")
        start = text.find(marker)
        end = text.rfind(");")
        if start < 0 or end < 0:
            raise ValueError(f"Cannot parse {path.relative_to(ROOT)}")
        result.update(json.loads(text[start + 3 : end]))
    return result


def load_fr_species() -> dict[str, str]:
    text = LOCALIZATION_ASSET.read_text(encoding="utf-8").strip()
    prefix = "window.TacticaLocalization="
    if not text.startswith(prefix):
        raise ValueError("Unexpected tactica-localization.js format")
    payload = text[len(prefix) :]
    if payload.endswith(";"):
        payload = payload[:-1]
    return json.loads(payload).get("species", {})


def pokedex_cards(path: Path) -> list[tuple[str, str]]:
    text = path.read_text(encoding="utf-8")
    cards: list[tuple[str, str]] = []
    for match in DEX_CARD.finditer(text):
        card = match.group(0)
        sprite = DEX_CARD_SPRITE.search(card)
        name = DEX_CARD_NAME.search(card)
        if sprite and name:
            cards.append((html.unescape(sprite.group(1)), html.unescape(name.group(1))))
    return cards


def build_display_map(lang: str, species_meta: dict[str, dict]) -> dict[str, str]:
    path = ROOT / ("docs/FR/Pokedex.html" if lang == "fr" else "docs/EN/Pokedex.html")
    result: dict[str, str] = {}
    for slug, name in pokedex_cards(path):
        slug_n = normalize(slug)
        name_n = normalize(name)
        best = None
        best_score = -1
        for constant, data in species_meta.items():
            constant_n = normalize(constant.removeprefix("SPECIES_"))
            runtime_n = normalize(data.get("name", ""))
            score = 0
            if slug_n and constant_n == slug_n:
                score += 1000
            if name_n and runtime_n == name_n:
                score += 200
            if slug_n and runtime_n and slug_n.startswith(runtime_n):
                score += 80
            if constant_n == name_n:
                score += 100
            for token in constant.removeprefix("SPECIES_").lower().split("_")[1:]:
                if len(token) > 1 and normalize(token) in slug_n:
                    score += 4
            if score > best_score:
                best = constant
                best_score = score
        if best:
            result[best] = name
    return result


def markdown_data_rows(text: str) -> tuple[str, list[str], list[int]]:
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
        raise ValueError(f"expected 462 encounter rows, got {len(data_rows)}")
    return newline, lines, data_rows


def seed_display_map_from_locations(
    lang: str,
    display_map: dict[str, str],
    species_meta: dict[str, dict],
    standard: list[dict],
) -> None:
    path = ROOT / ("wiki/FR/Localisations.md" if lang == "fr" else "wiki/EN/Locations.md")
    _, lines, rows = markdown_data_rows(path.read_text(encoding="utf-8"))
    for table, row_index in zip(standard, rows[: len(standard)]):
        cells = lines[row_index].split("|")
        names = [part.strip().rsplit(" ", 1)[0] for part in cells[5].split("·")]
        for constant, current_name in zip(table["species"], names):
            if constant in display_map:
                continue
            runtime = species_meta.get(constant, {}).get("name", "")
            runtime_base = normalize(runtime.split("-")[0])
            current_n = normalize(current_name)
            if runtime_base and (current_n.startswith(runtime_base) or runtime_base.startswith(current_n)):
                display_map[constant] = current_name


def fallback_label(
    constant: str,
    lang: str,
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> str:
    forced = FORM_FALLBACKS.get(lang, {}).get(constant)
    if forced:
        return forced
    runtime = species_meta.get(constant, {}).get("name")
    if not runtime:
        return title_name(constant.removeprefix("SPECIES_"))
    en = title_name(runtime.replace("-", " "))
    if lang == "en":
        return en
    return fr_species.get(en.lower(), en)


def display_label(
    constant: str,
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> str:
    return display_maps[lang].get(constant) or fallback_label(constant, lang, species_meta, fr_species)


def format_range(min_level: int, max_level: int) -> str:
    return str(min_level) if min_level == max_level else f"{min_level}–{max_level}"


def species_text(
    table: dict,
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> str:
    return " · ".join(
        f"{display_label(species, lang, display_maps, species_meta, fr_species)} {rate}%"
        for species, rate in zip(table["species"], table["rates"])
    )


def update_markdown(
    text: str,
    standard: list[dict],
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
    path: Path,
) -> str:
    newline, lines, data_rows = markdown_data_rows(text)
    for table, index in zip(standard, data_rows[: len(standard)]):
        cells = lines[index].split("|")
        level_range = format_range(table["min_level"], table["max_level"])
        slots = species_text(table, lang, display_maps, species_meta, fr_species)
        changed = False
        if cells[4].strip() != level_range:
            cells[4] = f" {level_range} "
            changed = True
        if cells[5].strip() != slots:
            cells[5] = f" {slots} "
            changed = True
        if changed:
            lines[index] = "|".join(cells)
    return newline.join(lines)


def update_html(
    text: str,
    standard: list[dict],
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
    path: Path,
) -> str:
    rows = list(HTML_STANDARD_ROW.finditer(text))
    if len(rows) != len(standard):
        raise ValueError(
            f"{path.relative_to(ROOT)}: expected {len(standard)} standard rows, got {len(rows)}"
        )
    replacements: list[str] = []
    for row_match, table in zip(rows, standard):
        row = row_match.group(0)
        cells = list(HTML_CELL.finditer(row))
        if len(cells) != 5:
            raise ValueError(f"{path.relative_to(ROOT)}: malformed encounter row")
        level_range = format_range(table["min_level"], table["max_level"])
        slots = html.escape(
            species_text(table, lang, display_maps, species_meta, fr_species),
            quote=False,
        )
        replacement = row
        for cell_index, value in ((4, slots), (3, level_range)):
            cells_now = list(HTML_CELL.finditer(replacement))
            cell = cells_now[cell_index]
            replacement = replacement[: cell.start()] + f"<td>{value}</td>" + replacement[cell.end() :]
        replacements.append(replacement)

    parts: list[str] = []
    cursor = 0
    for row_match, replacement in zip(rows, replacements):
        parts.append(text[cursor : row_match.start()])
        parts.append(replacement)
        cursor = row_match.end()
    parts.append(text[cursor:])
    return "".join(parts)


def special_display_counts(lang: str) -> collections.Counter[str]:
    path = ROOT / ("wiki/FR/Localisations.md" if lang == "fr" else "wiki/EN/Locations.md")
    _, lines, rows = markdown_data_rows(path.read_text(encoding="utf-8"))
    counts: collections.Counter[str] = collections.Counter()
    for row_index in rows[405:]:
        cells = lines[row_index].split("|")
        for part in cells[5].split("·"):
            name = part.strip().rsplit(" ", 1)[0]
            counts[name] += 1
    return counts


def encounter_counts(
    standard: list[dict],
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> collections.Counter[str]:
    counts: collections.Counter[str] = special_display_counts(lang)
    for table in standard:
        for constant in table["species"]:
            counts[display_label(constant, lang, display_maps, species_meta, fr_species)] += 1
    return counts


def update_pokedex_markdown(text: str, counts: collections.Counter[str]) -> str:
    newline = "\r\n" if "\r\n" in text else "\n"
    lines = text.split(newline)
    for index, line in enumerate(lines):
        match = POKEDEX_MD_ROW.match(line)
        if not match or match.group(1) == "Pokémon":
            continue
        name = match.group(1)
        if name in counts:
            lines[index] = f"| {name} | {counts[name]} |"
    return newline.join(lines)


def update_pokedex_html(text: str, counts: collections.Counter[str]) -> str:
    pieces: list[str] = []
    cursor = 0
    for match in DEX_CARD.finditer(text):
        card = match.group(0)
        name_match = DEX_CARD_NAME.search(card)
        if not name_match:
            continue
        name = html.unescape(name_match.group(1))
        if name not in counts:
            continue
        replacement = DEX_CARD_COUNT.sub(
            f"<small>{counts[name]} {'table' if counts[name] == 1 else 'tables'}</small>",
            card,
            count=1,
        )
        pieces.append(text[cursor : match.start()])
        pieces.append(replacement)
        cursor = match.end()
    pieces.append(text[cursor:])
    return "".join(pieces)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail instead of updating stale files")
    args = parser.parse_args()

    standard = load_standard()
    load_special()  # structural guard; special rows remain authored textual pools
    species_meta = load_species_metadata()
    fr_species = load_fr_species()
    display_maps = {
        "fr": build_display_map("fr", species_meta),
        "en": build_display_map("en", species_meta),
    }
    for lang in ("fr", "en"):
        seed_display_map_from_locations(lang, display_maps[lang], species_meta, standard)

    stale: list[Path] = []
    for path, lang in MARKDOWN_FILES:
        current = path.read_text(encoding="utf-8")
        expected = update_markdown(
            current, standard, lang, display_maps, species_meta, fr_species, path
        )
        if expected != current:
            stale.append(path)
            if not args.check:
                path.write_text(expected, encoding="utf-8")

    for path, lang in HTML_FILES:
        current = path.read_text(encoding="utf-8")
        expected = update_html(
            current, standard, lang, display_maps, species_meta, fr_species, path
        )
        if expected != current:
            stale.append(path)
            if not args.check:
                path.write_text(expected, encoding="utf-8")

    counts_by_lang = {
        lang: encounter_counts(standard, lang, display_maps, species_meta, fr_species)
        for lang in ("fr", "en")
    }
    for path, lang in POKEDEX_MARKDOWN_FILES:
        current = path.read_text(encoding="utf-8")
        expected = update_pokedex_markdown(current, counts_by_lang[lang])
        if expected != current:
            stale.append(path)
            if not args.check:
                path.write_text(expected, encoding="utf-8")

    for path, lang in POKEDEX_HTML_FILES:
        current = path.read_text(encoding="utf-8")
        expected = update_pokedex_html(current, counts_by_lang[lang])
        if expected != current:
            stale.append(path)
            if not args.check:
                path.write_text(expected, encoding="utf-8")

    if stale and args.check:
        for path in stale:
            print(f"out of date: {path.relative_to(ROOT)}", file=sys.stderr)
        return 1

    action = "checked" if args.check else "updated"
    print(
        f"Tactica localization: {action} "
        f"{len(MARKDOWN_FILES) + len(HTML_FILES)} location files and "
        f"{len(POKEDEX_MARKDOWN_FILES) + len(POKEDEX_HTML_FILES)} Pokédex files"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
