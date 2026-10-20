#!/usr/bin/env python3
"""Build every mapped unit that has source, link it at its real address and compare it with the
target image byte for byte (nothing masked, nothing fuzzy).

    python tools/dc_build.py                      # all units with src/<unit>.c
    python tools/dc_build.py --unit ll -v         # one unit, listing of the first differing function
    python tools/dc_build.py build/obj/ll.o       # objdiff's custom_make form (unit from the path)
    python tools/dc_build.py --map map/units.seed.json

Per unit (frozen configuration only, see BUILDING.md):
  1. compile src/<unit>.c with tools/shc_cc.py -v shc-v5.1r11 --no-default-flags -- -speed -noinline
     -> build/obj/<unit>.elf (rof2elf relocatable) + build/obj/<unit>.obj (SYSROF for LNK)
  2. link with tools/shc_link.py at the map's text start (START P), plus START for every data/BSS
     section the map places, DEFINE for every external the object leaves undefined
     (src/externs.txt first, then build/target/symbols.txt) -> build/link/<unit>.{abs,map,json}
  3. compare the linked image with the target over the map's ranges: each function, each pool or
     gap outside functions, each data range. A range is exact only if every byte was produced by
     this build and equals the target. Gaps inside a linked section that LNK leaves unwritten count
     as 0x00 (the linker's fill); bytes outside every linked section are "not built". Trailing
     alignment after a unit (< 4 bytes, not built) is reported as `align` and never counted.
  4. write build/obj/<unit>.o (the linked bytes as an ELF relocatable, objdiff's base side).
Results: build/report.json and a table. dc_split.py runs first (target objects, symbols.txt) and
objdiff.json is refreshed afterwards.
"""
import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
import tempfile
os.environ['PYTHONDONTWRITEBYTECODE'] = '1'
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dc_common as C  # noqa: E402
import dc_split  # noqa: E402
from sysrof_abs import read_abs  # noqa: E402

SHC_CC = os.path.join(C.TOOLS, 'shc_cc.py')
SHC_LINK = os.path.join(C.TOOLS, 'shc_link.py')
# The frozen configuration (pilot/config/toolchain.json, tools/shc_cc.cmd). Not configurable here.
FROZEN_VERSION = 'shc-v5.1r11'
FROZEN_ARGS = ['--no-default-flags', '--', '-speed', '-noinline']
REPORT = os.path.join(C.BUILD, 'report.json')
# Held while build/target is (re)split and while build/report.json / objdiff.json are read,
# merged and rewritten, so concurrent `--unit` builds by several workers never drop each
# other's results. Compiling, linking and comparing run outside the lock.
REPORT_LOCK = os.path.join(C.BUILD, 'report.lock')


# ---------------------------------------------------------------- steps

def run(cmd, timeout=600):
    r = subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)
    return r.returncode, (r.stdout + r.stderr).strip()


INCLUDE_RE = re.compile(r'^\s*#\s*include\s*"([^"]+)"')


def expand_includes(path, depth=0, seen_stack=()):
    """Inline quoted #include files: absolute paths as written (the include/README.md form
    "C:\\rush2049-dreamcast\\include\\dc_types.h"), else next to the including file, then src/include,
    then include/. SHC's own -include= rejects any path containing '-' (C:/rush2049-dreamcast),
    shc_cc.py compiles a copy of the source in a temp dir, and the WSL fallback cannot open Windows
    paths, so headers are resolved here instead; #line keeps diagnostics pointing at the real files.
    Angle-bracket includes are left to the compiler. Inlining is what the preprocessor does anyway,
    so the generated code is the same."""
    if depth > 32 or path in seen_stack:
        raise ValueError(f'include cycle or nesting too deep at {path}')
    out = []
    lines = open(path, encoding='latin-1').read().splitlines()
    for n, line in enumerate(lines, 1):
        m = INCLUDE_RE.match(line)
        if not m:
            out.append(line)
            continue
        cands = [os.path.join(d, m.group(1))
                 for d in (os.path.dirname(path), os.path.join(C.SRC, 'include'), os.path.join(C.DC, 'include'))]
        inc = next((c for c in cands if os.path.isfile(c)), None)
        if inc is None:
            raise ValueError(f'{C.rel(path)}:{n}: include file "{m.group(1)}" not found')
        out.append(f'#line 1 "{os.path.basename(inc)}"')
        out.append(expand_includes(os.path.abspath(inc), depth + 1, seen_stack + (path,)))
        out.append(f'#line {n + 1} "{os.path.basename(path)}"')
    return '\n'.join(out)


