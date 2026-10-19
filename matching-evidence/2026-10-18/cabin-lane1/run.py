"""cabin-lane1: private builds of the dirty units, WT(dirty) vs HEAD variants.

For every dirtied src/<unit>.c, build the unit twice with the frozen SHC5.1r11 + LNK
(frozen config from tools/dc_build.py: build_unit):

  dirty  - the working-tree source, C.SRC = the real src/  (== what the saved
           build/report.json was built from; consistency-checked against it)
  head   - a mirror of src/ with ONLY <unit>.c replaced by `git show HEAD:src/<unit>.c`
           (all headers are clean, so the mirror is otherwise byte-identical)

Nothing shared is written: C.BUILD/C.OBJ_DIR/C.LINK_DIR are redirected under this
directory; the canonical build/obj, build/link, build/report.json are untouched.
Outputs per variant: runs/<unit>/<variant>/report.json + source.c + hashes.json.
Output per unit: <unit>-delta.json (per-function status dirty vs head, text-byte
totals, and the report.json consistency check).

usage:  python run.py [unit ... | --all] [--rebuild-head-mirrors]
"""
import hashlib
import json
import os
import shutil
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(r'C:\rush2049-dreamcast')
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / 'tools'))
import dc_build as D  # noqa: E402
C = D.C

LOG = HERE / 'run.log'


def log(msg):
    line = f'[{time.strftime("%H:%M:%S")}] {msg}'
    print(line, flush=True)
    with open(LOG, 'a', encoding='utf-8') as f:
        f.write(line + '\n')


def dirty_units():
    r = subprocess.run(['git', 'status', '--porcelain', 'src/'], cwd=ROOT, capture_output=True)
    units = set()
    for line in r.stdout.decode('utf-8', 'replace').splitlines():
        parts = line.split(None, 1)
        if len(parts) < 2:
            continue
        p = parts[1].strip()
        if p.startswith('src/') and p.endswith('.c'):
            units.add(os.path.splitext(os.path.basename(p))[0])
    return sorted(units)


def git_show_bytes(relpath):
    r = subprocess.run(['git', 'show', f'HEAD:{relpath}'], cwd=ROOT, capture_output=True)
    if r.returncode != 0:
        raise RuntimeError(f'git show {relpath}: {r.stderr.decode("utf-8", "replace")}')
    return r.stdout


def head_source_sha(unit):
    return hashlib.sha256(git_show_bytes(f'src/{unit}.c')).hexdigest()


def build_variant(unit, variant, rebuild_head=False):
    if variant == 'dirty':
        srcdir = ROOT / 'src'
    else:
        srcdir = HERE / 'head' / unit
        if not (srcdir / f'{unit}.c').exists() or rebuild_head:
            shutil.rmtree(srcdir, ignore_errors=True)
            shutil.copytree(ROOT / 'src', srcdir)
            (srcdir / f'{unit}.c').write_bytes(git_show_bytes(f'src/{unit}.c'))
    out = HERE / 'runs' / unit / variant
    for sub in ('obj', 'link'):
        (out / sub).mkdir(parents=True, exist_ok=True)
    C.SRC = str(srcdir)
    C.BUILD = str(out)
    C.OBJ_DIR = str(out / 'obj')
    C.LINK_DIR = str(out / 'link')
    m = C.load_map()
    u = next((x for x in m['units'] if x['name'] == unit), None)
    if u is None:
        raise RuntimeError(f'unit {unit} not in {C.rel(m["path"])}')
    symbols = C.read_symbol_file(str(ROOT / 'build' / 'target' / 'symbols.txt'))
    externs = C.read_symbol_file(str(ROOT / 'src' / 'externs.txt'))
    e = D.build_unit(u, symbols, externs)
    (out / 'report.json').write_text(json.dumps(e, indent=2), encoding='utf-8')
    (out / 'source.c').write_bytes((srcdir / f'{unit}.c').read_bytes())
    hashes = {p.name: hashlib.sha256(p.read_bytes()).hexdigest()
              for p in sorted(out.rglob('*')) if p.is_file()}
    (out / 'hashes.json').write_text(json.dumps(hashes, indent=2), encoding='utf-8')
    return e


def funcmap(e):
    return {f['name']: f for f in e.get('functions', [])}


