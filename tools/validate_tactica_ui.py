#!/usr/bin/env python3
"""Validate the HNS/Tactica UI invariants requested by the playtest."""

from collections import deque
from pathlib import Path
import struct

from PIL import Image


ROOT = Path(__file__).resolve().parents[1]


def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit(message)


def palette_rgb(path: Path, index: int) -> tuple[int, int, int]:
    data = path.read_bytes()
    value = data[index * 2] | data[index * 2 + 1] << 8
    return ((value & 31) * 8, ((value >> 5) & 31) * 8, ((value >> 10) & 31) * 8)


def tile_pixels(data: bytes, tile: int) -> list[list[int]]:
    pixels = []
    start = tile * 32
    for y in range(8):
        row = []
        for packed in data[start + y * 4:start + y * 4 + 4]:
            row.extend((packed & 0xF, packed >> 4))
        pixels.append(row)
    return pixels


def render_summary_page(tile_data: bytes, map_path: Path) -> list[list[int]]:
    entries = struct.unpack("<1024H", map_path.read_bytes())
    rendered = [[0] * 256 for _ in range(256)]
    for pos, entry in enumerate(entries):
        tile = tile_pixels(tile_data, entry & 0x3FF)
        if entry & 0x400:
            tile = [row[::-1] for row in tile]
        if entry & 0x800:
            tile = tile[::-1]
        bank = entry >> 12
        left, top = (pos % 32) * 8, (pos // 32) * 8
        for y, row in enumerate(tile):
            for x, pixel in enumerate(row):
                rendered[top + y][left + x] = bank * 16 + pixel
    return rendered


def largest_light_surface(image: list[list[int]]) -> int:
    light = {2, 3, 4, 5, 19, 20, 21, 37, 66, 67, 68, 69}
    seen: set[tuple[int, int]] = set()
    largest = 0
    for y in range(160):
        for x in range(240):
            if (x, y) in seen or image[y][x] not in light:
                continue
            queue = deque([(x, y)])
            seen.add((x, y))
            size = 0
            while queue:
                px, py = queue.popleft()
                size += 1
                for nx, ny in ((px - 1, py), (px + 1, py), (px, py - 1), (px, py + 1)):
                    if (0 <= nx < 240 and 0 <= ny < 160
                            and (nx, ny) not in seen and image[ny][nx] in light):
                        seen.add((nx, ny))
                        queue.append((nx, ny))
            largest = max(largest, size)
    return largest


def main() -> None:
    menu = (ROOT / "src/menu.c").read_text(encoding="utf-8")
    start_menu = (ROOT / "src/start_menu.c").read_text(encoding="utf-8")
    text_window = (ROOT / "src/text_window.c").read_text(encoding="utf-8")
    malloc_header = (ROOT / "include/malloc.h").read_text(encoding="utf-8")
    general_config = (ROOT / "include/config/general.h").read_text(encoding="utf-8")
    battle_controller = (ROOT / "src/battle_controller_player.c").read_text(encoding="utf-8")
    battle_ui = (ROOT / "src/bw_battle_ui.c").read_text(encoding="utf-8")
    battle_ui_config = (ROOT / "include/config/bw_battle_ui.h").read_text(encoding="utf-8")
    battle_config = (ROOT / "include/config/battle.h").read_text(encoding="utf-8")
    bag_config = (ROOT / "include/config/swsh_item_menu.h").read_text(encoding="utf-8")
    party_config = (ROOT / "include/constants/party_menu.h").read_text(encoding="utf-8")
    summary_config = (ROOT / "include/swsh_summary_screen.h").read_text(encoding="utf-8")
    scrcmd = (ROOT / "src/scrcmd.c").read_text(encoding="utf-8")
    pokedex = (ROOT / "src/pokedex_plus_hgss.c").read_text(encoding="utf-8")
    summary = (ROOT / "src/pokemon_summary_screen.c").read_text(encoding="utf-8")
    challenge_menu = (ROOT / "src/challenge_menu.c").read_text(encoding="utf-8")
    title_screen = (ROOT / "src/title_screen.c").read_text(encoding="utf-8")
    violet_gym = (ROOT / "data/maps/VioletCity_Gym_hns/scripts.inc").read_text(encoding="utf-8")
    violet_map = (ROOT / "data/maps/VioletCity_Gym_hns/map.json").read_text(encoding="utf-8")
    trainers = (ROOT / "src/data/trainers_hns.party").read_text(encoding="utf-8")

    require('INCBIN_U16("graphics/interface/std_menu.gbapal")' in menu
            and "AddWindowParameterized(0, 22, 1, 7" in menu,
            "the Start menu must use the original compact layout and palette")
    require("sHnsStartMenuDescriptions" not in start_menu
            and "DrawHnsStartMenuActions" not in start_menu,
            "the oversized HNS Start-menu renderer must stay retired")
    require('INCBIN_U8("graphics/text_window/1.4bpp")' in text_window
            and 'INCBIN_U16("graphics/text_window/1.gbapal")' in text_window,
            "the default textbox must use its original frame and palette")
    require("#define SWSH_ITEM_MENU                  TRUE" in bag_config,
            "the Sword/Shield Bag must remain enabled")
    require("#define SWSH_PARTY_MENU                   TRUE" in party_config,
            "the Sword/Shield party menu must remain enabled")
    require("#define SWSH_SUMMARY_SCREEN                           TRUE" in summary_config
            and "#define SWSH_SUMMARY_SHOW_IV_EV                       TRUE" in summary_config,
            "the Sword/Shield Summary and its IV/EV page must remain enabled")
    require("ShowPokemonSummaryScreen_SwSh" in summary,
            "the public Summary entry point must remain routed to the Sword/Shield screen")
    require("CreatePokemartMenu(ptr);" in scrcmd
            and "CreateDecorationShop1Menu(ptr);" in scrcmd
            and "CreateDecorationShop2Menu(ptr);" in scrcmd
            and "NewShop_Create" not in scrcmd,
            "standard and decoration shops must use the stable native shop")
    require("#define MUDSKIP_SHOP_UI" not in "\n".join(
                line for line in general_config.splitlines()
                if not line.lstrip().startswith("//")),
            "the animated shop must stay disabled to preserve runtime memory")
    require("tileset_interface_DECA_hns" not in pokedex
            and "tileset_interface_hns" not in pokedex,
            "the HGSS Pokédex must not load the retired Tactica recolor assets")
    require(all(f"#define {setting}" in battle_ui_config for setting in (
                "BW_BATTLE_UI", "BW_BATTLE_UI_TEXTBOX", "BW_BATTLE_UI_INPUTBOX",
                "BW_BATTLE_UI_PARTY_SUMMARY", "BW_BATTLE_UI_HEALTHBOX",
                "BW_BATTLE_UI_ABILITY_POP_UP", "BW_BATTLE_UI_WINDOW_SPRITES"))
            and battle_ui_config.count("(TRUE)") >= 7,
            "all Black/White battle-interface components must remain enabled")
    require("#define B_MOVE_DESCRIPTION_BUTTON           R_BUTTON" in battle_config
            and "else if (JOY_NEW(START_BUTTON))" in battle_controller
            and "ChangeGimmickTriggerSprite" in battle_controller,
            "battle controls must keep move help on R and Mega activation on START")
    require("BattleUI_CreateGimmickTriggerSprite" in battle_ui,
            "the Black/White UI must expose the interactive Mega trigger")
    require("return gBattleTextboxTiles;" in battle_ui
            and "return gBattleTextboxPalette;" in battle_ui
            and "return gBattleTextboxTilemap;" in battle_ui,
            "the battle textbox backing must preserve environmental backgrounds")
    require("#define HEAP_SIZE 0x1ED00" in malloc_header,
            "the battle/menu heap must fit background restoration after a party switch")
    require("120 - width, 0, color, 0, tabName" in challenge_menu
            and "120 - GetStringWidth(FONT_SMALL, sText_TopBar_Cancel, 0) / 2,\n        9, color" in challenge_menu,
            "the Options header must keep navigation and SAVE & EXIT on separate rows")
    require("#define VERSION_BANNER_LEFT_X 98" in title_screen
            and "#define VERSION_BANNER_RIGHT_X 162" in title_screen,
            "the Tactica version banner must keep its centered screen position")

    title_asset = Image.open(ROOT / "graphics/title_screen/hns/emerald_version.png")
    require(title_asset.mode == "P" and title_asset.size == (64, 64),
            "the Tactica version banner must remain a 64x64 indexed image")
    title_pixels = Image.new("P", (128, 32), 0)
    title_pixels.paste(title_asset.crop((0, 0, 64, 32)), (0, 0))
    title_pixels.paste(title_asset.crop((0, 32, 64, 64)), (64, 0))
    bubble = title_pixels.getbbox()
    text = title_pixels.point(lambda pixel: 255 if pixel == 15 else 0).getbbox()
    require(bubble is not None and text is not None
            and abs((bubble[0] + bubble[2] - 1) - (text[0] + text[2] - 1)) <= 1,
            "TACTICA must remain horizontally centered inside its version bubble")
    require('"script": "VioletCity_Gym_EventScript_GymGuy"' in violet_map,
            "the Violet Gym guide must remain wired to the map")
    require("giveitem ITEM_FRESH_WATER" in violet_gym
            and "goto_if_set FLAG_FAMILY_GYM_WATER_VIOLET" in violet_gym
            and "setflag FLAG_FAMILY_GYM_WATER_VIOLET" in violet_gym,
            "the Violet Gym guide must give exactly one Fresh Water, retrying after a full Bag")
    falkner = trainers.split("=== TRAINER_FALKNER_1_HNS ===", 1)[1].split("===", 1)[0]
    for move in ("Tailwind", "Taunt", "U-turn"):
        require(move.upper() in violet_gym and f"- {move}" in falkner,
                f"the Violet Gym advice no longer matches Falkner's {move}")

    for path in (
        "graphics/party_menu/swsh/tiles.png",
        "graphics/bag/swsh/tiles.png",
        "graphics/summary_screen/swsh/tiles.png",
        "graphics/battle_interface/bw/actionbox.png",
        "graphics/battle_interface/bw/healthbox_singles_player.png",
        "graphics/battle_interface/bw/mega_trigger.png",
    ):
        image = Image.open(ROOT / path)
        require(image.mode == "P" and image.getbbox() is not None,
                f"modern UI asset is missing or invalid: {path}")

    print("Tactica UI validation passed: SwSh menus, HGSS Pokédex, native shop, BW battle UI, title and Violet Gym guide")


if __name__ == "__main__":
    main()
