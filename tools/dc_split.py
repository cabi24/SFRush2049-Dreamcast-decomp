#!/usr/bin/env python3
"""Cut the target image into one ELF relocatable per mapped unit (the objdiff "target" side).

    python tools/dc_split.py [--map map/units.json] [--unit NAME ...] [--out build/target]

For every unit with a text range in the map this writes build/target/<unit>.o: an ELF32
little-endian SH (e_machine 42, e_flags SH4) relocatable whose .text holds the unit's target bytes
(section offset 0 = text start), one sized global FUNC symbol per mapped function (linker name,
e.g. _InitLL; size runs to the next function so pools travel with the function before them; the
last function stops at the end of the last function/pool, i.e. trailing inter-unit alignment is
left out), plus .data / .rodata / .bss sections and OBJECT symbols when the map gives data ranges.
No relocations are emitted: target pools hold absolute addresses.

It also writes build/target/symbols.txt ("<linker symbol> 0x<address>" per mapped function and data
symbol, the table dc_build.py feeds to LNK DEFINE) and build/target/split.json (what was written).
The map defaults to map/units.json, or map/units.seed.json while the real map does not exist.
"""
import argparse
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dc_common as C  # noqa: E402


def unit_objects(unit):
    """Build (sections, symbols) for one unit's target object."""
    sections, symbols = [], []
    t0, t1 = unit['text']
    text = C.target_bytes(t0, t1)
    if text is None:
        raise ValueError(f'text range {t0:08X}..{t1:08X} lies outside the image')
    sections.append({'name': '.text', 'data': text, 'align': 4})
    for f, start, end in C.function_extents(unit):
        symbols.append({'name': f['symbol'], 'section': 0, 'value': start - t0, 'size': end - start,
                        'type': C.STT_FUNC, 'bind': C.STB_GLOBAL})
    counts = {}
    for key in ('data', 'bss'):
        for d in unit[key]:
            letter = d['section']
            base_name = C.SECTION_ELF_NAME.get(letter, '.' + letter.lower())
            counts[base_name] = counts.get(base_name, 0) + 1
            name = base_name if counts[base_name] == 1 else f'{base_name}_{counts[base_name]}'
            if counts[base_name] == 2:
                C.warn(f'{unit["name"]}: several {base_name} ranges; extra ones become {name}, ...')
            size = d['end'] - d['start']
            if key == 'bss' or letter == 'B':
                sections.append({'name': name, 'data': None, 'size': size, 'align': 4})
            else:
                data = C.target_bytes(d['start'], d['end'])
                if data is None:
                    C.warn(f'{unit["name"]}: {letter} range {d["start"]:08X}..{d["end"]:08X} is outside the image; '
                           'written as NOBITS')
                    sections.append({'name': name, 'data': None, 'size': size, 'align': 4})
                else:
                    sections.append({'name': name, 'data': data, 'align': 4})
            idx = len(sections) - 1
            syms = sorted(d['symbols'], key=lambda s: s['address'])
            for k, s in enumerate(syms):
                if not d['start'] <= s['address'] < d['end']:
                    C.warn(f'{unit["name"]}: symbol {s["name"]} {s["address"]:08X} outside its range; skipped')
                    continue
                size_s = s['size']
                if size_s is None:
                    size_s = (syms[k + 1]['address'] if k + 1 < len(syms) else d['end']) - s['address']
                symbols.append({'name': s['symbol'], 'section': idx, 'value': s['address'] - d['start'],
                                'size': size_s, 'type': C.STT_OBJECT, 'bind': C.STB_GLOBAL})
    return sections, symbols


def collect_symbols(m):
    """{linker symbol: (address, unit, kind)} for every mapped function and data symbol."""
    table = {}
    for u in m['units']:
        entries = [(f['symbol'], f['start'], 'func') for f in u['functions']]
        entries += [(s['symbol'], s['address'], 'data') for s in u['symbols']]
        for sym, addr, kind in entries:
            if sym in table and table[sym][0] != addr:
                C.warn(f'symbol {sym} mapped twice ({table[sym][0]:08X} in {table[sym][1]}, '
                       f'{addr:08X} in {u["name"]}); keeping the first')
                continue
            table.setdefault(sym, (addr, u['name'], kind))
    return table


def split(map_path=None, units=None, out_dir=C.TARGET_DIR, quiet=False):
    m = C.load_map(map_path)
    C.load_target()
    os.makedirs(out_dir, exist_ok=True)
    written = []
    wanted = set(units) if units else None
    for u in m['units']:
        if wanted is not None and u['name'] not in wanted:
            continue
        if not u['text']:
            C.warn(f'{u["name"]}: no text range; no target object')
            continue
        try:
            sections, symbols = unit_objects(u)
        except ValueError as e:
            C.warn(f'{u["name"]}: {e}')
            continue
        path = os.path.join(out_dir, u['name'] + '.o')
        C.write_elf(path, sections, symbols, file_name=u['name'] + '.c')
        written.append({'unit': u['name'], 'path': C.rel(path), 'text': [f'{u["text"][0]:08X}', f'{u["text"][1]:08X}'],
                        'functions': len(u['functions']),
                        'sections': [s['name'] for s in sections]})
    if wanted is None:  # drop target objects of units the map no longer has
        keep = {w['unit'] + '.o' for w in written}
        for f in os.listdir(out_dir):
            if f.endswith('.o') and f not in keep:
                os.remove(os.path.join(out_dir, f))
    if wanted:
        missing = wanted - {w['unit'] for w in written}
        for n in sorted(missing):
            C.warn(f'unit {n} not split (not in the map or no text range)')
    table = collect_symbols(m)
    with open(os.path.join(out_dir, 'symbols.txt'), 'w', encoding='utf-8', newline='\n') as f:
        f.write(f'# generated by tools/dc_split.py from {C.rel(m["path"])} (sha256 {m["sha256"][:16]})\n')
        f.write('# <linker symbol> <address>: every mapped function and data symbol, for LNK DEFINE\n')
        for sym, (addr, unit, kind) in sorted(table.items(), key=lambda kv: (kv[1][0], kv[0])):
            f.write(f'{sym} 0x{addr:08X}\n')
    manifest = {'map': C.rel(m['path']), 'map_sha256': m['sha256'], 'target_sha256': C.TARGET_SHA256,
                'units': written, 'symbols': len(table)}
    if wanted is None:
        json.dump(manifest, open(os.path.join(out_dir, 'split.json'), 'w'), indent=1)
    if not quiet:
        for w in written:
            print(f'[dc_split] {w["unit"]:24s} {w["text"][0]}..{w["text"][1]}  {w["functions"]:3d} functions  '
                  f'{",".join(w["sections"])}')
        print(f'[dc_split] {len(written)} objects in {C.rel(out_dir)}, {len(table)} symbols in '
              f'{C.rel(os.path.join(out_dir, "symbols.txt"))} (map {C.rel(m["path"])})')
    return m, written


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--map', help='unit map (default map/units.json, else map/units.seed.json)')
    ap.add_argument('--unit', action='append', help='split only this unit (repeatable; symbols.txt still covers the whole map)')
    ap.add_argument('--out', default=C.TARGET_DIR, help='output directory (default build/target)')
    args = ap.parse_args()
    units = [n for arg in args.unit for n in arg.split(',')] if args.unit else None
    split(args.map, units, args.out)
    return 0


if __name__ == '__main__':
    sys.exit(main())
