"""Compile the pilot training sources with SHC and compare against the target spans.

Comparison is instruction-exact except that the 8-bit displacement of PC-relative literal loads
(mov.w @(disp,pc) = 0x9nnn, mov.l @(disp,pc) = 0xDnnn) is masked, because the pool location is
decided by layout/link. Everything else (including branch displacements) must be byte-equal.

usage: python run_calib.py [-v VER[,VER...]] [-s SLOT[,SLOT...]] [--label NAME] [-- extra shc flags]
Writes out/<label>/<ver>/<slot>.{elf,obj,txt} and appends a row per compile to trials.jsonl.
"""
import argparse, datetime, hashlib, json, os, struct, subprocess, sys
import capstone
from sh4dis import disasm_words, is_pcrel_load

ROOT = os.path.dirname(os.path.abspath(__file__))
DC = os.path.dirname(ROOT)
TOOLS = os.path.join(DC, 'tools')
TARGET = os.path.join(DC, 'pilot', 'orig', '1ST_READ.BIN')
BASE = 0x8C010000
SLOTS = {  # name: (file offset, size, symbol)
    'interp': (0x131d2, 20, '_interp'),
    'bsearch': (0x8bc04, 118, '_bsearch'),
    'InsertLLE': (0x2a1c8, 198, '_InsertLLE'),
    'BodyVector': (0x16b00, 94, '_BodyVector'),
    'enginetorque': (0x130b0, 290, '_enginetorque'),
    'UpdateFrameTimer': (0x10a, 108, '_UpdateFrameTimer'),
    'll_tu': (0x2a1b8, 422, '_InitLL'),
    'timer_tu': (0xa4, 0x16a, '_RetraceCallback'),  # timer.c unit incl. pools: 0x8C0100A4..0x8C01020E
    'drivetra_part': (0x130b0, 0x248, '_enginetorque'),  # enginetorque, interp, engine incl. pools: ..0x8C0232F8  # whole ll.c TU: InitLL, ClearLL, InsertLLE, RemoveLLE
    'RemoveLLE': (0x2a28e, 208, '_RemoveLLE'),
}
ALL_VERS = ['shc-v5.0r10', 'shc-v5.0r26', 'shc-v5.0r28', 'shc-v5.0r31', 'shc-v5.0r32',
            'shc-v5.1r01', 'shc-v5.1r03', 'shc-v5.1r04', 'shc-v5.1r08', 'shc-v5.1r11', 'shc-v5.1r13']
md = capstone.Cs(capstone.CS_ARCH_SH, capstone.CS_MODE_SH4 | capstone.CS_MODE_LITTLE_ENDIAN)


