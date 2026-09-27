#!/usr/bin/env python3
"""Generate the FR/EN boss reference from canonical boss, Rocket and rival specs."""

from __future__ import annotations

import argparse
import html
import json
import re
import unicodedata
from collections import OrderedDict
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BOSSES = ROOT / "data/spec/bosses.json"
RIVAL = ROOT / "data/spec/rival.json"
LOCALIZATION = ROOT / "docs/assets/tactica-localization.js"

PAGES = {
    "fr": (ROOT / "docs/FR/Boss-et-Conseils.md", ROOT / "docs/FR/Boss-et-Conseils.html", ROOT / "wiki/FR/Boss-et-Conseils.md"),
    "en": (ROOT / "docs/EN/Bosses-and-Tips.md", ROOT / "docs/EN/Bosses-and-Tips.html", ROOT / "wiki/EN/Bosses-and-Tips.md"),
}

BOSS_FR = {
    "Falkner": "Albert", "Bugsy": "Hector", "Whitney": "Blanche", "Morty": "Mortimer",
    "Pryce": "Frédo", "Clair": "Sandra", "Will": "Clément", "Bruno": "Aldo",
    "Karen": "Marion", "Brock": "Pierre", "Misty": "Ondine", "Lt. Surge": "Major Bob",
    "Sabrina": "Morgane", "Janine": "Jeannine", "Blaine": "Auguste", "Champion": "Maître",
}

ARCHETYPE_FR = {"fire": "Feu", "water": "Eau", "grass": "Plante", "electric": "Électrik", "ground": "Sol", "ice": "Glace"}

TRAINER_SPRITES = {
    "Albert": "leader_falkner_hns.png", "Hector": "leader_bugsy_hns.png",
    "Blanche": "leader_whitney_hns.png", "Mortimer": "leader_morty_hns.png",
    "Chuck": "leader_chuck_hns.png", "Jasmine": "leader_jasmine_hns.png",
    "Frédo": "leader_pryce_hns.png", "Sandra": "leader_clair_hns.png",
    "Clément": "elite_four_will_hns.png", "Koga": "elite_four_koga_hns.png",
    "Aldo": "elite_four_bruno_hns.png", "Marion": "elite_four_karen_hns.png",
    "Proton": "proton_hns.png", "Petrel": "petrel_hns.png", "Ariana": "ariana_hns.png",
    "Archer": "archer_hns.png", "Pierre": "leader_brock_hns.png",
    "Ondine": "leader_misty_hns.png", "Major Bob": "leader_surge_hns.png",
    "Erika": "leader_erika_hns.png", "Morgane": "leader_sabrina_hns.png",
    "Jeannine": "leader_janine_hns.png", "Auguste": "leader_blaine_hns.png",
    "Blue": "leader_blue_hns.png", "Maître": "champion_lance_hns.png",
}

SPECIES_FR = {
    "Alolan Muk": "Grotadmorv d’Alola",
    "Alolan Ninetales": "Feunard d’Alola",
    "Arctozolt (Galvagla)": "Galvagla",
    "Galarian Slowking": "Roigada de Galar",
    "Galarian Weezing": "Smogogo de Galar",
    "Hisuian Zoroark": "Zoroark de Hisui",
    "Indeedee-F": "Wimessir femelle",
    "Mega Charizard X": "Méga-Dracaufeu X",
    "Mega Charizard Y": "Méga-Dracaufeu Y",
    "Mega Raichu Y": "Méga-Raichu Y",
    "Rotom-Wash": "Motisma-Lavage",
}


def load_localization() -> dict:
    text = LOCALIZATION.read_text(encoding="utf-8").strip()
    return json.loads(text.removeprefix("window.TacticaLocalization=").removesuffix(";"))


def localized_key(value: str) -> str:
    value = value.lower().replace("’", "'")
    for prefix, suffix in (("alolan ", "-alola"), ("galarian ", "-galar"), ("hisuian ", "-hisui")):
        if value.startswith(prefix):
            return value.removeprefix(prefix) + suffix
    return value


def translate(value: str | None, category: str, loc: dict, lang: str) -> str:
    if value is None:
        return "—"
    if lang == "en":
        return "King's Shield" if category == "moves" and value == "Kings Shield" else value
    if category == "species" and value in SPECIES_FR:
        return SPECIES_FR[value]
    if category == "species" and value == "saved starter":
        return "starter fixe"
    mega = category == "species" and value.startswith("Mega ")
    lookup = value.removeprefix("Mega ") if mega else value
    if category == "moves" and value == "Kings Shield":
        return "Bouclier Royal"
    translated = loc[category].get(localized_key(lookup), lookup)
    if mega:
        translated = "Méga-" + translated
    return translated


