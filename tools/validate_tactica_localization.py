#!/usr/bin/env python3
"""Validate player-facing Tactica identity and localized wiki structure."""

from __future__ import annotations

import re
import struct
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ERRORS: list[str] = []


def require(condition: bool, message: str) -> None:
    if not condition:
        ERRORS.append(message)


def read(relative: str) -> str:
    return (ROOT / relative).read_text(encoding="utf-8")


def validate_identity() -> None:
    makefile = read("Makefile")
    oak = read("src/oak_speech_hns.c")
    help_window = read("src/data/help_window.h")

    require(re.search(r"^\s*TITLE\s*:=\s*PKMN TACTICA\s*$", makefile, re.MULTILINE) is not None, "HnS ROM title is not PKMN TACTICA")
    require("POKéMON TACTICA defaults to HARD" in oak, "Oak intro does not identify Pokémon Tactica")
    require("You are both playing POKéMON TACTICA." in help_window, "multiplayer help uses the wrong game name")

    for relative, text in (
        ("src/oak_speech_hns.c", oak),
        ("src/data/help_window.h", help_window),
    ):
        for legacy in ("FAMILY REMIX", "HEART & SOUL"):
            require(legacy not in text, f"{relative} still exposes {legacy} to the player")


def validate_title_asset() -> None:
    path = ROOT / "graphics/title_screen/hns/emerald_version.png"
    data = path.read_bytes()
    require(data.startswith(b"\x89PNG\r\n\x1a\n"), "title subtitle is not a PNG")
    if len(data) >= 29 and data[12:16] == b"IHDR":
        width, height, bit_depth, color_type = struct.unpack(">IIBB", data[16:26])
        require((width, height) == (64, 64), "title subtitle must remain 64x64")
        require(bit_depth in {1, 2, 4, 8} and color_type == 3, "title subtitle must remain an indexed PNG")
    else:
        require(False, "title subtitle has no valid IHDR chunk")


def validate_wiki() -> None:
    languages = {
        "FR": ("Accueil", "Guide-de-jeu", "Changements", "Pokedex", "Localisations", "Boss-et-Conseils", "Credits-et-Versions", "Roadmap"),
        "EN": ("Home", "Game-Guide", "Changes", "Pokedex", "Locations", "Bosses-and-Tips", "Credits-and-Versions", "Roadmap"),
    }
    link_pattern = re.compile(r"\[[^]]+\]\(([^)#]+)(?:#[^)]+)?\)")

    for language, names in languages.items():
        for name in names:
            for base in (ROOT / "wiki" / language, ROOT / "docs" / language):
                path = base / f"{name}.md"
                require(path.is_file(), f"missing localized page: {path.relative_to(ROOT)}")
                if not path.is_file():
                    continue
                text = path.read_text(encoding="utf-8")
                for target in link_pattern.findall(text):
                    if "://" in target or target.startswith("mailto:"):
                        continue
                    require((path.parent / target).resolve().is_file(), f"broken link in {path.relative_to(ROOT)}: {target}")

        locations = ROOT / "wiki" / language / ("Localisations.md" if language == "FR" else "Locations.md")
        rows = [line for line in locations.read_text(encoding="utf-8").splitlines() if line.startswith("| ")]
        require(len(rows) == 463, f"{locations.relative_to(ROOT)} must contain one header and 462 encounter rows")



def validate_fr_player_copy() -> None:
    fr_locations = read("docs/FR/Localisations.md")
    fr_guide = read("docs/FR/Guide-de-jeu.md")
    fr_changes = read("docs/FR/Changements.md")
    dex_detail = read("docs/assets/pokedex-detail.js")
    localization = read("docs/assets/tactica-localization.js")

    english_zone_markers = (
        "Blackthorn City", "Burned Tower", "Celadon City", "Cerulean Cave",
        "Cerulean City", "Cherrygrove City", "Cianwood City", "Cinnabar Island",
        "Dark Cave", "Digletts Cave", "Dragons Den", "Ecruteak City",
        "Fuchsia City", "Ice Path", "Ilex Forest", "Lake Of Rage", "Mt Moon",
        "Mt Mortar", "Mt Silver", "National Park", "New Bark Town",
        "Olivine City", "Pallet Town", "Rock Tunnel", "Ruins Of Alph",
        "Seafoam Islands", "Slowpoke Well", "Sprout Tower", "Tin Tower",
        "Union Cave", "Vermilion City", "Victory Road", "Violet City",
        "Viridian City", "Viridian Forest", "Whirl Islands",
    )
    for marker in english_zone_markers:
        require(marker not in fr_locations, f"FR Localisations still exposes English zone name: {marker}")

    require(
        "L'assistant d'Orme ne déclenche pas le choix du second starter" in fr_guide,
        "FR guide still documents Elm's aide as the second-starter trigger",
    )
    for heading in ("**Feu**", "**Eau**", "**Plante**", "**Électrik**", "**Sol**", "**Glace**", "**Évoli**"):
        require(heading in fr_guide, f"FR guide starter grouping missing: {heading}")

    for npc in ("PNJ Objets", "PNJ Capacités / CT", "PNJ Méga-Gemmes"):
        require(npc in fr_changes, f"FR Changes missing specialist shop description: {npc}")

    require(
        '''const description=lang==="fr"?"":''' in dex_detail,
        "FR Pokédex can leak English ability descriptions",
    )
    require(
        '''"dragon's maw":"Dent de Dragon"''' in localization,
        "FR ability localization missing Dragon's Maw",
    )
    require(
        '''"mind's eye":"Œil Mental"''' in localization,
        "FR ability localization missing Mind's Eye",
    )

def main() -> int:
    validate_identity()
    validate_title_asset()
    validate_wiki()
    validate_fr_player_copy()
    if ERRORS:
        for error in ERRORS:
            print(f"ERROR: {error}", file=sys.stderr)
        return 1
    print("Tactica localization: identity, FR/EN wiki structure and player-facing FR copy validated")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