def compile_unit(name, src):
    elf = os.path.join(C.OBJ_DIR, name + '.elf')
    obj = os.path.join(C.OBJ_DIR, name + '.obj')
    for p in (elf, obj):
        if os.path.exists(p):
            os.remove(p)
    text = open(src, encoding='latin-1').read()
    if any(INCLUDE_RE.match(l) for l in text.splitlines()):
        try:
            flat = expand_includes(os.path.abspath(src))
        except ValueError as e:
            return False, str(e), elf, obj
        src = os.path.join(C.BUILD, 'pp', name + '.c')
        os.makedirs(os.path.dirname(src), exist_ok=True)
        with open(src, 'w', encoding='latin-1', newline='\n') as f:
            f.write(flat + '\n')
    cmd = [sys.executable, SHC_CC, '-v', FROZEN_VERSION, src, '-o', elf, '--keep-obj', obj] + FROZEN_ARGS
    rc, out = run(cmd)
    ok = rc == 0 and os.path.isfile(elf) and os.path.isfile(obj)
    return ok, out, elf, obj


def link_unit(name, obj, starts, defines):
    for ext in ('.abs', '.map', '.json'):
        p = os.path.join(C.LINK_DIR, name + ext)
        if os.path.exists(p):
            os.remove(p)
    cmd = [sys.executable, SHC_LINK, '--obj', obj, '--name', name, '--out-dir', C.LINK_DIR]
    for sec, addr in starts:
        cmd += ['--start', f'{sec}={addr:08X}']
    for sym, addr in defines:
        cmd += ['--define', f'{sym}={addr:08X}']
    rc, out = run(cmd)
    abs_path = os.path.join(C.LINK_DIR, name + '.abs')
    ok = os.path.isfile(abs_path) and os.path.isfile(os.path.join(C.LINK_DIR, name + '.json'))
    return ok, out, abs_path


def built_image(abs_path):
    """{addr: byte} for every byte inside a linked section that has contents (unwritten gaps = 0x00),
    plus the section list [(name, start, length, has_data)]."""
    sections, runs = read_abs(abs_path)
    with_data = {sec for sec, _a, _c in runs}
    image = {}
    out_secs = []
    for i, (sname, addr, length) in enumerate(sections):
        out_secs.append((sname, addr, length, i in with_data))
        if i in with_data:
            for a in range(addr, addr + length):
                image[a] = 0
    for _sec, addr, chunk in runs:
        for k, b in enumerate(chunk):
            image[addr + k] = b
    return out_secs, image


def compare_range(image, start, end):
    """(exact, built_count, diff_positions) for [start, end) against the target."""
    want = C.target_bytes(start, end)
    built = 0
    diff = 0
    for k in range(end - start):
        b = image.get(start + k)
        if b is not None:
            built += 1
        if want is None or b is None or b != want[k]:
            diff += 1
    return diff == 0, built, diff


def produced_credit(produced, start, end, exact, padding=()):
    """Earn only explicitly emitted bytes in an entirely exact accepted range.
    Linker zero fill remains part of image equality, but earns no source credit.
    """
    return sum(a in produced and not any(p['start'] <= a < p['end'] for p in padding)
               for a in range(start, end)) if exact else 0


def interval_union_size(ranges):
    end = None
    size = 0
    for a, b in sorted(ranges):
        size += max(0, b - max(a, end if end is not None else a))
        end = max(b, end if end is not None else b)
    return size


def text_segments(unit):
    """Partition the text range into function / pool / gap segments (functions own inner pools)."""
    t0, t1 = unit['text']
    owner = [None] * (t1 - t0)
    for i, f in enumerate(unit['functions']):
        for a in range(max(f['start'], t0), min(f['end'], t1)):
            if owner[a - t0] is None:
                owner[a - t0] = ('func', i)
    for j, (p0, p1) in enumerate(unit['pools']):
        for a in range(max(p0, t0), min(p1, t1)):
            if owner[a - t0] is None:
                owner[a - t0] = ('pool', j)
    segs = []
    k = 0
    while k < len(owner):
        o = owner[k]
        e = k
        while e < len(owner) and owner[e] == o:
            e += 1
        if o is None:
            segs.append({'kind': 'gap', 'name': None, 'start': t0 + k, 'end': t0 + e})
        elif o[0] == 'func':
            segs.append({'kind': 'func', 'name': unit['functions'][o[1]]['name'], 'start': t0 + k, 'end': t0 + e})
        else:
            segs.append({'kind': 'pool', 'name': None, 'start': t0 + k, 'end': t0 + e})
        k = e
    return segs