def elf_symbols_and_text(path):
    d = open(path, 'rb').read()
    shoff = struct.unpack_from('<I', d, 0x20)[0]
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    secs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
    st = secs[shstrndx]
    names = d[st[4]:st[4] + st[5]]
    nm = lambda o: names[o:names.index(b'\0', o)].decode()
    text = b''
    syms = {}
    for s in secs:
        if nm(s[0]) == '.text':
            text = d[s[4]:s[4] + s[5]]
    for s in secs:
        if nm(s[0]) == '.symtab':
            strtab = secs[s[6]]
            strs = d[strtab[4]:strtab[4] + strtab[5]]
            for i in range(s[5] // 16):
                name, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', d, s[4] + i * 16)
                if info & 15 == 2:
                    syms[strs[name:strs.index(b'\0', name)].decode()] = value
    return text, syms


def masked(code):
    out = bytearray(code)
    for i in range(0, len(code) - 1, 2):
        if is_pcrel_load(code[i] | (code[i + 1] << 8)):
            out[i] = 0
    return bytes(out)


def disasm(code, addr):
    return [f'{a:08x} {wb.hex():6s} {text}' for a, wb, text in disasm_words(code, addr)]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-v', '--versions', default='shc-v5.1r11')
    ap.add_argument('-s', '--slots', default='interp,bsearch,InsertLLE,BodyVector')
    ap.add_argument('--label', default='base')
    ap.add_argument('--src', default=os.path.join(ROOT, 'src'))
    ap.add_argument('--no-default', action='store_true', help='drop -aggressive=2 -extra=asm=1800 -pic=0')
    ap.add_argument('flags', nargs='*')
    args = ap.parse_args()
    vers = ALL_VERS if args.versions == 'all' else args.versions.split(',')
    target = open(TARGET, 'rb').read()
    trials = open(os.path.join(ROOT, 'trials.jsonl'), 'a')
    print(f'label={args.label} flags={" ".join(args.flags) or "(decomp.me base)"}')
    for ver in vers:
        for slot in args.slots.split(','):
            off, size, sym = SLOTS[slot]
            want = target[off:off + size]
            outdir = os.path.join(ROOT, 'out', args.label, ver)
            os.makedirs(outdir, exist_ok=True)
            elf = os.path.join(outdir, slot + '.elf')
            src = os.path.join(args.src, slot + '.c')
            cmd = [sys.executable, os.path.join(TOOLS, 'shc_cc.py'), '-v', ver, src, '-o', elf,
                   '--keep-obj', os.path.join(outdir, slot + '.obj')] + (['--no-default-flags'] if args.no_default else []) + (['--'] + args.flags if args.flags else [])
            r = subprocess.run(cmd, capture_output=True, text=True)
            row = {'time': datetime.datetime.now().isoformat(timespec='seconds'), 'label': args.label,
                   'version': ver, 'slot': slot, 'flags': (['--no-default-flags'] if args.no_default else []) + args.flags,
                   'source_sha256': hashlib.sha256(open(src, 'rb').read()).hexdigest()}
            if r.returncode != 0:
                row['status'] = 'compile failed'
                row['error'] = (r.stdout + r.stderr).strip()[-300:]
                print(f'{ver:12s} {slot:16s} COMPILE FAILED: {row["error"][-120:]}')
                trials.write(json.dumps(row) + '\n')
                continue
            text, syms = elf_symbols_and_text(elf)
            start = syms.get(sym, 0)
            got = text[start:start + size]
            exact = got == want
            mexact = masked(got) == masked(want)
            ndiff = sum(1 for a, b in zip(masked(got), masked(want)) if a != b) + abs(len(got) - len(want))
            row.update({'status': 'ok', 'object_sha256': hashlib.sha256(open(elf, 'rb').read()).hexdigest(),
                        'text_size': len(text), 'exact': exact, 'masked_exact': mexact, 'masked_diff_bytes': ndiff})
            trials.write(json.dumps(row) + '\n')
            verdict = 'EXACT' if exact else ('MATCH (pool disp only)' if mexact else f'differs: {ndiff} masked bytes')
            print(f'{ver:12s} {slot:16s} text={len(text):4d}  {verdict}')
            with open(os.path.join(outdir, slot + '.txt'), 'w') as f:
                f.write(f'# {ver} {slot} flags={args.flags}\n# target {BASE+off:08x} size {size}\n')
                f.write('--- emitted .text ---\n' + '\n'.join(disasm(text, 0)) + '\n')
                f.write('--- target ---\n' + '\n'.join(disasm(want, BASE + off)) + '\n')
                if not mexact:
                    f.write('--- side by side (masked compare) ---\n')
                    g = disasm(got, BASE + off); w = disasm(want, BASE + off)
                    for i in range(max(len(g), len(w))):
                        a = g[i] if i < len(g) else ''
                        b = w[i] if i < len(w) else ''
                        mark = ' ' if a.split(' ', 2)[2:] == b.split(' ', 2)[2:] else '*'
                        f.write(f'{mark} {a:44s} | {b}\n')
    trials.close()


if __name__ == '__main__':
    main()
