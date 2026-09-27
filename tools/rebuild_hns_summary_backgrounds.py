#!/usr/bin/env python3
"""Remove legacy light panels from the HnS Summary tilemaps.

The labels are baked into the shared tileset, so a global palette replacement
would erase them too. This script finds only large connected light surfaces in
each visible page, clones the affected tiles, darkens those surfaces, and
rewrites the tilemap entries to the clones. It is intentionally idempotent.
"""

from collections import deque
from pathlib import Path
import struct

from PIL import Image


ROOT = Path(__file__).resolve().parents[1]
GFX = ROOT / "graphics" / "summary_screen" / "hns"
MAPS = (
    "page_info.bin",
    "page_info_egg.bin",
    "page_skills.bin",
    "page_battle_moves.bin",
    "page_contest_moves.bin",
)
LIGHT = {2, 3, 4, 5, 19, 20, 21, 37, 66, 67, 68, 69}
MIN_SURFACE_PIXELS = 100
VISIBLE_WIDTH = 240
VISIBLE_HEIGHT = 160


def transform(tile, hflip, vflip):
    if hflip:
        tile = tile.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
    if vflip:
        tile = tile.transpose(Image.Transpose.FLIP_TOP_BOTTOM)
    return tile


def main():
    tiles_path = GFX / "tiles.png"
    sheet = Image.open(tiles_path)
    palette = sheet.getpalette()
    transparency = sheet.info.get("transparency")
    tiles = []
    for y in range(0, sheet.height, 8):
        for x in range(0, sheet.width, 8):
            tiles.append(sheet.crop((x, y, x + 8, y + 8)))

    clone_ids = {}
    changed_maps = 0
    for map_name in MAPS:
        map_path = GFX / map_name
        entries = list(struct.unpack("<1024H", map_path.read_bytes()))
        rendered = Image.new("P", (256, 256))
        rendered.putpalette(palette)
        for pos, entry in enumerate(entries):
            tile_id = entry & 0x3FF
            hflip = bool(entry & 0x400)
            vflip = bool(entry & 0x800)
            rendered.paste(transform(tiles[tile_id], hflip, vflip), ((pos % 32) * 8, (pos // 32) * 8))

        pixels = rendered.load()
        seen = set()
        replace = set()
        for y in range(VISIBLE_HEIGHT):
            for x in range(VISIBLE_WIDTH):
                if (x, y) in seen or pixels[x, y] not in LIGHT:
                    continue
                queue = deque([(x, y)])
                seen.add((x, y))
                component = []
                while queue:
                    px, py = queue.popleft()
                    component.append((px, py))
                    for nx, ny in ((px - 1, py), (px + 1, py), (px, py - 1), (px, py + 1)):
                        if (0 <= nx < VISIBLE_WIDTH and 0 <= ny < VISIBLE_HEIGHT
                                and (nx, ny) not in seen and pixels[nx, ny] in LIGHT):
                            seen.add((nx, ny))
                            queue.append((nx, ny))
                if len(component) >= MIN_SURFACE_PIXELS:
                    replace.update(component)

        changed_cells = {(x // 8, y // 8) for x, y in replace}
        if not changed_cells:
            continue
        for cell_x, cell_y in changed_cells:
            pos = cell_y * 32 + cell_x
            entry = entries[pos]
            tile_id = entry & 0x3FF
            hflip = bool(entry & 0x400)
            vflip = bool(entry & 0x800)
            bank = entry >> 12
            displayed = transform(tiles[tile_id], hflip, vflip).copy()
            data = list(displayed.getdata())
            dark = bank * 16 + (1 if bank else 0)
            for local_y in range(8):
                for local_x in range(8):
                    if (cell_x * 8 + local_x, cell_y * 8 + local_y) in replace:
                        data[local_y * 8 + local_x] = dark
            displayed.putdata(data)
            canonical = transform(displayed, hflip, vflip)
            key = bytes(canonical.getdata())
            if key not in clone_ids:
                clone_ids[key] = len(tiles)
                tiles.append(canonical)
            entries[pos] = (entry & 0xFC00) | clone_ids[key]
        map_path.write_bytes(struct.pack("<1024H", *entries))
        changed_maps += 1

    if changed_maps:
        rows = (len(tiles) + 15) // 16
        output = Image.new("P", (128, rows * 8))
        output.putpalette(palette)
        for tile_id, tile in enumerate(tiles):
            output.paste(tile, ((tile_id % 16) * 8, (tile_id // 16) * 8))
        save_args = {"transparency": transparency} if transparency is not None else {}
        output.save(tiles_path, **save_args)
    print(f"HnS Summary: {changed_maps} tilemap(s) rebuilt, {len(tiles)} tiles")


if __name__ == "__main__":
    main()
