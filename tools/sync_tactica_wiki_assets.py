#!/usr/bin/env python3
"""Build clean wiki maps and single-frame Pokémon art from engine assets."""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path

from PIL import Image, ImageOps


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "docs" / "assets"


def write_or_check(path: Path, image: Image.Image, check: bool) -> None:
    if check:
        if not path.exists():
            raise SystemExit(f"Wiki asset is stale; run tools/sync_tactica_wiki_assets.py: {path}")
        with Image.open(path) as current:
            if current.size != image.size or current.convert("RGBA").tobytes() != image.convert("RGBA").tobytes():
                raise SystemExit(f"Wiki asset is stale; run tools/sync_tactica_wiki_assets.py: {path}")
    else:
        path.parent.mkdir(parents=True, exist_ok=True)
        image.save(path, "PNG", optimize=False)


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


TRAINER_ASSETS = (
    "archer_hns.png", "ariana_hns.png", "champion_lance_hns.png",
    "elite_four_bruno_hns.png", "elite_four_karen_hns.png", "elite_four_koga_hns.png",
    "elite_four_will_hns.png", "leader_blaine_hns.png", "leader_blue_hns.png",
    "leader_brock_hns.png", "leader_bugsy_hns.png", "leader_chuck_hns.png",
    "leader_clair_hns.png", "leader_erika_hns.png", "leader_falkner_hns.png",
    "leader_janine_hns.png", "leader_jasmine_hns.png", "leader_misty_hns.png",
    "leader_morty_hns.png", "leader_pryce_hns.png", "leader_sabrina_hns.png",
    "leader_surge_hns.png", "leader_whitney_hns.png", "petrel_hns.png",
    "proton_hns.png", "silver_hns.png",
)


def transparent_indexed_frame(path: Path, size: tuple[int, int] = (64, 64)) -> Image.Image:
    """Preserve indexed trainer art while making engine palette index 0 transparent."""
    source = Image.open(path)
    frame = source.crop((0, 0, *size))
    if frame.mode == "P":
        frame.info["transparency"] = 0
        return frame
    return transparent_frame(path, size)


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
        write_or_check(ASSETS / f"guide-{region}.png", region_map(region), check)

    starters = json.loads((ROOT / "data" / "spec" / "starters.json").read_text(encoding="utf-8"))
    species = [entry for group in starters["categories"].values() for entry in group["species"]]
    species.append(starters["eevee"]["species"])
    for entry in species:
        slug = entry.removeprefix("SPECIES_").lower().replace("_", "-")
        image = transparent_frame(species_source(entry))
        write_or_check(ASSETS / "starters" / f"{slug}.png", image, check)

    for form in ("mega_x", "mega_y"):
        source = ROOT / "graphics" / "pokemon" / "charizard" / form / "front.png"
        output = ASSETS / f"mega-charizard-{form[-1]}.png"
        write_or_check(output, transparent_frame(source), check)

    trainer_source = ROOT / "graphics" / "trainers" / "front_pics"
    for filename in TRAINER_ASSETS:
        write_or_check(
            ASSETS / "trainers" / filename,
            transparent_indexed_frame(trainer_source / filename),
            check,
        )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true")
    synchronize(parser.parse_args().check)
