"""Shared helpers for the dc_* build tools (dc_split.py, dc_build.py, dc_objdiff.py).

Standard library only. Holds the project paths, the unit-map loader (map/units.json schema, robust
to missing fields), a minimal ELF32 little-endian SH relocatable writer/reader, and the
symbols.txt / externs.txt formats.
"""
import contextlib
import hashlib
import json
import os
import struct
import sys
import time

TOOLS = os.path.dirname(os.path.abspath(__file__))
DC = os.path.dirname(TOOLS)
TARGET = os.path.join(DC, 'pilot', 'orig', '1ST_READ.BIN')
TARGET_SHA256 = '336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688'
TARGET_SIZE = 1123492
BASE = 0x8C010000
MAP_DIR = os.path.join(DC, 'map')
MAP_DEFAULT = os.path.join(MAP_DIR, 'units.json')
MAP_SEED = os.path.join(MAP_DIR, 'units.seed.json')
SRC = os.path.join(DC, 'src')
BUILD = os.path.join(DC, 'build')
TARGET_DIR = os.path.join(BUILD, 'target')
OBJ_DIR = os.path.join(BUILD, 'obj')
LINK_DIR = os.path.join(BUILD, 'link')
SYMBOLS_TXT = os.path.join(TARGET_DIR, 'symbols.txt')
EXTERNS_TXT = os.path.join(SRC, 'externs.txt')

# Hitachi section letters -> ELF section names (same table as rof2elf.py)
SECTION_ELF_NAME = {'P': '.text', 'C': '.rodata', 'D': '.data', 'B': '.bss'}
ELF_SECTION_LETTER = {v: k for k, v in SECTION_ELF_NAME.items()}


def warn(msg):
    print(f'[warn] {msg}', file=sys.stderr)


def sha256_bytes(b):
    return hashlib.sha256(b).hexdigest()


def sha256_file(path):
    with open(path, 'rb') as f:
        return sha256_bytes(f.read())


@contextlib.contextmanager
def locked(path, timeout=600.0):
    """Exclusive inter-process lock on the file `path` (created if missing). Blocks up to
    `timeout` seconds, then raises TimeoutError. Released when the block exits or the process
    dies, so a crashed worker never leaves a stale lock."""
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    fd = os.open(path, os.O_RDWR | os.O_CREAT)
    try:
        deadline = time.monotonic() + timeout
        while True:
            try:
                if os.name == 'nt':
                    import msvcrt
                    msvcrt.locking(fd, msvcrt.LK_NBLCK, 1)
                else:
                    import fcntl
                    fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
                break
            except OSError:
                if time.monotonic() > deadline:
                    raise TimeoutError(f'could not lock {path} within {timeout:.0f} s')
                time.sleep(0.05)
        yield
    finally:
        try:
            if os.name == 'nt':
                import msvcrt
                msvcrt.locking(fd, msvcrt.LK_UNLCK, 1)
            else:
                import fcntl
                fcntl.flock(fd, fcntl.LOCK_UN)
        except OSError:
            pass
        os.close(fd)


def write_atomic(path, text, newline=None, retries=40):
    """Write `text` to `path` through a temporary file and os.replace, so a reader never sees a
    half-written file. On Windows the replace is retried while another process holds the file
    open. `newline` is passed to open() (None = platform default, as a plain open(path, 'w'))."""
    tmp = f'{path}.{os.getpid()}.tmp'
    with open(tmp, 'w', newline=newline) as f:
        f.write(text)
    for i in range(retries):
        try:
            os.replace(tmp, path)
            return
        except PermissionError:
            time.sleep(0.025 * (i + 1))
    os.replace(tmp, path)


def rel(path):
    """Project-relative path with forward slashes (for reports and objdiff.json)."""
    try:
        return os.path.relpath(path, DC).replace('\\', '/')
    except ValueError:
        return path.replace('\\', '/')


# ---------------------------------------------------------------- target image

_target_cache = None