def group_teams() -> OrderedDict[tuple[str, str, str], list[dict]]:
    groups: OrderedDict[tuple[str, str, str], list[dict]] = OrderedDict()
    for row in json.loads(BOSSES.read_text(encoding="utf-8"))["teams"]:
        groups.setdefault((row["region"], row["category"], row["boss"]), []).append(row)
    return groups


def rival_groups() -> OrderedDict[tuple[str, str, str], list[dict]]:
    spec = json.loads(RIVAL.read_text(encoding="utf-8"))
    groups: OrderedDict[tuple[str, str, str], list[dict]] = OrderedDict()
    fixed = spec["selection"]["fixed_starters"]
    for archetype, party in spec["fight_rosters"]["categories"].items():
        rows = []
        archetype_mega = spec["archetype_rules"][archetype]["mega_species"]
        for slot, member in enumerate(party, 1):
            species = member.get("mega_species") or member.get("target_final_species") or member["family"]
            if member["family"] == "saved starter":
                species = member.get("mega_species") or fixed[archetype]
                moves = ["level-legal moves"]
            else:
                moves = member["phase_moves"]["final"]
            if member.get("final_item", "").endswith("ite") and member.get("final_item") != "Eviolite":
                species = archetype_mega
            ability = member.get("mega_ability") if species == archetype_mega else member.get("ability")
            rows.append({
                "slot": slot, "species": species, "level": None,
                "item": member.get("final_item"), "ability": ability,
                "moves": moves, "role": member["role"], "theme": archetype,
            })
        groups[("Rival", "Rival", archetype)] = rows
    return groups


def sections(groups: OrderedDict, rivals: OrderedDict) -> list[tuple[str, list[tuple[tuple, list[dict]]]]]:
    johto, rocket, kanto, rematch = [], [], [], []
    for key, rows in groups.items():
        region, category, _ = key
        if category == "Rocket Executive":
            rocket.append((key, rows))
        elif region == "Johto":
            johto.append((key, rows))
        elif category == "Champion Rematch":
            rematch.append((key, rows))
        else:
            kanto.append((key, rows))
    kanto_order = {name: index for index, name in enumerate(("Major Bob", "Morgane", "Erika", "Jeannine", "Ondine", "Pierre", "Auguste", "Blue"))}
    kanto.sort(key=lambda entry: kanto_order[entry[0][2]])
    return [("johto", johto), ("rocket", rocket), ("rival", list(rivals.items())), ("kanto", kanto), ("rematch", rematch)]


def labels(lang: str) -> dict:
    if lang == "fr":
        return {
            "title": "Boss & Conseils", "intro": "Cette page est générée depuis les équipes canoniques du jeu. Elle permet de préparer les combats sans imposer un déroulé tour par tour.",
            "johto": "Johto et première Ligue", "rocket": "Team Rocket", "rival": "Rival", "kanto": "Kanto", "rematch": "Deuxième Ligue",
            "pokemon": "Pokémon", "level": "Niv.", "item": "Objet", "ability": "Talent", "moves": "Capacités",
            "dynamic": "Dynamique", "rocket_note": "Les Exécutifs passent de 3 à 4 puis 6 Pokémon. Leur niveau suit le dernier jalon Champion ou Rival pertinent, augmenté de 2 ; les Méga n'apparaissent qu'au combat FINAL.",
            "rival_note": "Le rival conserve un starter fixe selon son archétype et progresse de 1 à 3, 4 puis 6 Pokémon. Son niveau continue de suivre la logique du prochain jalon majeur et sa Méga n'arrive qu'après le badge 4.",
        }
    return {
        "title": "Bosses & Tips", "intro": "This page is generated from the game's canonical teams. Use it to prepare for major battles without following a turn-by-turn script.",
        "johto": "Johto and first League", "rocket": "Team Rocket", "rival": "Rival", "kanto": "Kanto", "rematch": "Second League",
        "pokemon": "Pokémon", "level": "Lv.", "item": "Item", "ability": "Ability", "moves": "Moves",
        "dynamic": "Dynamic", "rocket_note": "Executives grow from 3 to 4 and then 6 Pokémon. Their level is the latest relevant Gym or Rival milestone plus 2; Mega Evolution appears only in the FINAL battle.",
        "rival_note": "The rival keeps a fixed starter for each archetype and grows from 1 to 3, 4 and then 6 Pokémon. Its level logic is unchanged, and Mega Evolution begins only after Badge 4.",
    }


