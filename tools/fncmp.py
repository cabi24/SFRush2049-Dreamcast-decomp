#!/usr/bin/env python3
"""Register-accurate side-by-side of one function: the linked build against the target.

    python -B tools/fncmp.py <unit> <function>        # by map name (with or without the leading _)
    python -B tools/fncmp.py <unit> 8C068BB8          # by address inside the function
    python -B tools/fncmp.py 8C068BB8                 # unit looked up in the map
    python -B tools/fncmp.py zoid DrawZoid --context 3 --no-pool

Reads build/report.json (the function's status, linked address and size), build/link/<unit>.abs
(the linked image) and the pinned target, decodes BOTH sides word by word with calib/sh4dis.py
(every FPU and system instruction, never desynchronising on data), aligns the two instruction
streams, prints them side by side and marks the first differing word with `<<<`. Every
PC-relative load is annotated with the literal it reads (value, symbol or unit, float view), so a
displacement that differs only because the pool moved is told apart from a wrong literal. Pool
words are shown as `.long`/`.word` data. After the function the pool that follows it in the
target (the map's pool or the report's pool segment) is listed the same way.

Run without `python -I`: sh4dis needs capstone from the user site. No comparison is masked; the
alignment and the "displacement only" classification are reading aids, the byte counts are
exact.
"""
import argparse
import difflib
import os
import struct
import sys

TOOLS = os.path.dirname(os.path.abspath(__file__))
DC = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)
sys.path.insert(0, os.path.join(DC, 'calib'))
import dc_common as C  # noqa: E402
from sysrof_abs import read_abs  # noqa: E402
from sh4dis import disasm_words  # noqa: E402  (capstone)

DELAYED = ('bra ', 'bsr ', 'braf ', 'bsrf ', 'jmp ', 'jsr ', 'rts', 'rte', 'bt/s ', 'bf/s ')


# ---------------------------------------------------------------- images

def abs_image(abs_path):
    """(sections [(name, start, length)], {addr: byte}) of every byte inside a linked section with
    contents (unwritten holes = 0x00), exactly as dc_build.py compares it."""
    sections, runs = read_abs(abs_path)
    with_data = {sec for sec, _a, _c in runs}
    image = {}
    for i, (_name, addr, length) in enumerate(sections):
        if i in with_data:
            for a in range(addr, addr + length):
                image[a] = 0
    for _sec, addr, chunk in runs:
        for k, b in enumerate(chunk):
            image[addr + k] = b
    return [(n, a, l) for n, a, l in sections], image


def target_get(addr):
    data = C.load_target()
    off = addr - C.BASE
    return data[off] if 0 <= off < len(data) else None


def make_side(get, addr, length, label):
    """A side: words decoded from [addr, addr+length) with get(a) -> byte | None."""
    raw = [get(addr + i) for i in range(length)]
    code = bytes(b if b is not None else 0 for b in raw)
    built = [b is not None for b in raw]
    words = []
    for a, wb, text in disasm_words(code, addr):
        i = a - addr
        words.append({'addr': a, 'bytes': wb, 'word': wb[0] | (wb[1] << 8), 'text': text,
                      'built': built[i] and built[i + 1], 'kind': 'insn'})
    if length % 2:
        words.append({'addr': addr + length - 1, 'bytes': code[-1:], 'word': None, 'text': f'.byte 0x{code[-1]:02X}',
                      'built': built[-1], 'kind': 'byte'})
    while words and not words[-1]['built']:  # nothing linked there: not part of this side
        words.pop()
    return {'label': label, 'addr': addr, 'length': length, 'get': get, 'words': words}


def scan_literals(get, ranges, known=None):
    """{literal address: size} for every PC-relative load inside `ranges` [(start, end)], iterated
    so that words identified as literals are not themselves read as loads (a pool word such as
    0x8C0D9484 would otherwise decode as a mov.w). `known` seeds the set (the map's pools)."""
    words = []
    for s, e in ranges:
        code = bytes(b if b is not None else 0 for b in (get(a) for a in range(s, e)))
        words += [(a, wb[0] | (wb[1] << 8)) for a, wb, _t in disasm_words(code, s)]
    lits = dict(known or {})
    for _ in range(4):
        new = dict(known or {})
        for a, w in words:
            if a in lits or (lits.get(a - 2) == 4):
                continue
            t = pcrel(a, w)
            if t:
                new[t[0]] = max(new.get(t[0], 0), t[1])
        if new == lits:
            break
        lits = new
    return lits