def load_target():
    """The pinned target image. Refuses to run against anything else."""
    global _target_cache
    if _target_cache is None:
        if not os.path.isfile(TARGET):
            sys.exit(f'target image missing: {TARGET}')
        data = open(TARGET, 'rb').read()
        if sha256_bytes(data) != TARGET_SHA256:
            sys.exit(f'target image {TARGET} does not have the pinned SHA-256 {TARGET_SHA256}')
        _target_cache = data
    return _target_cache


def target_bytes(start, end):
    """Target bytes for [start, end) in VA; None if any part lies outside the image."""
    data = load_target()
    lo, hi = start - BASE, end - BASE
    if lo < 0 or hi > len(data) or hi < lo:
        return None
    return data[lo:hi]


# ---------------------------------------------------------------- unit map

def parse_addr(v):
    if v is None:
        return None
    if isinstance(v, bool):
        raise ValueError(f'bad address {v!r}')
    if isinstance(v, int):
        return v
    s = str(v).strip().replace('_', '')
    if not s:
        return None
    if s.lower().startswith('0x'):
        return int(s, 16)
    return int(s, 16)  # the map writes hex; bare strings are hex too


def linker_symbol(name, explicit=None):
    """C name -> SHC linker symbol. An explicit 'symbol' field wins; names that already carry the
    double underscore of a runtime helper (__divls) are kept; everything else gets one '_'."""
    if explicit:
        return explicit
    if name.startswith('__'):
        return name
    return '_' + name


def _range(d, what, unit_name):
    """{'start','end'} or {'start','size'} -> (start, end) or None."""
    if not isinstance(d, dict):
        return None
    try:
        start = parse_addr(d.get('start', d.get('address')))
        end = parse_addr(d.get('end'))
        if end is None and d.get('size') is not None:
            size = d['size']
            end = start + (int(size, 0) if isinstance(size, str) else int(size))
    except (ValueError, TypeError) as e:
        warn(f'{unit_name}: bad {what} range {d!r}: {e}')
        return None
    if start is None:
        return None
    return start, end


def resolve_map_path(path=None):
    if path:
        return os.path.abspath(path)
    if os.path.isfile(MAP_DEFAULT):
        return MAP_DEFAULT
    return MAP_SEED