def elf_text_symbols(elf_syms):
    """{symbol: offset} of functions/labels defined in the compiled object's .text."""
    return {s['name']: s['value'] for s in elf_syms
            if s['section'] == '.text' and s['type'] in (C.STT_FUNC, C.STT_NOTYPE, C.STT_OBJECT) and s['name']}


def write_base_object(name, sections, image, elf_syms):
    """build/obj/<unit>.o: the linked sections as an ELF relocatable (objdiff base side)."""
    out_secs, syms = [], []
    letter_to_index = {}
    for sname, addr, length, has_data in sections:
        ename = C.SECTION_ELF_NAME.get(sname, '.' + sname.lower())
        if has_data:
            data = bytes(image.get(a, 0) for a in range(addr, addr + length))
            out_secs.append({'name': ename, 'data': data, 'align': 4})
        else:
            out_secs.append({'name': ename, 'data': None, 'size': length, 'align': 4})
        letter_to_index[ename] = (len(out_secs) - 1, addr, length, has_data)
    by_sec = {}
    for s in elf_syms:
        if s['name'] and s['section'] in letter_to_index and s['type'] in (C.STT_FUNC, C.STT_OBJECT, C.STT_NOTYPE):
            by_sec.setdefault(s['section'], []).append(s)
    for ename, lst in by_sec.items():
        idx, addr, length, has_data = letter_to_index[ename]
        end = length
        if has_data:  # stop the last symbol at the last byte LNK wrote (trailing alignment excluded)
            while end > 0 and (addr + end - 1) not in image:
                end -= 1
        lst.sort(key=lambda s: s['value'])
        for k, s in enumerate(lst):
            nxt = lst[k + 1]['value'] if k + 1 < len(lst) else end
            stype = s['type'] if s['type'] != C.STT_NOTYPE else (C.STT_FUNC if ename == '.text' else C.STT_OBJECT)
            syms.append({'name': s['name'], 'section': idx, 'value': s['value'], 'size': max(nxt - s['value'], 0),
                         'type': stype, 'bind': C.STB_GLOBAL})
    C.write_elf(os.path.join(C.OBJ_DIR, name + '.o'), out_secs, syms, file_name=name + '.c')


# ---------------------------------------------------------------- one unit

