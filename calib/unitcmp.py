"""Per-function comparison of a compiled unit against the target.

    python unitcmp.py out/label/ver/unit.elf  sym=TARGET_VA[:SIZE] ...

Each function is located in the ELF by symbol; its extent runs to the next function symbol
(or the given SIZE). Target bytes come from pilot/orig/1ST_READ.BIN. The comparison masks
PC-relative literal-load displacements like run_calib.py and prints, per function, the masked
diff count and the first differing lines; literal pools between functions are reported separately
by their raw words.
"""
import os, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sh4dis import disasm_words, is_pcrel_load

DC = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGET = os.path.join(DC, 'pilot', 'orig', '1ST_READ.BIN')
BASE = 0x8C010000


def elf_text_syms(path):
    d = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', d, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    secs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    st = secs[shstrndx]
    names = d[st[4]:st[4] + st[5]]
    nm = lambda o: names[o:names.index(b'\0', o)].decode()
    text = next(d[s[4]:s[4] + s[5]] for s in secs if nm(s[0]) == '.text')
    symtab = next(s for s in secs if nm(s[0]) == '.symtab')
    strs = d[secs[symtab[6]][4]:secs[symtab[6]][4] + secs[symtab[6]][5]]
    syms = {}
    for i in range(symtab[5] // 16):
        n, v, sz, info, o, sh = struct.unpack_from('<IIIBBH', d, symtab[4] + i * 16)
        if info & 15 == 2:
            syms[strs[n:strs.index(b'\0', n)].decode()] = v
    return text, syms


def masked(code):
    out = bytearray(code)
    for i in range(0, len(code) - 1, 2):
        if is_pcrel_load(code[i] | (code[i + 1] << 8)):
            out[i] = 0
    return bytes(out)


def main():
    elf = sys.argv[1]
    text, syms = elf_text_syms(elf)
    target = open(TARGET, 'rb').read()
    specs = []
    for a in sys.argv[2:]:
        name, rest = a.split('=')
        va, _, size = rest.partition(':')
        specs.append((name, int(va, 16), int(size, 16) if size else None))
    order = sorted(syms.items(), key=lambda kv: kv[1])
    for name, va, size in specs:
        sym = '_' + name if ('_' + name) in syms else name
        start = syms[sym]
        nxt = next((v for n, v in order if v > start), len(text))
        mine = text[start:nxt]
        if size is None:
            size = len(mine)
        want = target[va - BASE: va - BASE + size]
        m, w = masked(mine[:size]), masked(want)
        ndiff = sum(1 for a, b in zip(m, w) if a != b) + abs(len(m) - len(w))
        tag = 'EXACT' if mine[:size] == want else ('MATCH (pool disp only)' if m == w else f'{ndiff} masked bytes differ')
        print(f'{name:18s} mine +{start:#06x} len {len(mine):#5x}  target {va:08x} len {size:#5x}  {tag}')
        if m != w:
            ml = list(disasm_words(mine[:size], va))
            wl = list(disasm_words(want, va))
            shown = 0
            for i in range(max(len(ml), len(wl))):
                a = ml[i] if i < len(ml) else None
                b = wl[i] if i < len(wl) else None
                ta = a[2] if a else ''
                tb = b[2] if b else ''
                if a and b and masked(a[1]) == masked(b[1]):
                    continue
                print(f'    * {va + 2 * i:08x} {(a[1].hex() if a else "----"):6s} {ta:30s} | {(b[1].hex() if b else "----"):6s} {tb}')
                shown += 1
                if shown >= 12:
                    print('    ...')
                    break


if __name__ == '__main__':
    main()
