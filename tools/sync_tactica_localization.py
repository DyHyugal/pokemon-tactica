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
from functools import lru_cache
from pathlib import Path
from sync_tactica_special_encounters import species_constant
from sync_tactica_species_evolutions import runtime_evolutions
from validate_tactica_encounter_evolutions import legal_stage, level_evolutions
level_evolutions = lru_cache(maxsize=1)(level_evolutions)


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

HTML_STANDARD_ROW = re.compile(r'<tr data-kind="(?:standard|headbutt|safari)".*?</tr>', re.S)
HTML_CELL = re.compile(r"<td>.*?</td>", re.S)
DEX_CARD = re.compile(r'<article class="card dex-card".*?</article>', re.S)
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


def pokedex_cards(path: Path) -> dict[str, str]:
    text = path.read_text(encoding="utf-8")
    cards: dict[str, str] = {}
    for match in DEX_CARD.finditer(text):
        card = match.group(0)
        sprite = DEX_CARD_SPRITE.search(card)
        name = DEX_CARD_NAME.search(card)
        if sprite and name:
            cards[html.unescape(sprite.group(1))] = html.unescape(name.group(1))
    return cards


def runtime_slug(value: str) -> str:
    return re.sub(r"[^a-z0-9]+", "-", (value or "").lower()).strip("-")


def slug_candidates(constant: str, species_meta: dict[str, dict]) -> list[str]:
    base = constant.removeprefix("SPECIES_").lower().replace("_", "-")
    runtime = runtime_slug(species_meta.get(constant, {}).get("name", ""))
    candidates = [base, runtime]
    if base.endswith("-m"):
        candidates.extend((base[:-2] + "-male", base[:-2]))
    if base.endswith("-f"):
        candidates.extend((base[:-2] + "-female", base[:-2]))
    if base.endswith("-galar-standard"):
        candidates.append(base.removesuffix("-standard"))
    for suffix in (
        "-red", "-shield", "-zero", "-overcast", "-midday", "-amped",
        "-curly", "-droopy", "-stretchy", "-meadow", "-average",
        "-small", "-large", "-super", "-meteor", "-red-striped",
        "-west", "-east",
    ):
        if base.endswith(suffix):
            candidates.append(base.removesuffix(suffix))
    return list(dict.fromkeys(candidate for candidate in candidates if candidate))


def build_display_map(lang: str, species_meta: dict[str, dict]) -> dict[str, str]:
    path = ROOT / ("docs/FR/Pokedex.html" if lang == "fr" else "docs/EN/Pokedex.html")
    cards = pokedex_cards(path)
    result: dict[str, str] = {}
    for constant in species_meta:
        for slug in slug_candidates(constant, species_meta):
            if slug in cards:
                result[constant] = cards[slug]
                break
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


@lru_cache(maxsize=1)
def conditional_level_routes():
    return runtime_evolutions()


def possible_stages(constant: str, level: int, time: str = "Any", depth: int = 3) -> set[str]:
    """Possible wild forms, preserving official conditions and player items."""
    constant = legal_stage(constant, level, level_evolutions())
    if depth == 0:
        return {constant}
    candidates = []
    definite = False
    genders = set()
    pids = set()
    for route in conditional_level_routes().get(constant, []):
        if route["method"] != "EVO_LEVEL" or not route["param"].isdigit():
            continue
        if not 0 < int(route["param"]) <= level:
            continue
        conditions = route["conditions"]
        if any(condition in {"IF_HOLD_ITEM", "IF_BAG_ITEM_COUNT"} for condition, _ in conditions):
            continue
        possible = True
        uncertain = []
        for condition, value in conditions:
            if condition == "IF_REGION" and value in {"REGION_HISUI", "REGION_ALOLA"}:
                possible = False
            elif condition == "IF_NOT_REGION" and value in {"REGION_HISUI", "REGION_ALOLA"}:
                continue
            elif condition in {"IF_TIME", "IF_NOT_TIME"} and value == "TIME_NIGHT" and time != "Any":
                matches = time == "Night"
                if matches != (condition == "IF_TIME"):
                    possible = False
            else:
                uncertain.append((condition, value))
        if not possible:
            continue
        candidates.append(route["target"])
        definite |= not uncertain
        if len(uncertain) == 1:
            condition, value = uncertain[0]
            if condition == "IF_GENDER":
                genders.add(value)
            elif condition == "IF_PID_UPPER_MODULO_10_GT":
                pids.update(range(int(value) + 1, 10))
            elif condition == "IF_PID_UPPER_MODULO_10_LT":
                pids.update(range(int(value)))
    definite |= genders >= {"MON_MALE", "MON_FEMALE"} or len(pids) == 10
    result = set() if definite else {constant}
    for target in candidates:
        result.update(possible_stages(target, level, time, depth - 1))
    return result