# ---------------------------------------------------------------- annotation

def pcrel(addr, word):
    """(literal address, size) for mov.w/mov.l @(disp,pc) and mova; else None."""
    hi = word >> 12
    if hi == 0x9:
        return addr + 4 + (word & 0xFF) * 2, 2
    if hi == 0xD:
        return (addr & ~3) + 4 + (word & 0xFF) * 4, 4
    if (word >> 8) == 0xC7:
        return (addr & ~3) + 4 + (word & 0xFF) * 4, 4
    return None


class Names:
    """Symbol and unit names for literal values (build/target/symbols.txt, src/externs.txt, map)."""

    def __init__(self, m):
        self.by_addr = {}
        for path in (C.SYMBOLS_TXT, C.EXTERNS_TXT):
            for sym, addr in C.read_symbol_file(path).items():
                self.by_addr.setdefault(addr, sym)
        self.ranges = []
        for u in m['units']:
            if u['text']:
                self.ranges.append((u['text'][0], u['text'][1], u['name'], 'text'))
            for d in u['data']:
                self.ranges.append((d['start'], d['end'], u['name'], d['section']))
            for b in u['bss']:
                self.ranges.append((b['start'], b['end'], u['name'], b['section']))
        self.ranges.sort()

    def describe(self, value, size):
        if size == 2:
            v = value if value < 0x8000 else value - 0x10000
            return f'{v}' if v != value else f'0x{value:04X}'
        sym = self.by_addr.get(value)
        if sym:
            return f'[{sym}]'
        for lo, hi, name, sec in self.ranges:
            if lo <= value < hi:
                return f'[{name} {sec}+{value - lo:#x}]'
        if C.BASE <= value < C.BASE + C.TARGET_SIZE:
            return '[image, unmapped]'
        f = struct.unpack('<f', struct.pack('<I', value))[0]
        if f == f and 1e-7 <= abs(f) <= 1e9:
            return f'f={f:.9g}'
        if value >= 0x80000000:
            return f'{value - 0x100000000}'
        return f'{value}'


def annotate(side, names, literals=None):
    """Attach the loaded value to every PC-relative load and show literal words as data.
    `literals` ({address: size}, from scan_literals over the whole unit) marks pool words that
    other functions of the unit load; the side's own loads are always included."""
    literals = dict(literals or {})
    for i, w in enumerate(side['words']):
        if w['word'] is None:
            continue
        t = pcrel(w['addr'], w['word'])
        if t is None:
            continue
        lit, size = t
        literals[lit] = max(literals.get(lit, 0), size)
        raw = [side['get'](lit + k) for k in range(size)]
        if any(b is None for b in raw):
            w['note'] = f'@{lit:08X} not built'
            continue
        value = int.from_bytes(bytes(raw), 'little')
        mova = (w['word'] >> 8) == 0xC7
        w['literal'] = (lit, size, value)
        if mova:
            w['note'] = f'r0={lit:08X}'
        else:
            w['note'] = f'@{lit:08X}={value:0{size * 2}X} {names.describe(value, size)}'
        if i and any(side['words'][i - 1]['text'].startswith(d) for d in DELAYED):
            w['note'] += ' (delay slot)'
    for w in side['words']:
        if w['kind'] != 'insn':
            continue
        if literals.get(w['addr'] - 2) == 4:
            w['kind'] = 'lit'
            w['text'] = '(high half)'
            continue
        size = literals.get(w['addr'])
        if size is None:
            continue
        raw = [side['get'](w['addr'] + k) for k in range(size)]
        if any(b is None for b in raw):
            continue
        value = int.from_bytes(bytes(raw), 'little')
        w['kind'] = 'lit'
        w['text'] = (f'.long 0x{value:08X}' if size == 4 else f'.word 0x{value:04X}') + '  ' + names.describe(value, size)


# ---------------------------------------------------------------- rendering

def align(a_words, b_words):
    """Pair up the two word lists by instruction text (difflib); returns [(ia, ib)] with None
    for an unmatched position. Literal words pair positionally."""
    key = lambda w: w['text'] if w['kind'] == 'insn' else '<lit>'
    sm = difflib.SequenceMatcher(None, [key(w) for w in a_words], [key(w) for w in b_words], autojunk=False)
    pairs = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal' or tag == 'replace':
            n = max(i2 - i1, j2 - j1)
            for k in range(n):
                pairs.append((i1 + k if i1 + k < i2 else None, j1 + k if j1 + k < j2 else None))
        elif tag == 'delete':
            pairs += [(i, None) for i in range(i1, i2)]
        else:
            pairs += [(None, j) for j in range(j1, j2)]
    return pairs


