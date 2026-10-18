"""Shared helpers for the Rush 2049 DC unit map (read-only on the target)."""
import os, sys, struct
ROOT = r'C:\rush2049-dreamcast'
MAP = os.path.join(ROOT, 'map')
RAW = os.path.join(MAP, 'raw')
sys.path.insert(0, os.path.join(ROOT, 'calib'))
BIN = os.path.join(ROOT, 'pilot', 'orig', '1ST_READ.BIN')
BASE = 0x8C010000
DATA = open(BIN, 'rb').read()
SIZE = len(DATA)
END = BASE + SIZE


def inimg(va):
    return BASE <= va < END


def w16(va):
    o = va - BASE
    return DATA[o] | (DATA[o + 1] << 8)


def w32(va):
    return struct.unpack_from('<I', DATA, va - BASE)[0]


def s8(x):
    return x - 256 if x & 0x80 else x


def s12(x):
    return x - 4096 if x & 0x800 else x


def classify(va):
    """Flow class of the instruction at va: returns (kind, target or None).
    kinds: rts, rte, bra, bsr, bt, bf, bts, bfs, jmp, jsr, braf, bsrf, ldl (mov.l pc), ldw (mov.w pc),
    mova, other."""
    w = w16(va)
    hi = w >> 12
    if w == 0x000B:
        return 'rts', None
    if w == 0x002B:
        return 'rte', None
    if hi == 0xA:
        return 'bra', va + 4 + 2 * s12(w & 0xFFF)
    if hi == 0xB:
        return 'bsr', va + 4 + 2 * s12(w & 0xFFF)
    top = w >> 8
    if top in (0x89, 0x8B, 0x8D, 0x8F):
        t = va + 4 + 2 * s8(w & 0xFF)
        return {0x89: 'bt', 0x8B: 'bf', 0x8D: 'bts', 0x8F: 'bfs'}[top], t
    if (w & 0xF0FF) == 0x402B:
        return 'jmp', None
    if (w & 0xF0FF) == 0x400B:
        return 'jsr', None
    if (w & 0xF0FF) == 0x0023:
        return 'braf', None
    if (w & 0xF0FF) == 0x0003:
        return 'bsrf', None
    if hi == 0xD:
        return 'ldl', (va & ~3) + 4 + 4 * (w & 0xFF)
    if hi == 0x9:
        return 'ldw', va + 4 + 2 * (w & 0xFF)
    if top == 0xC7:
        return 'mova', (va & ~3) + 4 + 4 * (w & 0xFF)
    return 'other', None


DELAYED = {'rts', 'rte', 'bra', 'bsr', 'bts', 'bfs', 'jmp', 'jsr', 'braf', 'bsrf'}


def h(x):
    return '0x%08X' % x