def build_unit(unit, symbols, externs):
    name = unit['name']
    src = os.path.join(C.SRC, name + '.c')
    t0, t1 = unit['text']
    entry = {'name': name, 'kind': unit['kind'], 'status': 'failed', 'source': C.rel(src),
             'source_sha256': C.sha256_file(src), 'text': [f'{t0:08X}', f'{t1:08X}'], 'text_bytes': t1 - t0,
             'exact_text_bytes': 0, 'align_bytes': 0,
             'data_bytes': sum(d['end'] - d['start'] for d in unit['data']
                               if C.target_bytes(d['start'], d['end']) is not None),
             'exact_data_bytes': 0, 'functions_total': len(unit['functions']), 'functions_exact': 0,
             'functions_moved': 0, 'functions': [], 'segments': [], 'notes': [], 'built_at': time.strftime('%Y-%m-%dT%H:%M:%S')}
    t_start = time.time()
    ok, out, elf, obj = compile_unit(name, src)
    if not ok:
        entry['notes'].append('compile failed: ' + out[-600:])
        return entry
    _secs, elf_syms = C.read_elf(elf)
    defined = {s['name'] for s in elf_syms if s['shndx'] != 0 and s['name']}
    undefined = sorted({s['name'] for s in elf_syms
                        if s['shndx'] == 0 and s['name'] and s['type'] not in (C.STT_FILE, C.STT_SECTION)} - defined)
    defines, unresolved = [], []
    for sym in undefined:
        if sym in externs:
            defines.append((sym, externs[sym]))
        elif sym in symbols:
            defines.append((sym, symbols[sym]))
        else:
            unresolved.append(sym)
    entry['defines'] = {s: f'{a:08X}' for s, a in defines}
    if unresolved:
        entry['unresolved'] = unresolved
        entry['notes'].append('unresolved externals (add them to the map or src/externs.txt): ' + ', '.join(unresolved))
    obj_letters = {C.ELF_SECTION_LETTER.get(s['name']) for s in _secs if s['name'] in C.ELF_SECTION_LETTER}
    starts = [('P', t0)]
    placed = {'P'}
    for d in unit['data'] + unit['bss']:
        sec = d['section']
        if sec in placed:
            continue
        if sec not in obj_letters:
            entry['notes'].append(f'map places section {sec} but the object has none')
            continue
        starts.append((sec, min(x['start'] for x in unit['data'] + unit['bss'] if x['section'] == sec)))
        placed.add(sec)
    for sec in sorted(l for l in obj_letters if l and l not in placed):
        entry['notes'].append(f'object section {sec} has no address in the map (LNK default placement; not compared)')
    entry['starts'] = {s: f'{a:08X}' for s, a in starts}
    ok, out, abs_path = link_unit(name, obj, starts, defines)
    if not ok:
        entry['notes'].append('link failed: ' + out[-600:])
        return entry
    sections, image = built_image(abs_path)
    _raw_sections, raw_runs = read_abs(abs_path)
    produced = {addr + k for _sec, addr, chunk in raw_runs for k in range(len(chunk))}
    entry['sections'] = [{'name': n, 'address': f'{a:08X}', 'length': l} for n, a, l, _h in sections]
    write_base_object(name, sections, image, elf_syms)

    # ---- text: functions, pools, gaps
    exact_text = 0
    equal_text_including_fill = 0
    all_exact = True
    segs = text_segments(unit)
    for k, seg in enumerate(segs):
        ex, built, diff = compare_range(image, seg['start'], seg['end'])
        size = seg['end'] - seg['start']
        if (seg['kind'] == 'gap' and k == len(segs) - 1 and built == 0 and size < 4
                and set(C.target_bytes(seg['start'], seg['end']) or b'\1') <= {0}):
            seg['kind'] = 'align'
            entry['align_bytes'] += size
            ex = None
        seg.update({'exact': ex, 'built': built, 'diff_bytes': diff})
        if ex:
            equal_text_including_fill += size
            if seg['kind'] != 'gap':
                exact_text += produced_credit(produced, seg['start'], seg['end'], ex)
        elif ex is False:
            all_exact = False
    entry['exact_text_bytes'] = exact_text
    entry['equal_text_bytes_including_fill'] = equal_text_including_fill
    entry['excluded_exact_text_fill_or_gaps'] = equal_text_including_fill - exact_text

    # ---- functions (map boundaries; "moved" = identical bytes linked at another address)
    p_sec = next(((a, l) for n, a, l, _h in sections if n == 'P'), None)
    our_off = elf_text_symbols(elf_syms)
    our_sorted = sorted(our_off.values())
    for f in unit['functions']:
        ex, built, diff = compare_range(image, f['start'], f['end'])
        rec = {'name': f['name'], 'start': f'{f["start"]:08X}', 'end': f'{f["end"]:08X}',
               'size': f['end'] - f['start'], 'status': 'exact' if ex else ('missing' if built == 0 else 'differs'),
               'diff_bytes': diff}
        if p_sec and f['symbol'] in our_off:
            ours = p_sec[0] + our_off[f['symbol']]
            rec['linked_at'] = f'{ours:08X}'
            nxt = next((o for o in our_sorted if o > our_off[f['symbol']]), None)
            rec['linked_size'] = ((p_sec[0] + nxt) if nxt is not None else (p_sec[0] + p_sec[1])) - ours
            if not ex and ours != f['start']:
                want = C.target_bytes(f['start'], f['end'])
                got = [image.get(ours + i) for i in range(f['end'] - f['start'])]
                if want is not None and all(g == w for g, w in zip(got, want)):
                    rec['status'] = 'moved'
        elif built and not ex:
            rec['note'] = 'symbol not in the compiled object'
        entry['functions'].append(rec)
    entry['functions_exact'] = sum(1 for r in entry['functions'] if r['status'] == 'exact')
    entry['functions_moved'] = sum(1 for r in entry['functions'] if r['status'] == 'moved')
    entry['segments'] = [{'kind': s['kind'], 'start': f'{s["start"]:08X}', 'end': f'{s["end"]:08X}',
                          'exact': s['exact'], 'built': s['built'], 'diff_bytes': s['diff_bytes']}
                         for s in segs if s['kind'] != 'func']

    # ---- data ranges
    exact_data = 0
    equal_data_including_fill = 0
    for d in unit['data']:
        if C.target_bytes(d['start'], d['end']) is None:
            entry['notes'].append(f'{d["section"]} range {d["start"]:08X}..{d["end"]:08X} outside the image; not compared')
            continue
        ex, built, diff = compare_range(image, d['start'], d['end'])
        entry['segments'].append({'kind': 'data', 'section': d['section'], 'start': f'{d["start"]:08X}',
                                  'end': f'{d["end"]:08X}', 'exact': ex, 'built': built, 'diff_bytes': diff})
        if ex:
            equal_data_including_fill += d['end'] - d['start']
            exact_data += produced_credit(produced, d['start'], d['end'], ex, unit.get('data_padding', []))
        else:
            all_exact = False
    entry['exact_data_bytes'] = exact_data
    entry['equal_data_bytes_including_fill'] = equal_data_including_fill
    entry['excluded_exact_data_fill'] = equal_data_including_fill - exact_data
    entry['excluded_data_padding'] = [dict(start=f'{p["start"]:08X}', end=f'{p["end"]:08X}', reason=p['reason'])
                                      for p in unit.get('data_padding', [])]

    # ---- BSS placement (no bytes; checked by extent)
    for sec in sorted({b['section'] for b in unit['bss']}):
        ranges = [b for b in unit['bss'] if b['section'] == sec]
        m0, m1 = min(b['start'] for b in ranges), max(b['end'] for b in ranges)
        linked = next(((a, l) for n, a, l, _h in sections if n == sec), None)
        ok_b = linked is not None and linked[0] == m0 and linked[0] + linked[1] == m1
        entry['segments'].append({'kind': 'bss', 'section': sec, 'start': f'{m0:08X}', 'end': f'{m1:08X}',
                                  'exact': ok_b, 'linked': [f'{linked[0]:08X}', f'{linked[0] + linked[1]:08X}'] if linked else None})
        if not ok_b:
            all_exact = False
            entry['notes'].append(f'{sec} extent differs from the map')

    # ---- bytes built outside the unit's mapped ranges
    mapped = [(t0, t1)] + [(d['start'], d['end']) for d in unit['data']]
    outside = [a for a in image if not any(lo <= a < hi for lo, hi in mapped)]
    if outside:
        all_exact = False
        entry['notes'].append(f'{len(outside)} built bytes fall outside the mapped ranges '
                              f'(first {min(outside):08X}, last {max(outside):08X})')
    if p_sec:
        entry['text_size_delta'] = p_sec[1] - (C.text_extent_end(unit) - t0)
    if unresolved:
        all_exact = False
    entry['status'] = 'exact' if all_exact else 'partial'
    entry['seconds'] = round(time.time() - t_start, 1)
    return entry