def classify(a, b):
    """'same' | 'disp' (same opcode and literal value, different displacement) | 'diff'."""
    if a is None or b is None:
        return 'diff'
    if a['bytes'] == b['bytes'] and a['built']:
        return 'same'
    la, lb = a.get('literal'), b.get('literal')
    if la and lb and a['built'] and (a['word'] & 0xFF00) == (b['word'] & 0xFF00) and la[1] == lb[1] and la[2] == lb[2]:
        return 'disp'
    return 'diff'


def fmt(w, body_end=None):
    if w is None:
        return ''
    hexs = w['bytes'].hex() if w['built'] else '----'
    text = w['text']
    note = w.get('note', '')
    extra = ''
    if body_end is not None and w['addr'] >= body_end and w['kind'] != 'lit':
        extra = ' ;next'
    s = f'{w["addr"]:08X} {hexs:4s} {text:<30s}'
    if note or extra:
        s += ' ' + note + extra
    return s


def render(linked, target, names, out=print, context=None, header=None, linked_lits=None, target_lits=None):
    """Print the two sides aligned. Returns a summary dict (word counts and first diff)."""
    annotate(linked, names, linked_lits)
    annotate(target, names, target_lits)
    pairs = align(linked['words'], target['words'])
    classes = [classify(linked['words'][ia] if ia is not None else None,
                        target['words'][ib] if ib is not None else None) for ia, ib in pairs]
    first = next((k for k, c in enumerate(classes) if c != 'same'), None)
    first_real = next((k for k, c in enumerate(classes) if c == 'diff'), None)
    if header:
        out(header)
    out(f'{"":2s}{linked["label"]:<66s} | {target["label"]}')
    show = set(range(len(pairs)))
    if context is not None:
        show = set()
        for k, c in enumerate(classes):
            if c != 'same':
                show.update(range(max(0, k - context), min(len(pairs), k + context + 1)))
    last = -1
    for k, ((ia, ib), c) in enumerate(zip(pairs, classes)):
        if k not in show:
            continue
        if last != k - 1:
            out('  ...')
        last = k
        a = linked['words'][ia] if ia is not None else None
        b = target['words'][ib] if ib is not None else None
        mark = {'same': ' ', 'disp': 'd', 'diff': '*'}[c]
        tail = ''
        if k == first:
            tail = '   <<< first diff' + (' (displacement only)' if c == 'disp' else '')
        if k == first_real and first_real != first:
            tail = '   <<< first non-displacement diff'
        out(f'{mark} {fmt(a, linked.get("body_end")):<66s} | {fmt(b):<66s}{tail}')
    n_disp = sum(1 for c in classes if c == 'disp')
    n_diff = sum(1 for c in classes if c == 'diff')
    out(f'  words: {len(linked["words"])} linked / {len(target["words"])} target; '
        f'{n_disp} displacement-only, {n_diff} differing'
        + (f'; first diff at target {target["words"][pairs[first][1]]["addr"]:08X}' if first is not None and pairs[first][1] is not None else '')
        + (f'; first non-displacement diff at target {target["words"][pairs[first_real][1]]["addr"]:08X}'
           if first_real is not None and first_real != first and pairs[first_real][1] is not None else ''))
    return {'disp': n_disp, 'diff': n_diff, 'first': first, 'first_real': first_real}


# ---------------------------------------------------------------- lookup

def parse_hex(s):
    try:
        return int(s.replace('0x', '').replace('0X', ''), 16)
    except ValueError:
        return None


