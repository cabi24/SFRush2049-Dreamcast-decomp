"""MAT3 function inventory in source order with call sequences and strings.
-> raw/mat3_funcs.json: [{file, name, line, calls:[...], strings:[...], gc_size, stripped}] and file order."""
import os, re, json, csv, glob, collections
from common import RAW
SRC = r'C:\rush2049-mat3\decomp\src'
CSV = r'C:\rush2049-mat3\rush_d_functions.csv'
rows = list(csv.DictReader(open(CSV)))
gc = {}
order = []
for r in rows:
    f = r['source_file']
    if f not in order: order.append(f)
    gc[(f, r['name'])] = dict(size=int(r['size']) if r['size'] else 0, stripped=r['stripped'] == '1', addr=r['address'])
allnames = set(r['name'] for r in rows)
defre = re.compile(r'^(?![\s#{}])(?!typedef|extern|return|else|if\b)([A-Za-z_][\w\s\*]*?)\b([A-Za-z_]\w*)\s*\(([^;]*)\)\s*(\{)?\s*$')
callre = re.compile(r'\b([A-Za-z_]\w*)\s*\(')
lit = re.compile(r'"((?:[^"\\n]|\.)*)"')
fltre = re.compile(r'(?<![\w.])(\d+\.\d*(?:[eE][-+]?\d+)?[fF]?|\.\d+(?:[eE][-+]?\d+)?[fF]?|\d+[eE][-+]?\d+[fF]?)(?![\w.])')
KW = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'else', 'do', 'case'}
out = []
files = {}
for path in sorted(glob.glob(os.path.join(SRC, '**', '*.c*'), recursive=True)):
    rel = os.path.relpath(path, SRC).replace(os.sep, '/')
    base = os.path.basename(path)
    lines = open(path, encoding='latin-1').read().split('\n')
    i = 0; funcs = []
    while i < len(lines):
        m = defre.match(lines[i])
        if m and m.group(2) not in KW:
            j = i if m.group(4) else i + 1
            if j < len(lines) and lines[j].strip().startswith('{') or m.group(4):
                k = j + 1
                while k < len(lines) and not lines[k].startswith('}'):
                    k += 1
                body = '\n'.join(lines[j:k + 1])
                body_nc = re.sub(r'/\*.*?\*/', ' ', body, flags=re.S)
                body_nc = re.sub(r'//[^\n]*', ' ', body_nc)
                strs = [s for s in lit.findall(body_nc)]
                floats = sorted(set(float(x.rstrip('fF')) for x in fltre.findall(lit.sub('""', body_nc))))
                body_ns = lit.sub('""', body_nc)
                calls = [c for c in callre.findall(body_ns) if c not in KW]
                name = m.group(2)
                g = gc.get((base, name), {})
                funcs.append(dict(file=rel, base=base, name=name, line=i + 1, end_line=k + 1, calls=calls, floats=floats,
                                  strings=strs, gc_size=g.get('size', 0), stripped=g.get('stripped', None),
                                  static='static' in m.group(1)))
                i = k + 1; continue
        i += 1
    files[rel] = [f['name'] for f in funcs]
    out.extend(funcs)
# full per-file order from DWARF (includes GC-deadstripped functions)
dw = collections.defaultdict(list)
for r in rows:
    dw[r['source_file']].append((int(r['debug_offset'], 16), r['name'], r['stripped'] == '1', int(r['size']) if r['size'] else 0, r['address']))
srcmap = {(f['base'], f['name']): f for f in out}
full = []
for fname in order:
    for k, (off, name, st, size, addr) in enumerate(sorted(dw[fname])):
        f = srcmap.get((fname, name))
        full.append(dict(file=fname, name=name, idx=k, stripped=st, gc_size=size, gc_addr=addr,
                         calls=f['calls'] if f else [], strings=f['strings'] if f else [],
                         floats=f['floats'] if f else [], has_src=bool(f)))
json.dump(full, open(os.path.join(RAW, 'mat3_full.json'), 'w'))
json.dump(dict(funcs=out, gc_order=order), open(os.path.join(RAW, 'mat3_funcs.json'), 'w'))
print(len(out), 'functions in', len(files), 'files')
# coverage check vs csv
have = set((f['base'], f['name']) for f in out)
miss = [k for k in gc if k not in have]
print('csv functions not found in source:', len(miss), miss[:20])