# ---------------------------------------------------------------- verbose listing

def listing(unit, entry, max_lines, function=None):
    sys.path.insert(0, os.path.join(C.DC, 'calib'))
    from sh4dis import disasm_words  # noqa: E402  (capstone; run without python -I)
    if function:
        rec = next((r for r in entry['functions'] if r['name'] == function or '_' + r['name'] == function), None)
        if rec is None:
            print(f'\n  {unit["name"]}: no function {function} in the map')
            return
    else:
        rec = next((r for r in entry['functions'] if r['status'] != 'exact'), None)
    if rec is None:
        seg = next((s for s in entry['segments'] if s.get('exact') is False and s['kind'] in ('pool', 'gap', 'data')), None)
        if seg is None:
            return
        rec = {'name': f'{seg["kind"]} {seg["start"]}', 'start': seg['start'], 'end': seg['end'], 'status': 'differs'}
    start, end = int(rec['start'], 16), int(rec['end'], 16)
    ours = int(rec.get('linked_at', rec['start']), 16)
    our_len = rec.get('linked_size', end - start)
    abs_path = os.path.join(C.LINK_DIR, unit['name'] + '.abs')
    _s, image = built_image(abs_path)
    mine = bytes(image.get(ours + i, 0) for i in range(our_len))
    built = [image.get(ours + i) is not None for i in range(our_len)]
    want = C.target_bytes(start, end) or b''
    print(f'\n  {unit["name"]}: {rec["name"]} [{rec["status"]}]  ours {ours:08X} +{our_len:#x}  '
          f'target {start:08X} +{end - start:#x}')
    ml = list(disasm_words(mine, ours))
    wl = list(disasm_words(want, start))
    shown = 0
    for i in range(max(len(ml), len(wl))):
        a = ml[i] if i < len(ml) else None
        b = wl[i] if i < len(wl) else None
        a_txt = (f'{a[0]:08x} {a[1].hex() if built[2 * i] else "----"} {a[2]}') if a else ''
        b_txt = (f'{b[0]:08x} {b[1].hex()} {b[2]}') if b else ''
        mark = ' ' if (a and b and a[1] == b[1] and built[2 * i]) else '*'
        print(f'  {mark} {a_txt:44s} | {b_txt}')
        shown += 1
        if shown >= max_lines:
            print('    ... (use --lines N for more)')
            break