def find_function(m, report, unit_arg, func_arg):
    """-> (unit, report entry or None, map function, report function record or None)."""
    units = {u['name']: u for u in m['units']}
    addr = parse_hex(func_arg) if func_arg else None
    if unit_arg not in units:
        addr = parse_hex(unit_arg)
        if addr is None:
            sys.exit(f'unit {unit_arg} is not in {C.rel(m["path"])}')
        unit = next((u for u in m['units'] if u['text'] and u['text'][0] <= addr < u['text'][1]), None)
        if unit is None:
            sys.exit(f'no unit covers {addr:08X}')
    else:
        unit = units[unit_arg]
    if addr is not None:
        f = next((f for f in unit['functions'] if f['start'] <= addr < f['end']), None)
        if f is None:
            sys.exit(f'{unit["name"]}: no function covers {addr:08X}')
    else:
        f = next((f for f in unit['functions'] if f['name'] == func_arg or f['symbol'] == func_arg
                  or '_' + f['name'] == func_arg), None)
        if f is None:
            sys.exit(f'{unit["name"]}: no function {func_arg} in the map '
                     f'(have: {", ".join(x["name"] for x in unit["functions"][:12])}...)')
    entry = next((e for e in (report or {}).get('units', []) if e['name'] == unit['name']), None)
    rec = next((r for r in (entry or {}).get('functions', []) if r['name'] == f['name']), None)
    return unit, entry, f, rec


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('unit', help='unit name, or an address when no function is given')
    ap.add_argument('function', nargs='?', help='function name or an address inside it')
    ap.add_argument('--map', help='unit map (default map/units.json)')
    ap.add_argument('--report', default=os.path.join(C.BUILD, 'report.json'))
    ap.add_argument('--abs', help='linked .abs (default build/link/<unit>.abs)')
    ap.add_argument('--linked-at', type=parse_hex, help='override the linked address of the function')
    ap.add_argument('--context', type=int, help='show only differing words with N lines of context')
    ap.add_argument('--no-pool', action='store_true', help='do not list the pool after the function')
    args = ap.parse_args()

    m = C.load_map(args.map)
    report = None
    if os.path.isfile(args.report):
        import json
        report = json.load(open(args.report))
        if report.get('map_sha256') != m['sha256']:
            print(f'[fncmp] note: {C.rel(args.report)} was made with another map; linked addresses may be stale')
    unit, entry, f, rec = find_function(m, report, args.unit, args.function)
    abs_path = args.abs or os.path.join(C.LINK_DIR, unit['name'] + '.abs')
    if not os.path.isfile(abs_path):
        sys.exit(f'{abs_path} missing: build the unit first (python -B tools/dc_build.py --unit {unit["name"]})')
    sections, image = abs_image(abs_path)
    names = Names(m)
    t0, t1 = unit['text']
    pool_words = {a: 2 for p0, p1 in unit['pools'] for a in range(p0, p1, 2)}
    target_lits = scan_literals(target_get, [(t0, t1)], pool_words)
    linked_lits = scan_literals(image.get, [(a, a + l) for n, a, l in sections if n == 'P'])

    start, end = f['start'], f['end']
    linked_at = args.linked_at
    linked_size = None
    note = ''
    if linked_at is None and rec and rec.get('linked_at'):
        linked_at = int(rec['linked_at'], 16)
        linked_size = rec.get('linked_size')
    if linked_at is None:
        linked_at = start
        note = ' (linked address assumed = target start: symbol not in the compiled object or unit not built)'
    if linked_size is None:
        linked_size = end - start

    # pool after the function (target side): report pool segment or map pool starting at `end`
    pool_end = end
    if not args.no_pool:
        segs = [(int(s['start'], 16), int(s['end'], 16)) for s in (entry or {}).get('segments', []) if s['kind'] == 'pool']
        segs += unit['pools']
        for p0, p1 in sorted(segs):
            if p0 == pool_end:
                pool_end = p1
    t_len = pool_end - start
    l_len = max(linked_size, t_len)
    status = rec['status'] if rec else 'not in report'
    diffb = f', {rec["diff_bytes"]} bytes differ' if rec and 'diff_bytes' in rec else ''
    header = (f'{unit["name"]}: {f["name"]} [{status}{diffb}]  target {start:08X}..{end:08X} ({end - start:#x})'
              + (f' + pool ..{pool_end:08X}' if pool_end != end else '')
              + f'  linked {linked_at:08X} +{linked_size:#x}{note}')
    linked = make_side(image.get, linked_at, l_len, f'LINKED (build/link/{unit["name"]}.abs)')
    linked['body_end'] = linked_at + linked_size
    target = make_side(target_get, start, t_len, 'TARGET (1ST_READ.BIN)')
    render(linked, target, names, context=args.context, header=header,
           linked_lits=linked_lits, target_lits=target_lits)
    if entry is None:
        print(f'  [fncmp] {unit["name"]} has no entry in {C.rel(args.report)}; status unknown')
    return 0


if __name__ == '__main__':
    sys.exit(main())
