#!/usr/bin/env python3
"""Build clean wiki maps and single-frame Pokémon art from engine assets."""

from __future__ import annotations

import argparse
import io
import json
import struct
from pathlib import Path

from PIL import Image, ImageOps


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "docs" / "assets"


def png_bytes(image: Image.Image) -> bytes:
    output = io.BytesIO()
    image.save(output, "PNG", optimize=False)
    return output.getvalue()


def write_or_check(path: Path, content: bytes, check: bool) -> None:
    if check:
        if not path.exists() or path.read_bytes() != content:
            raise SystemExit(f"Wiki asset is stale; run tools/sync_tactica_wiki_assets.py: {path}")
    else:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(content)


def transparent_frame(path: Path, size: tuple[int, int] = (64, 64)) -> Image.Image:
    source = Image.open(path)
    frame = source.crop((0, 0, *size))
    if frame.mode == "P":
        alpha = Image.new("L", frame.size, 255)
        pixels = frame.get_flattened_data() if hasattr(frame, "get_flattened_data") else frame.getdata()
        alpha.putdata([0 if pixel == 0 else 255 for pixel in pixels])
        rgba = frame.convert("RGBA")
        rgba.putalpha(alpha)
        return rgba
    return frame.convert("RGBA")


def region_map(name: str) -> Image.Image:
    base = ROOT / "graphics" / "pokedex"
    tiles = Image.open(base / f"region_map_{name}.png").convert("RGBA")
    raw = (base / f"region_map_{name}.bin").read_bytes()
    entries = struct.unpack(f"<{len(raw) // 2}H", raw)
    output = Image.new("RGBA", (256, 160), (0, 0, 0, 0))
    for index, entry in enumerate(entries):
        x, y = index % 32, index // 32
        if y >= 20:
            break
        tile_id = entry & 0x3FF
        tx, ty = (tile_id % 16) * 8, (tile_id // 16) * 8
        tile = tiles.crop((tx, ty, tx + 8, ty + 8))
        if entry & 0x400:
            tile = ImageOps.mirror(tile)
        if entry & 0x800:
            tile = ImageOps.flip(tile)
        output.alpha_composite(tile, (x * 8, y * 8))
    return output


def species_source(species: str) -> Path:
    slug = species.removeprefix("SPECIES_").lower()
    if slug.endswith("_alola"):
        base, _ = slug.rsplit("_", 1)
        directory = ROOT / "graphics" / "pokemon" / base / "alola"
        return directory / ("anim_front.png" if (directory / "anim_front.png").exists() else "front.png")
    if slug.endswith("_galar"):
        base, _ = slug.rsplit("_", 1)
        directory = ROOT / "graphics" / "pokemon" / base / "galar"
        return directory / ("anim_front.png" if (directory / "anim_front.png").exists() else "front.png")
    directory = ROOT / "graphics" / "pokemon" / slug
    return directory / ("anim_front.png" if (directory / "anim_front.png").exists() else "front.png")


def synchronize(check: bool) -> None:
    for region in ("johto", "kanto"):
        write_or_check(ASSETS / f"guide-{region}.png", png_bytes(region_map(region)), check)

    starters = json.loads((ROOT / "data" / "spec" / "starters.json").read_text(encoding="utf-8"))
    species = [entry for group in starters["categories"].values() for entry in group["species"]]
    species.append(starters["eevee"]["species"])
    for entry in species:
        slug = entry.removeprefix("SPECIES_").lower().replace("_", "-")
        image = transparent_frame(species_source(entry))
        write_or_check(ASSETS / "starters" / f"{slug}.png", png_bytes(image), check)

    for form in ("mega_x", "mega_y"):
        source = ROOT / "graphics" / "pokemon" / "charizard" / form / "front.png"
        output = ASSETS / f"mega-charizard-{form[-1]}.png"
        write_or_check(output, png_bytes(transparent_frame(source)), check)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    synchronize(parser.parse_args().check)