def species_text(
    table: dict,
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> str:
    slots = []
    for constant, rate in zip(table["species"], table["rates"]):
        stages = collections.OrderedDict()
        for level in range(table["min_level"], table["max_level"] + 1):
            for stage in sorted(possible_stages(constant, level, table.get("time", "Any"))):
                stages.setdefault(stage, []).append(level)
        labels = []
        for stage, levels in stages.items():
            label = display_label(stage, lang, display_maps, species_meta, fr_species)
            if len(stages) > 1:
                label += " " + format_range(min(levels), max(levels))
            labels.append(label)
        slots.append(" / ".join(labels) + f" {rate}%")
    return " · ".join(slots)


def update_markdown(
    text: str,
    standard: list[dict],
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> str:
    newline, lines, data_rows = markdown_data_rows(text)
    for table, index in zip(standard, data_rows[: len(standard)]):
        cells = lines[index].split("|")
        cells[4] = f" {format_range(table['min_level'], table['max_level'])} "
        cells[5] = f" {species_text(table, lang, display_maps, species_meta, fr_species)} "
        lines[index] = "|".join(cells)
    return newline.join(lines)


def html_cell_text(cell: str) -> str:
    return html.unescape(re.sub(r"<[^>]+>", "", cell))


def search_aliases() -> list[tuple[str, str, str, str, str, str]]:
    pages = {}
    for lang, path in (
        ("fr", ROOT / "docs/FR/Localisations.html"),
        ("en", ROOT / "docs/EN/Locations.html"),
    ):
        rows = list(HTML_STANDARD_ROW.finditer(path.read_text(encoding="utf-8")))
        if len(rows) != 462:
            raise ValueError(f"{path.relative_to(ROOT)}: expected 462 encounter rows")
        pages[lang] = [
            [html_cell_text(cell.group(0)) for cell in HTML_CELL.finditer(row.group(0))][:3]
            for row in rows
        ]
    return [
        (
            pages["fr"][index][0], pages["en"][index][0],
            pages["fr"][index][1], pages["en"][index][1],
            pages["fr"][index][2], pages["en"][index][2],
        )
        for index in range(462)
    ]


def update_html(
    text: str,
    standard: list[dict],
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
    aliases: list[tuple[str, str, str, str, str, str]],
    path: Path,
) -> str:
    rows = list(HTML_STANDARD_ROW.finditer(text))
    if len(rows) != len(standard):
        raise ValueError(
            f"{path.relative_to(ROOT)}: expected {len(standard)} standard rows, got {len(rows)}"
        )
    replacements: list[str] = []
    for index, (row_match, table) in enumerate(zip(rows, standard)):
        replacement = row_match.group(0)
        for cell_index, value in (
            (4, html.escape(species_text(table, lang, display_maps, species_meta, fr_species), quote=False)),
            (3, format_range(table["min_level"], table["max_level"])),
        ):
            cells = list(HTML_CELL.finditer(replacement))
            if len(cells) != 5:
                raise ValueError(f"{path.relative_to(ROOT)}: malformed encounter row")
            cell = cells[cell_index]
            replacement = replacement[: cell.start()] + f"<td>{value}</td>" + replacement[cell.end() :]

        search_terms = list(aliases[index])
        for constant in table["species"]:
            search_terms.extend((
                display_label(constant, "fr", display_maps, species_meta, fr_species),
                display_label(constant, "en", display_maps, species_meta, fr_species),
            ))
        search_value = html.escape(" ".join(search_terms).lower(), quote=True)
        replacement = re.sub(
            r'data-search="[^"]*"',
            f'data-search="{search_value}"',
            replacement,
            count=1,
        )
        replacements.append(replacement)

    parts: list[str] = []
    cursor = 0
    for row_match, replacement in zip(rows, replacements):
        parts.append(text[cursor : row_match.start()])
        parts.append(replacement)
        cursor = row_match.end()
    parts.append(text[cursor:])
    return "".join(parts)


def encounter_counts(
    standard: list[dict],
    lang: str,
    display_maps: dict[str, dict[str, str]],
    species_meta: dict[str, dict],
    fr_species: dict[str, str],
) -> collections.Counter[str]:
    counts = collections.Counter()
    for table in standard:
        # Count a table once per actual species, including ranges that cross
        # a level evolution. Do not double-count duplicate slots.
        species = {
            stage
            for constant in table["species"]
            for level in range(table["min_level"], table["max_level"] + 1)
            for stage in possible_stages(constant, level, table.get("time", "Any"))
        }
        for constant in species:
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
        lines[index] = f"| {name} | {counts[name]} |"
    return newline.join(lines)


def update_pokedex_html(text: str, counts: collections.Counter[str]) -> str:
    parts: list[str] = []
    cursor = 0
    for match in DEX_CARD.finditer(text):
        card = match.group(0)
        name_match = DEX_CARD_NAME.search(card)
        if not name_match:
            continue
        name = html.unescape(name_match.group(1))
        replacement = DEX_CARD_COUNT.sub(
            f"<small>{counts[name]} {'table' if counts[name] == 1 else 'tables'}</small>",
            card,
            count=1,
        )
        parts.append(text[cursor : match.start()])
        parts.append(replacement)
        cursor = match.end()
    parts.append(text[cursor:])
    return "".join(parts)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="fail instead of updating stale files")
    args = parser.parse_args()

    standard = load_standard() + load_special()
    standard = [dict(table, species=[species_constant(s) for s in table["species"]]) for table in standard]
    species_meta = load_species_metadata()
    fr_species = load_fr_species()
    display_maps = {
        "fr": build_display_map("fr", species_meta),
        "en": build_display_map("en", species_meta),
    }
    aliases = search_aliases()

    stale: list[Path] = []
    for path, lang in MARKDOWN_FILES:
        current = path.read_text(encoding="utf-8")
        expected = update_markdown(current, standard, lang, display_maps, species_meta, fr_species)
        if expected != current:
            stale.append(path)
            if not args.check:
                path.write_text(expected, encoding="utf-8")

    for path, lang in HTML_FILES:
        current = path.read_text(encoding="utf-8")
        expected = update_html(
            current, standard, lang, display_maps, species_meta, fr_species, aliases, path
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