def load_map(path=None):
    """Load and normalise the unit map. Returns a dict:
    {path, sha256, base, image_size, units: [unit]} where every unit has
    name, kind, source, confidence, notes,
    text: (start, end) | None,
    functions: [{name, symbol, start, end, confidence}] sorted by start, ends filled in,
    pools: [(start, end)], data: [{section, start, end, symbols}], bss: [same],
    symbols: [{name, symbol, address, size, section}] (data/other symbols, functions excluded).
    """
    path = resolve_map_path(path)
    if not os.path.isfile(path):
        sys.exit(f'unit map not found: {path}')
    raw_bytes = open(path, 'rb').read()
    raw = json.loads(raw_bytes)
    out = {'path': path, 'sha256': sha256_bytes(raw_bytes),
           'base': parse_addr(raw.get('base')) or BASE,
           'image_size': int(raw.get('image_size') or TARGET_SIZE), 'units': []}
    if out['base'] != BASE:
        warn(f'map base {out["base"]:#x} differs from the image base {BASE:#x}; using the map value for nothing')
    seen = set()
    for i, u in enumerate(raw.get('units', [])):
        name = u.get('name')
        if not name:
            warn(f'map unit #{i} has no name; skipped')
            continue
        if name in seen:
            warn(f'duplicate unit name {name}; later entry skipped')
            continue
        seen.add(name)
        unit = {'name': name, 'kind': u.get('kind') or 'unknown', 'source': u.get('source'),
                'confidence': u.get('confidence'), 'notes': u.get('notes'),
                'text': None, 'functions': [], 'pools': [], 'data': [], 'bss': [], 'symbols': [],
                'data_padding': []}
        text = u.get('text')
        if isinstance(text, list):
            if len(text) > 1:
                warn(f'{name}: several text ranges given; only the first is used')
            text = text[0] if text else None
        r = _range(text, 'text', name)
        if r and r[1] is not None:
            unit['text'] = r
        funcs = []
        for f in u.get('functions') or []:
            fname = f.get('name')
            r = _range(f, 'function', name)
            if not fname or not r:
                warn(f'{name}: function entry without name/start skipped: {f!r}')
                continue
            funcs.append({'name': fname, 'symbol': linker_symbol(fname, f.get('symbol')),
                          'start': r[0], 'end': r[1], 'confidence': f.get('confidence'),
                          'mat3': f.get('mat3')})
        funcs.sort(key=lambda f: f['start'])
        for k, f in enumerate(funcs):
            if f['end'] is None:
                if k + 1 < len(funcs):
                    f['end'] = funcs[k + 1]['start']
                elif unit['text']:
                    f['end'] = unit['text'][1]
                else:
                    warn(f'{name}: function {f["name"]} has no end and nothing follows it; dropped')
        funcs = [f for f in funcs if f['end'] is not None and f['end'] > f['start']]
        unit['functions'] = funcs
        for p in u.get('pools') or []:
            r = _range(p, 'pool', name)
            if r and r[1] is not None and r[1] > r[0]:
                unit['pools'].append(r)
        for key, default_sec in (('data', 'D'), ('bss', 'B')):
            for d in u.get(key) or []:
                r = _range(d, key, name)
                if not r or r[1] is None or r[1] <= r[0]:
                    continue
                syms = []
                for s in d.get('symbols') or []:
                    sr = _range(s, 'symbol', name)
                    if s.get('name') and sr:
                        syms.append({'name': s['name'], 'symbol': linker_symbol(s['name'], s.get('symbol')),
                                     'address': sr[0], 'size': (sr[1] - sr[0]) if sr[1] is not None else None,
                                     'section': d.get('section') or default_sec})
                unit[key].append({'section': d.get('section') or default_sec, 'start': r[0], 'end': r[1],
                                  'symbols': syms})
                unit['symbols'] += syms
        for d in u.get('data_padding') or []:
            r = _range(d, 'data padding', name)
            if not r or r[1] is None or r[1] <= r[0] or not d.get('reason'):
                raise ValueError(f'{name}: data padding requires a range and evidence reason')
            if not any(x['start'] <= r[0] < r[1] <= x['end'] for x in unit['data']):
                raise ValueError(f'{name}: data padding is outside mapped initialized data')
            unit['data_padding'].append({'start': r[0], 'end': r[1], 'reason': d['reason']})
        for s in u.get('symbols') or []:
            sr = _range(s, 'symbol', name)
            if s.get('name') and sr:
                unit['symbols'].append({'name': s['name'], 'symbol': linker_symbol(s['name'], s.get('symbol')),
                                        'address': sr[0], 'size': (sr[1] - sr[0]) if sr[1] is not None else None,
                                        'section': s.get('section')})
        if unit['text'] is None and funcs:
            unit['text'] = (funcs[0]['start'], max(f['end'] for f in funcs))
            warn(f'{name}: no text range; using the function extent {unit["text"][0]:08X}..{unit["text"][1]:08X}')
        if unit['text']:
            for f in funcs:
                if f['start'] < unit['text'][0] or f['end'] > unit['text'][1]:
                    warn(f'{name}: function {f["name"]} {f["start"]:08X}..{f["end"]:08X} leaves the text range')
        out['units'].append(unit)
    spans = sorted((u['text'][0], u['text'][1], u['name']) for u in out['units'] if u['text'])
    for (a0, a1, an), (b0, b1, bn) in zip(spans, spans[1:]):
        if b0 < a1:
            warn(f'text ranges overlap: {an} {a0:08X}..{a1:08X} and {bn} {b0:08X}..{b1:08X}')
    return out