def display_boss(key: tuple, lang: str) -> str:
    region, category, boss = key
    if category == "Rival":
        return ("Archétype " + ARCHETYPE_FR[boss]) if lang == "fr" else boss.title() + " archetype"
    if lang == "fr":
        return BOSS_FR.get(boss, boss)
    reverse = {value: key for key, value in BOSS_FR.items()}
    return reverse.get(boss, boss)


def row_values(row: dict, loc: dict, lang: str, dynamic: str) -> list[str]:
    level = str(row["level"]) if row.get("level") is not None else dynamic
    moves = " · ".join(translate(move, "moves", loc, lang) for move in row["moves"])
    return [
        translate(row["species"], "species", loc, lang), level,
        translate(row.get("item"), "items", loc, lang),
        translate(row.get("ability"), "abilities", loc, lang), moves,
    ]


def pokemon_sprite(species: str) -> str:
    local = {
        "Mega Charizard X": "../assets/mega-charizard-x.png",
        "Mega Charizard Y": "../assets/mega-charizard-y.png",
    }
    if species in local:
        return local[species]
    name = species.split(" (", 1)[0]
    suffixes = (("Alolan ", "-alola"), ("Galarian ", "-galar"), ("Hisuian ", "-hisui"))
    for prefix, suffix in suffixes:
        if name.startswith(prefix):
            name = name.removeprefix(prefix) + suffix
    if name.startswith("Mega "):
        bits = name.removeprefix("Mega ").split()
        name = bits[0] + "-mega" + (("-" + "-".join(bits[1:])) if len(bits) > 1 else "")
    normalized = unicodedata.normalize("NFKD", name).encode("ascii", "ignore").decode().lower()
    slug = re.sub(r"[^a-z0-9]+", "-", normalized).strip("-")
    return f"https://play.pokemonshowdown.com/sprites/gen5/{slug}.png"


def trainer_sprite(key: tuple) -> str:
    boss = key[2]
    sprite = "silver_hns.png" if key[1] == "Rival" else TRAINER_SPRITES[boss]
    return f"../assets/trainers/{sprite}"


def render_markdown(lang: str, groups: OrderedDict, rivals: OrderedDict, loc: dict) -> str:
    t = labels(lang)
    if lang == "fr":
        nav = "[Accueil](Accueil.md) · [Guide de jeu](Guide-de-jeu.md) · [Changements](Changements.md) · [Pokédex](Pokedex.md) · [Localisations](Localisations.md) · **[Boss & Conseils](Boss-et-Conseils.md)** · [Crédits](Credits-et-Versions.md) · [Roadmap](Roadmap.md) · **[EN](../EN/Bosses-and-Tips.md)**"
    else:
        nav = "[Home](Home.md) · [Game Guide](Game-Guide.md) · [Changes](Changes.md) · [Pokédex](Pokedex.md) · [Locations](Locations.md) · **[Bosses & Tips](Bosses-and-Tips.md)** · [Credits](Credits-and-Versions.md) · [Roadmap](Roadmap.md) · **[FR](../FR/Boss-et-Conseils.md)**"
    output = [nav, "", f"# {t['title']}", "", t["intro"], ""]
    for section, entries in sections(groups, rivals):
        output.extend((f"## {t[section]}", ""))
        if section == "rocket": output.extend((t["rocket_note"], ""))
        if section == "rival": output.extend((t["rival_note"], ""))
        for key, rows in entries:
            theme = (("équipe progressive" if lang == "fr" else "progressive team")
                     if key[1] == "Rival" else rows[0].get("theme") or key[1])
            output.extend((f"### {display_boss(key, lang)} — {theme}", "", f"| {t['pokemon']} | {t['level']} | {t['item']} | {t['ability']} | {t['moves']} |", "|---|---:|---|---|---|"))
            for row in rows:
                output.append("| " + " | ".join(row_values(row, loc, lang, t["dynamic"])) + " |")
            output.append("")
    return "\n".join(output).rstrip() + "\n"