# ---------------------------------------------------------------- report

def summarize(m, entries):
    units = m['units']
    game = [u for u in units if u['kind'] == 'game' and u['text']]
    fresh = {n: e for n, e in entries.items() if e.get('status') in ('exact', 'partial')}
    game_total = sum(u['text'][1] - u['text'][0] for u in game)
    game_exact = sum(fresh[u['name']]['exact_text_bytes'] for u in game if u['name'] in fresh)
    image_exact = sum(e['exact_text_bytes'] + e['exact_data_bytes'] for e in fresh.values())
    mapped_text = sum(u['text'][1] - u['text'][0] for u in units if u['text'])
    mapped_data = sum(d['end'] - d['start'] for u in units for d in u['data']
                      if C.target_bytes(d['start'], d['end']) is not None)
    mapped_unique = interval_union_size([u['text'] for u in units if u['text']] +
                                       [(d['start'], d['end']) for u in units for d in u['data']
                                        if C.target_bytes(d['start'], d['end']) is not None])
    funcs_total = sum(len(u['functions']) for u in units)
    game_funcs_total = sum(len(u['functions']) for u in game)
    status_count = {}
    for e in entries.values():
        status_count[e['status']] = status_count.get(e['status'], 0) + 1
    pct = lambda a, b: round(100.0 * a / b, 4) if b else 0.0
    return {
        'game_code': {'exact_bytes': game_exact, 'total_bytes': game_total, 'percent': pct(game_exact, game_total)},
        'image': {'exact_bytes': image_exact, 'total_bytes': m['image_size'], 'percent': pct(image_exact, m['image_size'])},
        'map_coverage': {'text_bytes': mapped_text, 'data_bytes': mapped_data, 'unique_file_bytes': mapped_unique,
                         'image_bytes': m['image_size'], 'percent': pct(mapped_unique, m['image_size'])},
        'functions': {'exact': sum(e['functions_exact'] for e in fresh.values()), 'total': funcs_total,
                      'game_exact': sum(fresh[u['name']]['functions_exact'] for u in game if u['name'] in fresh),
                      'game_total': game_funcs_total},
        'units': {'mapped': len(units), 'game': len(game), 'with_source': len(entries), **status_count},
    }


