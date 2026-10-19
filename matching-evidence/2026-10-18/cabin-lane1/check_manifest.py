#!/usr/bin/env python3
"""Verify objdiff.json against map/units.json, src/, and build/report.json.

Questions answered:
  1. Do WT manifest entries == map units that have a text range (name + order)?
  2. What explains the 5 names in the HEAD<->WT name diff (1 removed, 4 added, 1 rename)?
  3. Is the WT manifest internally consistent (base_path/auto_generated/source_path vs src/ files)?
  4. Do the `complete` flags match build/report.json statuses (via map_sha256 gate)?
"""
import hashlib
import json
import os
import subprocess

DC = r'C:\rush2049-dreamcast'


def load_json(p):
    with open(p, 'r', encoding='utf-8') as f:
        return json.load(f)


def main():
    map_path = os.path.join(DC, 'map', 'units.json')
    map_raw = open(map_path, 'rb').read()
    m = json.loads(map_raw)
    map_sha = hashlib.sha256(map_raw).hexdigest()
    wt = load_json(os.path.join(DC, 'objdiff.json'))
    head_raw = subprocess.run(
        ['git', '-C', DC, 'show', 'HEAD:objdiff.json'],
        stdout=subprocess.PIPE, check=True).stdout
    head = json.loads(head_raw.decode('utf-8'))

    map_units = m['units']
    map_names = [u['name'] for u in map_units]
    text_units, no_text = [], []
    for u in map_units:
        t = u.get('text')
        ok = False
        if isinstance(t, dict):
            ok = bool(t.get('start') and t.get('end'))
        elif isinstance(t, list) and t:
            t = t[0]
            ok = bool(t) and bool(t.get('start')) and bool(t.get('end')) if isinstance(t, dict) else False
        (text_units if ok else no_text).append(u['name'])

    wt_names = [e['name'] for e in wt['units']]
    head_names = [e['name'] for e in head['units']]

    print(f'map: {len(map_names)} units; with text: {len(text_units)}; without text: {len(no_text)}')
    print(f'  no-text: {no_text}')
    print(f'WT objdiff: {len(wt_names)} units;  HEAD objdiff: {len(head_names)} units')
    print(f'WT names == map text-units (same order): {wt_names == text_units}')

    only_wt = [n for n in wt_names if n not in head_names]
    only_head = [n for n in head_names if n not in wt_names]
    print(f'in WT only ({len(only_wt)}): {only_wt}')
    print(f'in HEAD only ({len(only_head)}): {only_head}')

    interesting = ['unk_8C0951C0', 'unk_8C01021C_video', 'unk_8C01026C_entry',
                   'unk_8C0102A4_lz', 'asincos', 'unk_8C010218', 'unk_8C010218_empty']
    print('--- names in question ---')
    for n in interesting:
        u = next((x for x in map_units if x['name'] == n), None)
        src = os.path.isfile(os.path.join(DC, 'src', n + '.c'))
        t = u.get('text') if u else None
        tstr = 'none' if not t else f"{t.get('start')}-{t.get('end')}"
        print(f'  {n}: in_map={u is not None} text={tstr} src_exists={src}')

    print('--- WT internal consistency ---')
    bad = []
    for e in wt['units']:
        n = e['name']
        src = os.path.isfile(os.path.join(DC, 'src', n + '.c'))
        has_base = 'base_path' in e
        ag = e['metadata'].get('auto_generated')
        sp = e['metadata'].get('source_path')
        if has_base != src:
            bad.append((n, 'base_path!=src', src))
        if ag == src:  # auto_generated must be NOT src
            bad.append((n, 'auto_generated!=not(src)', src))
        if src and sp != f'src/{n}.c':
            bad.append((n, 'source_path odd', sp))
        if not src and sp is not None:
            bad.append((n, 'source_path w/o src', sp))
        if has_base and not os.path.isfile(os.path.join(DC, e['base_path'])):
            bad.append((n, 'base_path file missing', e['base_path']))
    print(f'  problems: {len(bad)}')
    for b in bad[:20]:
        print('   ', b)

    print('--- complete flags vs build/report.json ---')
    rep = load_json(os.path.join(DC, 'build', 'report.json'))
    rep_sha = rep.get('map_sha256')
    print(f'  map sha256: {map_sha}')
    print(f'  report map_sha256: {rep_sha}  match={rep_sha == map_sha}')
    status = {u['name']: u.get('status') for u in rep.get('units', [])}
    mism = [(e['name'], e['metadata'].get('complete'), status.get(e['name']))
            for e in wt['units']
            if e['metadata'].get('complete') != (status.get(e['name']) == 'exact')]
    n_complete = sum(1 for e in wt['units'] if e['metadata'].get('complete'))
    n_exact = sum(1 for s in status.values() if s == 'exact')
    print(f'  complete=true in WT: {n_complete}; status=exact in report: {n_exact}; mismatches: {len(mism)}')
    for x in mism[:20]:
        print('   ', x)

    print('--- report summary (reference) ---')
    print('  ', json.dumps(rep.get('summary', {}))[:300])
    fn = rep.get('functions') or {}
    print(f'  functions: {json.dumps({k: v for k, v in (fn.items() if isinstance(fn, dict) else [])})[:200] if fn else "n/a"}')


if __name__ == '__main__':
    main()