def text_extent_end(unit):
    """End of the last function or pool inside the text range (trailing alignment excluded)."""
    t0, t1 = unit['text']
    ends = [f['end'] for f in unit['functions']] + [p[1] for p in unit['pools']]
    ends = [e for e in ends if t0 < e <= t1]
    return max(ends) if ends else t1


def function_extents(unit):
    """[(function, start, end)] where end runs to the next function start (pools included);
    the last function ends at text_extent_end(). Used for ELF symbol sizes."""
    funcs = [f for f in unit['functions'] if unit['text'][0] <= f['start'] < unit['text'][1]]
    last = text_extent_end(unit)
    out = []
    for k, f in enumerate(funcs):
        end = funcs[k + 1]['start'] if k + 1 < len(funcs) else max(last, f['end'])
        out.append((f, f['start'], min(end, unit['text'][1])))
    return out


# ---------------------------------------------------------------- symbols.txt / externs.txt

def read_symbol_file(path):
    """'name addr' or 'name=addr' per line, '#' comments. Returns {linker_symbol: addr}."""
    out = {}
    if not os.path.isfile(path):
        return out
    for n, line in enumerate(open(path, encoding='utf-8'), 1):
        line = line.split('#', 1)[0].strip()
        if not line:
            continue
        parts = line.replace('=', ' ').split()
        if len(parts) < 2:
            warn(f'{path}:{n}: expected "name address": {line!r}')
            continue
        name = parts[0] if parts[0].startswith('_') else '_' + parts[0]
        try:
            out[name] = parse_addr(parts[1])
        except ValueError:
            warn(f'{path}:{n}: bad address {parts[1]!r}')
    return out


# ---------------------------------------------------------------- ELF32 LE SH relocatable

EM_SH = 42
EF_SH4 = 9
SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_NOBITS = 1, 2, 3, 8
SHF_WRITE, SHF_ALLOC, SHF_EXECINSTR = 1, 2, 4
STB_LOCAL, STB_GLOBAL = 0, 1
STT_NOTYPE, STT_OBJECT, STT_FUNC, STT_SECTION, STT_FILE = 0, 1, 2, 3, 4
SHN_ABS = 0xFFF1


def section_flags(elf_name):
    if elf_name == '.text':
        return SHF_ALLOC | SHF_EXECINSTR
    if elf_name in ('.data', '.bss') or elf_name.startswith(('.data', '.bss')):
        return SHF_ALLOC | SHF_WRITE
    return SHF_ALLOC