def print_table(m, entries, summary, verbose_units=()):
    print(f'\n{"unit":22s} {"kind":8s} {"status":8s} {"text exact/total":>18s} {"align":>5s} {"data":>11s} '
          f'{"funcs":>9s}  notes')
    for u in m['units']:
        e = entries.get(u['name'])
        if e is None:
            continue
        funcs = f'{e.get("functions_exact", 0)}/{e.get("functions_total", 0)}'
        if e.get('functions_moved'):
            funcs += f' +{e["functions_moved"]}m'
        text = f'{e.get("exact_text_bytes", 0)}/{e.get("text_bytes", 0)}'
        data = f'{e.get("exact_data_bytes", 0)}/{e.get("data_bytes", 0)}' if e.get('data_bytes') else '-'
        notes = '; '.join(e.get('notes', []))[:90]
        if e.get('text_size_delta'):
            notes = f'size {e["text_size_delta"]:+d}' + ('; ' + notes if notes else '')
        print(f'{u["name"]:22s} {u["kind"]:8s} {e["status"]:8s} {text:>18s} {e.get("align_bytes", 0):5d} {data:>11s} '
              f'{funcs:>9s}  {notes}')
    g, im, cov, fn = summary['game_code'], summary['image'], summary['map_coverage'], summary['functions']
    print(f'\ngame code : {g["exact_bytes"]} / {g["total_bytes"]} bytes exact ({g["percent"]:.4f}%) over '
          f'{summary["units"]["game"]} mapped game units')
    print(f'whole image: {im["exact_bytes"]} / {im["total_bytes"]} bytes exact ({im["percent"]:.4f}%)')
    print(f'functions : {fn["exact"]} / {fn["total"]} exact (game {fn["game_exact"]} / {fn["game_total"]}); '
          f'map covers {cov["percent"]:.2f}% of the image')


def split_is_current(m):
    """True when build/target holds dc_split.py's complete output for this map, target and
    split tool version, so the split can be skipped (its output is a pure function of those)."""
    try:
        s = json.load(open(os.path.join(C.TARGET_DIR, 'split.json')))
    except (OSError, ValueError):
        return False
    if (s.get('map_sha256') != m['sha256'] or s.get('target_sha256') != C.TARGET_SHA256
            or s.get('tool_sha256') != dc_split.tool_sha256() or not os.path.isfile(C.SYMBOLS_TXT)):
        return False
    return all(os.path.isfile(os.path.join(C.DC, w['path'])) for w in s.get('units', []))


def previous_entries(m, srcs, buildable):
    """Results in build/report.json that are still valid: same map, and the unit's source
    unchanged since it was built. Called under REPORT_LOCK right before the merge, so results
    other workers wrote in the meantime are kept."""
    entries = {}
    if os.path.isfile(REPORT):
        try:
            old = json.load(open(REPORT))
            if old.get('map_sha256') == m['sha256']:
                for e in old.get('units', []):
                    src = os.path.join(C.SRC, e['name'] + '.c')
                    if e['name'] in srcs and os.path.isfile(src) and C.sha256_file(src) == e.get('source_sha256'):
                        entries[e['name']] = e
        except (ValueError, KeyError, OSError):
            pass
    names = {u['name'] for u in buildable}
    return {n: e for n, e in entries.items() if n in names}


