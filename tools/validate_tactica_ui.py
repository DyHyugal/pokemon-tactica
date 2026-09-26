#!/usr/bin/env python3
"""Validate the HNS/Tactica UI invariants requested by the playtest."""

from pathlib import Path


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


def main() -> None:
    menu = (ROOT / "src/menu.c").read_text(encoding="utf-8")
    start_menu = (ROOT / "src/start_menu.c").read_text(encoding="utf-8")
    battle_interface = (ROOT / "src/battle_interface.c").read_text(encoding="utf-8")
    battle_message = (ROOT / "src/battle_message.c").read_text(encoding="utf-8")
    graphics = (ROOT / "src/graphics.c").read_text(encoding="utf-8")
    shop = (ROOT / "src/shop.c").read_text(encoding="utf-8")
    summary = (ROOT / "src/pokemon_summary_screen.c").read_text(encoding="utf-8")
    violet_gym = (ROOT / "data/maps/VioletCity_Gym_hns/scripts.inc").read_text(encoding="utf-8")
    violet_map = (ROOT / "data/maps/VioletCity_Gym_hns/map.json").read_text(encoding="utf-8")
    trainers = (ROOT / "src/data/trainers_hns.party").read_text(encoding="utf-8")

    require("(numActions * 2) + 2, 15, 0x50" in menu,
            "HNS start-menu pixels must stay below the overworld tilemap blocks")
    require("sHnsStartMenuTextColors[] = {12, 10, 13}" in start_menu,
            "HNS start-menu text must remain black on red")
    require("FillWindowPixelBuffer(windowId, PIXEL_FILL(12))" in start_menu,
            "HNS start-menu background must remain red")
    require("#define HEALTHBOX_BG_INDEX 7" in battle_interface,
            "HNS healthbox dynamic fields must remain dark")
    require(".background = 7" in battle_interface and ".foreground = 2" in battle_interface,
            "HNS healthbox text must remain light on dark")
    require("FillWindowPixelRect(windowId, PIXEL_FILL(2), 0, 14, 64, 2)" in battle_message,
            "move rows must retain their red horizontal separator")
    require(graphics.count('graphics/battle_interface/hns/textbox.gbapal') == 2,
            "both battle tilemap palette banks must be initialized explicitly")
    require(".fillValue = 12" in shop
            and "[COLORID_NORMAL]      = {12, 10, 13}" in shop
            and "FillWindowPixelBuffer(sMartInfo.windowId, PIXEL_FILL(12))" in shop
            and "FillWindowPixelBuffer(WIN_ITEM_DESCRIPTION, PIXEL_FILL(12))" in shop,
            "HNS shops must keep the red surface / black text runtime treatment")
    require("sMonSummaryScreen->maxPageIndex = PSS_PAGE_BATTLE_MOVES;" in summary
            and 'static const u8 sText_HnsHeldItem[] = _("{STR_VAR_1}")' in summary
            and 'static const u8 sText_HnsFriendship[] = _("{STR_VAR_1}")' in summary,
            "HNS Summary must keep IV/EV on Skills without duplicate fixed labels or normal Contest page")

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

    ui_root = ROOT / "graphics/battle_interface/hns"
    require(palette_rgb(ui_root / "textbox.gbapal", 5) == (16, 16, 24),
            "battle panel fill color must remain black")

    textbox = (ui_root / "textbox.4bpp").read_bytes()
    separator = tile_pixels(textbox, 31)
    require(all(row == [0, 0, 0, 2, 2, 0, 0, 0] for row in separator),
            "battle move-column separator tile is invalid")
    tilemap = (ui_root / "textbox_map.bin").read_bytes()
    for y in range(55, 59):
        offset = (y * 32 + 10) * 2
        entry = tilemap[offset] | tilemap[offset + 1] << 8
        require(entry == 31, f"missing red move separator at tilemap row {y}")

    hpbar = (ui_root / "hpbar.4bpp").read_bytes()
    require(all((packed & 0xF) != 2 and (packed >> 4) != 2 for packed in hpbar),
            "HNS HP bar still contains the residual white plate color")

    expbar = (ui_root / "expbar.4bpp").read_bytes()
    for fill in range(9):
        pixels = tile_pixels(expbar, fill)
        for y, row in enumerate(pixels):
            expected = [0] * 8 if y >= 6 else [1] * 8
            if y in (3, 4):
                expected[:fill] = [14] * fill
            require(row == expected, f"EXP bar state {fill}, row {y} is not dark/red only")

    print("Tactica UI validation passed: Summary, shops, battle HUD/panels, menu VRAM and Violet Gym guide")


if __name__ == "__main__":
    main()