def compact(e):
    return {'status': e.get('status'), 'functions_exact': e.get('functions_exact', 0),
            'functions_total': e.get('functions_total', 0), 'functions_moved': e.get('functions_moved', 0),
            'exact_text_bytes': e.get('exact_text_bytes', 0), 'equal_text_bytes_including_fill': e.get('equal_text_bytes_including_fill', 0),
            'text_bytes': e.get('text_bytes', 0), 'exact_data_bytes': e.get('exact_data_bytes', 0),
            'source_sha256': e.get('source_sha256')}


def main():
    args = sys.argv[1:]
    rebuild = '--rebuild-head-mirrors' in args
    args = [a for a in args if a != '--rebuild-head-mirrors']
    if not args or args == ['--all']:
        units = dirty_units()
    else:
        units = args
    LOG.write_text('', encoding='utf-8')
    report = json.load(open(ROOT / 'build' / 'report.json', 'rb'))
    rep_by_name = {e['name']: e for e in report.get('units', [])}

    tally = []
    for unit in units:
        t0 = time.time()
        log(f'=== {unit}: building dirty + head')
        e_d = build_variant(unit, 'dirty')
        log(f'  dirty : {compact(e_d)}')
        e_h = build_variant(unit, 'head', rebuild_head=rebuild)
        log(f'  head  : {compact(e_h)}')

        fd, fh = funcmap(e_d), funcmap(e_h)
        names = sorted(set(fd) | set(fh))
        gains, losses = [], []
        for n in names:
            ds = fd.get(n, {}).get('status')
            hs = fh.get(n, {}).get('status')
            rec = {'name': n, 'dirty': ds, 'head': hs,
                   'size': fd.get(n, fh.get(n, {})).get('size')}
            if ds == 'exact' and hs != 'exact':
                gains.append(rec)
            elif hs == 'exact' and ds != 'exact':
                losses.append(rec)

        rep = rep_by_name.get(unit)
        rep_match = None
        if rep is not None:
            rep_match = {'report_functions_exact': rep.get('functions_exact'),
                         'dirty_rebuild_functions_exact': e_d.get('functions_exact'),
                         'report_exact_text_bytes': rep.get('exact_text_bytes'),
                         'dirty_rebuild_exact_text_bytes': e_d.get('exact_text_bytes'),
                         'report_source_sha256': rep.get('source_sha256'),
                         'dirty_source_sha256': e_d.get('source_sha256'),
                         'match': rep.get('functions_exact') == e_d.get('functions_exact')
                                  and rep.get('exact_text_bytes') == e_d.get('exact_text_bytes')
                                  and rep.get('source_sha256') == e_d.get('source_sha256')}

        verdict = 'keep' if len(gains) > len(losses) else ('revert' if len(losses) > len(gains) else 'neutral')
        d = {'unit': unit, 'verdict': verdict,
             'dirty': compact(e_d), 'head': compact(e_h),
             'gains_dirty_over_head': gains, 'losses_dirty_vs_head': losses,
             'text_bytes_dirty': e_d.get('exact_text_bytes'), 'text_bytes_head': e_h.get('exact_text_bytes'),
             'report_consistency': rep_match,
             'head_source_sha256': head_source_sha(unit),
             'seconds': round(time.time() - t0, 1)}
        (HERE / f'{unit}-delta.json').write_text(json.dumps(d, indent=2), encoding='utf-8')
        tally.append(d)
        log(f'  delta : gains={len(gains)} losses={len(losses)} -> {verdict}'
            + (f'  GAIN ' + ','.join(g['name'] for g in gains) if gains else '')
            + (f'  LOSS ' + ','.join(l['name'] for l in losses) if losses else '')
            + (f'  [report check {rep_match["match"]}]' if rep_match else ''))

    (HERE / 'tally.json').write_text(json.dumps(tally, indent=2), encoding='utf-8')
    n_keep = sum(1 for t in tally if t['verdict'] == 'keep')
    n_rev = sum(1 for t in tally if t['verdict'] == 'revert')
    n_neu = sum(1 for t in tally if t['verdict'] == 'neutral')
    tot_gain = sum(len(t['gains_dirty_over_head']) for t in tally)
    tot_loss = sum(len(t['losses_dirty_vs_head']) for t in tally)
    log(f'DONE: {len(tally)} units: keep={n_keep} revert={n_rev} neutral={n_neu}; '
        f'functions gained by dirty={tot_gain}, lost by dirty={tot_loss}')


if __name__ == '__main__':
    main()