def unit_names_from_args(args):
    names = []
    for a in (args.unit or []) + (args.targets or []):
        for part in a.split(','):
            part = part.strip()
            if not part:
                continue
            if part.endswith(('.o', '.c', '.elf', '.obj')) or '/' in part or '\\' in part:
                part = os.path.splitext(os.path.basename(part))[0]
            names.append(part)
    return names


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('targets', nargs='*', help='unit names or object paths (build/obj/<unit>.o)')
    ap.add_argument('--map', help='unit map (default map/units.json, else map/units.seed.json)')
    ap.add_argument('-u', '--unit', action='append', help='build only this unit (repeatable, comma lists ok)')
    ap.add_argument('-v', '--verbose', action='store_true', help='side-by-side listing of the first differing function')
    ap.add_argument('--lines', type=int, default=80, help='listing lines per unit with --verbose (default 80)')
    ap.add_argument('-f', '--function', help='with --verbose: list this function instead of the first differing one')
    ap.add_argument('-j', '--jobs', type=int, default=4, help='units built in parallel (default 4)')
    ap.add_argument('--check', action='store_true', help='exit 1 unless every built unit is exact')
    ap.add_argument('--no-objdiff', action='store_true', help='do not refresh objdiff.json')
    ap.add_argument('--resplit', action='store_true', help='redo the target split even if build/target is current')
    args = ap.parse_args()

    os.makedirs(os.path.join(tempfile.gettempdir(), 'rush2049-dc'), exist_ok=True)
    for d in (C.OBJ_DIR, C.LINK_DIR):
        os.makedirs(d, exist_ok=True)
    m = C.load_map(args.map)
    with C.locked(REPORT_LOCK):
        if args.resplit or not split_is_current(m):
            m, _written = dc_split.split(args.map, quiet=True)
    symbols = C.read_symbol_file(C.SYMBOLS_TXT)
    externs = C.read_symbol_file(C.EXTERNS_TXT)
    for sym, addr in externs.items():
        if sym in symbols and symbols[sym] != addr:
            C.warn(f'src/externs.txt {sym}={addr:08X} overrides the map ({symbols[sym]:08X})')

    by_name = {u['name']: u for u in m['units']}
    srcs = {os.path.splitext(f)[0] for f in os.listdir(C.SRC) if f.endswith('.c')} if os.path.isdir(C.SRC) else set()
    for s in sorted(srcs - set(by_name)):
        C.warn(f'src/{s}.c has no unit in {C.rel(m["path"])}; not built')
    wanted = unit_names_from_args(args)
    for n in wanted:
        if n not in by_name:
            sys.exit(f'unit {n} is not in {C.rel(m["path"])}')
        if n not in srcs:
            sys.exit(f'unit {n} has no source src/{n}.c')
    buildable = [u for u in m['units'] if u['name'] in srcs and u['text']]
    todo = [u for u in buildable if not wanted or u['name'] in wanted]

    print(f'[dc_build] map {C.rel(m["path"])}: {len(m["units"])} units, {len(buildable)} with source; '
          f'building {len(todo)} with {FROZEN_VERSION} {" ".join(FROZEN_ARGS)}')
    built = {}
    with concurrent.futures.ThreadPoolExecutor(max_workers=max(1, args.jobs)) as ex:
        futs = {u['name']: ex.submit(build_unit, u, symbols, externs) for u in todo}
        for u in todo:
            try:
                built[u['name']] = futs[u['name']].result()
            except Exception as e:  # keep going; report the unit as failed
                built[u['name']] = {'name': u['name'], 'kind': u['kind'], 'status': 'failed',
                                    'notes': [f'{type(e).__name__}: {e}'], 'text_bytes': u['text'][1] - u['text'][0],
                                    'exact_text_bytes': 0, 'exact_data_bytes': 0, 'functions_total': len(u['functions']),
                                    'functions_exact': 0, 'functions_moved': 0}

    # merge into build/report.json under the lock: previous results for units not rebuilt now
    # (same map, same source only; re-read here so other workers' results are kept), then ours
    with C.locked(REPORT_LOCK):
        entries = previous_entries(m, srcs, buildable)
        entries.update(built)
        for u in buildable:
            if u['name'] not in entries:
                entries[u['name']] = {'name': u['name'], 'kind': u['kind'], 'status': 'unbuilt',
                                      'text_bytes': u['text'][1] - u['text'][0], 'exact_text_bytes': 0,
                                      'exact_data_bytes': 0, 'functions_total': len(u['functions']),
                                      'functions_exact': 0, 'functions_moved': 0, 'notes': ['not built yet with this map/source']}
        summary = summarize(m, entries)
        report = {'generated': time.strftime('%Y-%m-%dT%H:%M:%S'), 'map': C.rel(m['path']), 'map_sha256': m['sha256'],
                  'target_sha256': C.TARGET_SHA256, 'compiler': {'version': FROZEN_VERSION, 'args': FROZEN_ARGS},
                  'summary': summary, 'units': [entries[u['name']] for u in m['units'] if u['name'] in entries]}
        C.write_atomic(REPORT, json.dumps(report, indent=1))
        if not args.no_objdiff:
            import dc_objdiff
            dc_objdiff.write_config(m, report, quiet=True)
    print_table(m, entries, summary)
    print(f'report: {C.rel(REPORT)}')
    if args.verbose:
        for u in todo:
            e = entries[u['name']]
            if e['status'] == 'partial' or (args.function and e['status'] == 'exact'):
                listing(u, e, args.lines, args.function)
            elif e['status'] == 'failed':
                print(f'\n  {u["name"]}: FAILED\n    ' + '\n    '.join(e.get('notes', [])))
    failed = [u['name'] for u in todo if entries[u['name']]['status'] == 'failed']
    if failed:
        print(f'[dc_build] FAILED: {", ".join(failed)}', file=sys.stderr)
        return 1
    if args.check and any(entries[u['name']]['status'] != 'exact' for u in todo):
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