def render_html(lang: str, groups: OrderedDict, rivals: OrderedDict, loc: dict) -> str:
    t = labels(lang)
    if lang == "fr":
        nav = '<a class="brand" href="../index.html">Pokémon Tactica</a><a href="Accueil.html">Accueil</a><a href="Guide-de-jeu.html">Guide</a><a href="Changements.html">Changements</a><a href="Pokedex.html">Pokédex</a><a href="Localisations.html">Localisations</a><a href="Routes-et-Villes.html">Routes et Villes</a><a class="active" href="Boss-et-Conseils.html">Boss & Conseils</a><a href="Credits-et-Versions.html">Crédits</a><a href="Roadmap.html">Roadmap</a><span class="langs"><a class="lang current" href="Boss-et-Conseils.html">FR</a><a class="lang" href="../EN/Bosses-and-Tips.html">EN</a></span>'
    else:
        nav = '<a class="brand" href="../index.html">Pokémon Tactica</a><a href="Home.html">Home</a><a href="Game-Guide.html">Guide</a><a href="Changes.html">Changes</a><a href="Pokedex.html">Pokédex</a><a href="Locations.html">Locations</a><a href="Routes-and-Cities.html">Routes & Cities</a><a class="active" href="Bosses-and-Tips.html">Bosses & Tips</a><a href="Credits-and-Versions.html">Credits</a><a href="Roadmap.html">Roadmap</a><span class="langs"><a class="lang" href="../FR/Boss-et-Conseils.html">FR</a><a class="lang current" href="Bosses-and-Tips.html">EN</a></span>'
    parts = [f'<!doctype html><html lang="{lang}"><head><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1"><title>{html.escape(t["title"])} — Pokémon Tactica</title><link rel="stylesheet" href="../assets/site.css"></head><body><header class="top"><nav class="nav">{nav}</nav></header><main><span class="eyebrow">CANONICAL TEAMS</span><h1>{html.escape(t["title"])}</h1><p>{html.escape(t["intro"])}</p>']
    for section, entries in sections(groups, rivals):
        parts.append(f'<h2 id="{section}">{html.escape(t[section])}</h2>')
        if section == "rocket": parts.append(f'<p class="status-note">{html.escape(t["rocket_note"])}</p>')
        if section == "rival": parts.append(f'<p class="status-note">{html.escape(t["rival_note"])}</p>')
        for key, rows in entries:
            theme = (("équipe progressive" if lang == "fr" else "progressive team")
                     if key[1] == "Rival" else rows[0].get("theme") or key[1])
            boss_name = display_boss(key, lang)
            parts.append(f'<details class="boss-card"><summary><span class="trainer-head"><img class="trainer-sprite" loading="lazy" src="{trainer_sprite(key)}" alt="{html.escape(boss_name)}"><span><strong>{html.escape(boss_name)}</strong><small>{html.escape(str(theme))}</small></span></span></summary><div class="table-wrap"><table><thead><tr><th>{t["pokemon"]}</th><th>{t["level"]}</th><th>{t["item"]}</th><th>{t["ability"]}</th><th>{t["moves"]}</th></tr></thead><tbody>')
            for row in rows:
                values = row_values(row, loc, lang, t["dynamic"])
                pokemon = f'<span class="poke-name"><img class="poke-sprite" loading="lazy" src="{pokemon_sprite(row["species"])}" alt="{html.escape(values[0])}" onerror="this.style.display=\'none\'"><strong>{html.escape(values[0])}</strong></span>'
                cells = "<td>" + pokemon + "</td>" + "".join(f"<td>{html.escape(value)}</td>" for value in values[1:])
                parts.append(f"<tr>{cells}</tr>")
            parts.append("</tbody></table></div></details>")
    parts.append(f'<div class="footer">Pokémon Tactica — {html.escape(t["title"])}</div></main></body></html>\n')
    return "".join(parts)


def synchronize(check: bool) -> None:
    loc = load_localization()
    groups, rivals = group_teams(), rival_groups()
    for lang, paths in PAGES.items():
        md = render_markdown(lang, groups, rivals, loc)
        rendered = (md, render_html(lang, groups, rivals, loc), md)
        for path, expected in zip(paths, rendered):
            current = path.read_text(encoding="utf-8") if path.exists() else ""
            if current != expected:
                if check:
                    raise SystemExit(f"Boss wiki is stale; run tools/sync_tactica_boss_wiki.py: {path}")
                path.write_text(expected, encoding="utf-8")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    synchronize(parser.parse_args().check)
