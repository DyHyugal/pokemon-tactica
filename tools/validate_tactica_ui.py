#!/usr/bin/env python3
"""Check actual image/tilemap assets; this does not certify in-game rendering."""
from pathlib import Path
import struct
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]

def require(condition: bool, message: str) -> None:
    if not condition:
        raise SystemExit(message)

def render_card_page(tiles_path: Path, map_path: Path) -> list[list[int]]:
    tiles = Image.open(tiles_path)
    entries = struct.unpack("<600H", map_path.read_bytes())
    rendered = [[0] * 240 for _ in range(160)]
    for pos, entry in enumerate(entries):
        tile = entry & 0x3FF
        for y in range(8):
            for x in range(8):
                source_x = 7 - x if entry & 0x400 else x
                source_y = 7 - y if entry & 0x800 else y
                rendered[(pos // 30) * 8 + y][(pos % 30) * 8 + x] = tiles.getpixel(
                    ((tile % 16) * 8 + source_x, (tile // 16) * 8 + source_y)
                )
    return rendered


def main() -> None:
    for name in ("front.bin", "back.bin", "bg.bin"):
        require((ROOT / "graphics/trainer_card/hns/swsh" / name).stat().st_size == 1200,
                f"Trainer Card tilemap must fit its 30x20 buffer exactly: {name}")
    card_tiles = ROOT / "graphics/trainer_card/hns/swsh/tiles.png"
    card_front = render_card_page(card_tiles, ROOT / "graphics/trainer_card/hns/swsh/front.bin")
    card_back = render_card_page(card_tiles, ROOT / "graphics/trainer_card/hns/swsh/back.bin")
    require(all(card_front[y][x] != 5 for y in range(18, 28) for x in range(10, 99))
            and all(card_front[y][x] != 5 for y in range(116, 127) for x in range(10, 56))
            and all(card_back[y][x] != 5 for y in range(18, 30) for x in range(20, 106)),
            "the Trainer Card source labels must stay neutral so runtime localization remains authoritative")
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
    for path in (
        "graphics/party_menu/swsh/tiles.png",
        "graphics/bag/swsh/tiles.png",
        "graphics/summary_screen/swsh/tiles.png",
        "graphics/battle_interface/bw/actionbox.png",
        "graphics/battle_interface/bw/healthbox_singles_player.png",
        "graphics/battle_interface/bw/mega_trigger.png",
        "graphics/shop/b2w2/menu.png",
        "graphics/ui_startmenu_full/menu.png",
        "graphics/ui_startmenu_full/menu_tiles.png",
        "graphics/trainer_card/hns/swsh/tiles.png",
    ):
        image = Image.open(ROOT / path)
        require(image.mode in ("P", "RGBA") and image.getbbox() is not None,
                f"modern UI asset is missing or invalid: {path}")

    print("Tactica UI asset checks passed: files, tilemap buffers, source labels and banner centering; in-game appearance remains manual")

if __name__ == '__main__':
    main()
