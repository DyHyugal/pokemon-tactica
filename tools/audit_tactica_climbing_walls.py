#!/usr/bin/env python3
"""Report actually placed Rock Climb tiles in HnS layouts (read-only)."""

import collections
import json
import re
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def audit():
    behaviors = re.findall(
        r'^\s*(MB_\w+)\s*,',
        (ROOT / 'include/constants/metatile_behaviors.h').read_text(), re.M,
    )
    climb = behaviors.index('MB_ROCK_CLIMB')
    attributes = {}
    for bits, symbol, filename in re.findall(
        r'const u(16|32)\s+(gMetatileAttributes_\w+)\[\]\s*=\s*INCBIN_U\d+\("([^"]+)"\)',
        (ROOT / 'src/data/tilesets/metatiles.h').read_text(),
    ):
        data = (ROOT / filename).read_bytes()
        attributes[symbol] = [value for (value,) in struct.iter_unpack('<H' if bits == '16' else '<I', data)]
    tilesets = dict(re.findall(
        r'const struct Tileset (gTileset_\w+)\s*=\s*\{[^}]*?\.metatileAttributes\s*=\s*(\w+)',
        (ROOT / 'src/data/tilesets/headers.h').read_text(), re.S,
    ))
    maps = collections.defaultdict(list)
    for filename in (ROOT / 'data/maps').glob('*/map.json'):
        entry = json.loads(filename.read_text())
        maps[entry['layout']].append(entry['id'])
    found = []
    checked = 0
    layouts = json.loads((ROOT / 'data/layouts/layouts.json').read_text())['layouts']
    for layout in layouts:
        if layout.get('layout_version', 'emerald') != 'hns':
            continue
        checked += 1
        climb_ids = set()
        for offset, key in ((0, 'primary_tileset'), (640, 'secondary_tileset')):
            values = attributes[tilesets[layout[key]]]
            climb_ids.update(index + offset for index, value in enumerate(values) if value & 255 == climb)
        if not climb_ids:
            continue
        blocks = struct.iter_unpack('<H', (ROOT / layout['blockdata_filepath']).read_bytes())
        positions = [(index % layout['width'], index // layout['width'])
                     for index, (block,) in enumerate(blocks) if block & 1023 in climb_ids]
        if positions:
            found.append({'layout': layout['id'], 'maps': maps[layout['id']],
                          'count': len(positions), 'positions': positions[:10]})
    return {'behavior': climb, 'hns_layouts_checked': checked, 'layouts_with_climb': found}


if __name__ == '__main__':
    print(json.dumps(audit(), indent=2))