def write_elf(path, sections, symbols, file_name=None):
    """Write an ELF32 little-endian SH relocatable.
    sections: [{'name', 'data': bytes | None, 'size': int (for NOBITS), 'align': int}]
    symbols:  [{'name', 'section': index into sections, 'value', 'size', 'type', 'bind'}]
    Section symbols (and a FILE symbol) are added automatically."""
    shstr = bytearray(b'\0')
    strtab = bytearray(b'\0')

    def add_str(tab, s):
        off = len(tab)
        tab += s.encode() + b'\0'
        return off

    # section header indices: 0 null, 1..n user sections, then symtab, strtab, shstrtab
    nuser = len(sections)
    sym_entries = [(0, 0, 0, 0, 0, 0)]  # name, value, size, info, other, shndx
    if file_name:
        sym_entries.append((add_str(strtab, file_name), 0, 0, (STB_LOCAL << 4) | STT_FILE, 0, SHN_ABS))
    for i, sec in enumerate(sections):
        sym_entries.append((add_str(strtab, sec['name']), 0, 0, (STB_LOCAL << 4) | STT_SECTION, 0, i + 1))
    locals_ = [s for s in symbols if s.get('bind', STB_GLOBAL) == STB_LOCAL]
    globals_ = [s for s in symbols if s.get('bind', STB_GLOBAL) != STB_LOCAL]
    for s in locals_ + globals_:
        shndx = s['section'] + 1 if s.get('section') is not None else 0
        info = (s.get('bind', STB_GLOBAL) << 4) | s.get('type', STT_NOTYPE)
        sym_entries.append((add_str(strtab, s['name']), s.get('value', 0), s.get('size', 0) or 0, info, 0, shndx))
    first_global = len(sym_entries) - len(globals_)
    symtab = b''.join(struct.pack('<IIIBBH', *e) for e in sym_entries)

    body = bytearray()
    headers = [(0,) * 10]
    off = 52
    for sec in sections:
        name_off = add_str(shstr, sec['name'])
        align = sec.get('align', 4) or 1
        if sec.get('data') is None:
            headers.append((name_off, SHT_NOBITS, section_flags(sec['name']), 0, off + len(body),
                            sec.get('size', 0), 0, 0, align, 0))
            continue
        while (off + len(body)) % align:
            body += b'\0'
        headers.append((name_off, SHT_PROGBITS, section_flags(sec['name']), 0, off + len(body),
                        len(sec['data']), 0, 0, align, 0))
        body += sec['data']
    symtab_idx = len(headers)
    while (off + len(body)) % 4:
        body += b'\0'
    headers.append((add_str(shstr, '.symtab'), SHT_SYMTAB, 0, 0, off + len(body), len(symtab),
                    symtab_idx + 1, first_global, 4, 16))
    body += symtab
    headers.append((add_str(shstr, '.strtab'), SHT_STRTAB, 0, 0, off + len(body), len(strtab), 0, 0, 1, 0))
    body += strtab
    shstr_off_name = add_str(shstr, '.shstrtab')
    headers.append((shstr_off_name, SHT_STRTAB, 0, 0, off + len(body), len(shstr), 0, 0, 1, 0))
    body += shstr
    while (off + len(body)) % 4:
        body += b'\0'
    shoff = off + len(body)
    ehdr = b'\x7fELF' + bytes([1, 1, 1, 0]) + b'\0' * 8
    ehdr += struct.pack('<HHIIIIIHHHHHH', 1, EM_SH, 1, 0, 0, shoff, EF_SH4, 52, 0, 0, 40,
                        len(headers), len(headers) - 1)
    out = ehdr + body + b''.join(struct.pack('<IIIIIIIIII', *h) for h in headers)
    os.makedirs(os.path.dirname(os.path.abspath(path)), exist_ok=True)
    with open(path, 'wb') as f:
        f.write(out)


def read_elf(path):
    """Minimal ELF32 LE reader: returns (sections, symbols).
    sections: [{'index', 'name', 'type', 'data', 'size'}]
    symbols:  [{'name', 'value', 'size', 'bind', 'type', 'shndx', 'section' (name or None)}]"""
    d = open(path, 'rb').read()
    if d[:4] != b'\x7fELF' or d[4] != 1 or d[5] != 1:
        raise ValueError(f'{path}: not an ELF32 little-endian file')
    shoff = struct.unpack_from('<I', d, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    raw = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    names = d[raw[shstrndx][4]:raw[shstrndx][4] + raw[shstrndx][5]]
    nm = lambda tab, o: tab[o:tab.index(b'\0', o)].decode(errors='replace')
    sections = []
    for i, s in enumerate(raw):
        sections.append({'index': i, 'name': nm(names, s[0]), 'type': s[1], 'size': s[5],
                         'data': d[s[4]:s[4] + s[5]] if s[1] != SHT_NOBITS else None})
    symbols = []
    for s in raw:
        if s[1] != SHT_SYMTAB:
            continue
        strs = d[raw[s[6]][4]:raw[s[6]][4] + raw[s[6]][5]]
        for i in range(1, s[5] // 16):
            n, v, sz, info, _o, sh = struct.unpack_from('<IIIBBH', d, s[4] + i * 16)
            symbols.append({'name': nm(strs, n), 'value': v, 'size': sz, 'bind': info >> 4, 'type': info & 15,
                            'shndx': sh,
                            'section': sections[sh]['name'] if 0 < sh < len(sections) else None})
    return sections, symbols
