#!/usr/bin/env python3
"""Validate the Tactica Mega Evolution progression and Morty integration."""

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit(f"Tactica Mega validation failed: {message}")


def sections(text: str, marker: str) -> list[str]:
    return [part.split("\n=== ", 1)[0] for part in text.split(marker)[1:]]


def main() -> None:
    species_config = (ROOT / "include/config/species_enabled.h").read_text(encoding="utf-8")
    pokemon_config = (ROOT / "include/config/pokemon.h").read_text(encoding="utf-8")
    battle_config = (ROOT / "include/config/battle.h").read_text(encoding="utf-8")
    player_controller = (ROOT / "src/battle_controller_player.c").read_text(encoding="utf-8")
    battle_util = (ROOT / "src/battle_util.c").read_text(encoding="utf-8")
    morty_script = (ROOT / "data/maps/EcruteakCity_Gym_hns/scripts.inc").read_text(encoding="utf-8")
    parties = (ROOT / "src/data/trainers_hns.party").read_text(encoding="utf-8")
    gengar_family = (ROOT / "src/data/pokemon/species_info/gen_1_families.h").read_text(encoding="utf-8")

    require("#define P_MEGA_EVOLUTIONS                TRUE" in species_config,
            "Mega Evolution must be enabled in the production species configuration")
    require("#define P_MODIFIED_MEGA_CRIES            FALSE" in pokemon_config,
            "Mega forms must reuse base cries to protect the 32 MiB production ROM budget")
    require("#if P_MODIFIED_MEGA_CRIES\n        .cryId = CRY_SLOWBRO_MEGA,\n    #else\n        .cryId = CRY_SLOWBRO," in gengar_family,
            "Mega Slowbro must fall back to the base cry when modified Mega cries are disabled")
    rayquaza_family = (ROOT / "src/data/pokemon/species_info/gen_3_families.h").read_text(encoding="utf-8")
    require("#if P_MODIFIED_MEGA_CRIES\n        .cryId = CRY_RAYQUAZA_MEGA,\n    #else\n        .cryId = CRY_RAYQUAZA," in rayquaza_family,
            "Mega Rayquaza must fall back to the base cry when modified Mega cries are disabled")
    require("#define B_MOVE_DESCRIPTION_BUTTON           SELECT_BUTTON" in battle_config,
            "START must remain available for the battle gimmick command")
    description = player_controller.index("else if (JOY_NEW(B_MOVE_DESCRIPTION_BUTTON)")
    gimmick = player_controller.index("else if (JOY_NEW(START_BUTTON))", description)
    require(description < gimmick,
            "the move-description and Mega inputs are no longer handled in the expected flow")

    mega_check = battle_util.split("bool32 CanMegaEvolve", 1)[1].split("bool32 CanUltraBurst", 1)[0]
    require("CheckBagHasItem(ITEM_MEGA_RING, 1)" in mega_check,
            "player Mega Evolution must require the Mega Ring")

    badge = morty_script.index("setflag FLAG_BADGE04_GET")
    ring = morty_script.index("giveitem ITEM_MEGA_RING")
    require(badge < ring, "Morty must award the Mega Ring after badge 4")

    morty_teams = sections(parties, "=== TRAINER_MORTY_1_HNS ===\n")
    require(len(morty_teams) == 2, "Morty must have one Normal and one Hard primary team")
    for expected, team in zip(("Difficulty: Normal", "Difficulty: Hard"), morty_teams):
        require(expected in team, f"Morty team is missing {expected}")
        require("Gengar @ Gengarite" in team, f"{expected} Morty must carry Gengarite")
        gengar = team.split("Gengar @ Gengarite", 1)[1]
        require("Ability: Cursed Body" in gengar,
                f"{expected} Morty Gengar must use its legal base ability before Mega Evolution")

    mega_gengar = gengar_family.split("[SPECIES_GENGAR_MEGA]", 1)[1].split("#endif //P_MEGA_EVOLUTIONS", 1)[0]
    require(".abilities = { ABILITY_SHADOW_TAG, ABILITY_SHADOW_TAG, ABILITY_SHADOW_TAG }" in mega_gengar,
            "Mega Gengar must gain Shadow Tag through transformation")

    print("Tactica Mega validation passed: input, Mega Ring, Morty teams and Gengar abilities")


if __name__ == "__main__":
    main()
