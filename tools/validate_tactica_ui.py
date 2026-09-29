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
    battle_interface = (ROOT / "src/battle_interface.c").read_text(encoding="utf-8")
    battle_message = (ROOT / "src/battle_message.c").read_text(encoding="utf-8")
    graphics = (ROOT / "src/graphics.c").read_text(encoding="utf-8")
    shop = (ROOT / "src/shop.c").read_text(encoding="utf-8")
    summary = (ROOT / "src/pokemon_summary_screen.c").read_text(encoding="utf-8")
    challenge_menu = (ROOT / "src/challenge_menu.c").read_text(encoding="utf-8")
    title_screen = (ROOT / "src/title_screen.c").read_text(encoding="utf-8")
    violet_gym = (ROOT / "data/maps/VioletCity_Gym_hns/scripts.inc").read_text(encoding="utf-8")
    violet_map = (ROOT / "data/maps/VioletCity_Gym_hns/map.json").read_text(encoding="utf-8")
    trainers = (ROOT / "src/data/trainers_hns.party").read_text(encoding="utf-8")

    require("(numActions * 2) + 2, STD_WINDOW_PALETTE_NUM, 0x50" in menu,
            "HNS start menu must use the dark/red standard palette without touching field tilemaps")
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
    require("#define BATTLE_ACTION_PROMPT_FILL       PIXEL_FILL(5)" in battle_message
            and "#define BATTLE_ACTION_MENU_FILL         PIXEL_FILL(8)" in battle_message
            and "#define BATTLE_MOVE_MENU_FILL           PIXEL_FILL(8)" in battle_message
            and "FillWindowPixelBuffer(B_WIN_ACTION_MENU, PIXEL_FILL(8))" in
                (ROOT / "src/battle_controller_player.c").read_text(encoding="utf-8"),
            "battle action and move panels must use opaque dark fills")
    require(graphics.count('graphics/battle_interface/hns/textbox.gbapal') == 2,
            "both battle tilemap palette banks must be initialized explicitly")
    require(".fillValue = 12" in shop
            and "[COLORID_NORMAL]      = {12, 10, 13}" in shop
            and "FillWindowPixelBuffer(sMartInfo.windowId, PIXEL_FILL(12))" in shop
            and "FillWindowPixelBuffer(WIN_ITEM_DESCRIPTION, PIXEL_FILL(12))" in shop
            and "FillWindowPixelBuffer(WIN_ITEM_LIST, PIXEL_FILL(12))" in shop
            and shop.count("Menu_LoadStdPalAt(BG_PLTT_ID(15))") >= 2
            and "SetStandardWindowBorderStyle(sMartInfo.windowId, FALSE);" in shop,
            "HNS shops must keep the red surface / black text runtime treatment")
    require("sMonSummaryScreen->maxPageIndex = PSS_PAGE_BATTLE_MOVES;" in summary
            and 'static const u8 sText_HnsHeldItem[] = _("{STR_VAR_1}")' in summary
            and 'static const u8 sText_HnsFriendship[] = _("{STR_VAR_1}")' in summary
            and summary.count("#if !IS_HNS") >= 7,
            "HNS Summary must keep IV/EV on Skills without duplicate fixed labels or normal Contest page")
    require(".tilemapTop = 0," in summary
            and "ClearWindowTilemap(PSS_LABEL_WINDOW_SKILLS_MODE);" in summary,
            "HNS Summary mode tabs must stay in the header and be cleared between pages")
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

    misc = (ui_root / "misc.4bpp").read_bytes()
    require(all((packed & 0xF) != 2 and (packed >> 4) != 2 for packed in misc),
            "HNS healthbox still contains the residual white plate below the HP bar")

    expbar = (ui_root / "expbar.4bpp").read_bytes()
    for fill in range(9):
        pixels = tile_pixels(expbar, fill)
        for y, row in enumerate(pixels):
            expected = [0] * 8 if y >= 6 else [1] * 8
            if y in (2, 3, 4, 5):
                expected[:fill] = [14] * fill
            require(row == expected, f"EXP bar state {fill}, row {y} is not dark/red only")

    summary_root = ROOT / "graphics/summary_screen/hns"
    summary_tiles = (summary_root / "tiles.4bpp").read_bytes()
    for page in (
        "page_info.bin",
        "page_info_egg.bin",
        "page_skills.bin",
        "page_battle_moves.bin",
        "page_contest_moves.bin",
    ):
        largest = largest_light_surface(render_summary_page(summary_tiles, summary_root / page))
        require(largest < 100,
                f"HNS Summary {page} still contains a legacy light panel ({largest} connected pixels)")

    print("Tactica UI validation passed: Summary, shops, battle HUD/panels, menu VRAM and Violet Gym guide")


if __name__ == "__main__":
    main()
