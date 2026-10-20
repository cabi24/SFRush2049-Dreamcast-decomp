#!/usr/bin/env python3
"""Write objdiff.json (project configuration for tools/objdiff/objdiff.exe and objdiff-cli).

    python tools/dc_objdiff.py [--map map/units.json]

One unit per mapped unit with a text range: target_path build/target/<unit>.o (dc_split.py) and,
when src/<unit>.c exists, base_path build/obj/<unit>.o (dc_build.py: the linked bytes of the unit
as an ELF relocatable, so both sides hold absolute pool words and compare like for like).
metadata.complete is true only for units build/report.json lists as exact. The GUI rebuilds a
unit through custom_make/custom_args: `python tools/dc_build.py build/obj/<unit>.o`.

Check one unit from the command line:
    tools\\objdiff\\objdiff-cli.exe diff -p . -u ll -o out.json --format json-pretty
"""
import argparse
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dc_common as C  # noqa: E402

OBJDIFF_JSON = os.path.join(C.DC, 'objdiff.json')
CATEGORY_NAMES = {'game': 'Game code', 'sdk': 'SDK', 'library': 'Library', 'browser': 'Browser',
                  'startup': 'Startup', 'data': 'Data', 'unknown': 'Unknown'}


def write_config(m, report=None, quiet=False):
    if report is None and os.path.isfile(os.path.join(C.BUILD, 'report.json')):
        try:
            report = json.load(open(os.path.join(C.BUILD, 'report.json')))
            if report.get('map_sha256') != m['sha256']:
                report = None
        except ValueError:
            report = None
    status = {e['name']: e.get('status') for e in (report or {}).get('units', [])}
    units, cats = [], []
    for u in m['units']:
        if not u['text']:
            continue
        kind = u['kind'] or 'unknown'
        if kind not in cats:
            cats.append(kind)
        entry = {'name': u['name'], 'target_path': f'build/target/{u["name"]}.o'}
        src = os.path.join(C.SRC, u['name'] + '.c')
        if os.path.isfile(src):
            entry['base_path'] = f'build/obj/{u["name"]}.o'
        meta = {'complete': status.get(u['name']) == 'exact', 'reverse_fn_order': False,
                'progress_categories': [kind], 'auto_generated': not os.path.isfile(src)}
        if os.path.isfile(src):
            meta['source_path'] = f'src/{u["name"]}.c'
        entry['metadata'] = meta
        units.append(entry)
    cfg = {
        '$schema': 'https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json',
        'min_version': '2.0.0',
        'custom_make': 'python',
        'custom_args': ['tools/dc_build.py', '--no-objdiff'],
        'build_target': False,
        'build_base': True,
        'target_dir': 'build/target',
        'base_dir': 'build/obj',
        'watch_patterns': ['src/**/*.c', 'src/**/*.h', 'src/*.txt', 'map/*.json'],
        'ignore_patterns': ['build/**/*'],
        'progress_categories': [{'id': c, 'name': CATEGORY_NAMES.get(c, c)} for c in cats],
        'units': units,
    }
    C.write_atomic(OBJDIFF_JSON, json.dumps(cfg, indent=2) + '\n', newline='\n')
    if not quiet:
        print(f'[dc_objdiff] {C.rel(OBJDIFF_JSON)}: {len(units)} units '
              f'({sum(1 for x in units if "base_path" in x)} with source) from {C.rel(m["path"])}')
    return cfg


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--map', help='unit map (default map/units.json, else map/units.seed.json)')
    args = ap.parse_args()
    write_config(C.load_map(args.map))
    return 0


if __name__ == '__main__':
    sys.exit(main())
