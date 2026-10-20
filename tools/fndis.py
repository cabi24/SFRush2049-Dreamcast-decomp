"""Side-by-side SH-4 disassembly of a function: linked .abs range vs target image range.

    python tools/fndis.py build/link/zoid.abs pilot/orig/1ST_READ.BIN \
        --start 8C068BB8 --end 8C068C50 [--linked-end 8C068C68]

The .abs carries absolute addresses already (LNK FORM A); the target image is indexed by
addr - --image-base (default 0x8C010000). Uses capstone (CS_ARCH_SH / CS_MODE_SH4) to
disassemble both sides instruction-by-instruction and print them side by side. Bytes that
differ between the two sides are flagged with a trailing '*'.

NOTE: both columns print capstone's text for the NATIVE (target) side; only the trailing '*'
reflects the linked bytes, so register-number differences are invisible here. Use a
calib/sh4dis.py-based listing (as calib/unitcmp.py does) to see the linked operands, and note
capstone stops at the first FPU instruction. capstone frames cleanly only when given the true start address; it will render
pool literals as bogus instructions, so read the tail with fndiff instead.
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sysrof_abs import read_abs

from capstone import Cs, CS_ARCH_SH, CS_MODE_SH4


def abs_range(path, lo, hi):
    _, runs = read_abs(path)
    img = bytearray(hi - lo)
    for _, addr, chunk in runs:
        for i, b in enumerate(chunk):
            if lo <= addr + i < hi:
                img[addr + i - lo] = b
    return bytes(img)


def file_range(path, lo, hi, base):
    data = open(path, 'rb').read()
    olo, ohi = lo - base, hi - base
    if ohi > len(data):
        raise SystemExit(f'{path}: need {ohi:#x} bytes, have {len(data):#x}')
    return data[olo:ohi]


def disasm(cd, code, base):
    out = {}
    for ins in cd.disasm(code, base):
        out[ins.address] = (ins.mnemonic, ins.op_str, ins.bytes)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('abs_file')
    ap.add_argument('target_bin')
    ap.add_argument('--image-base', type=lambda x: int(x, 16), default=0x8C010000)
    ap.add_argument('--start', type=lambda x: int(x, 16), required=True)
    ap.add_argument('--end', type=lambda x: int(x, 16), required=True, help='native end (exclusive)')
    ap.add_argument('--linked-end', type=lambda x: int(x, 16), help='linked end (exclusive); default --end')
    ap.add_argument('--only-diff', action='store_true', help='print only lines whose bytes differ')
    ap.add_argument('--out', help='write output to this file instead of stdout')
    args = ap.parse_args()
    linked_end = args.linked_end if args.linked_end is not None else args.end
    out = open(args.out, 'w') if args.out else sys.stdout

    cd = Cs(CS_ARCH_SH, CS_MODE_SH4)

    linked = abs_range(args.abs_file, args.start, linked_end)
    native = file_range(args.target_bin, args.start, args.end, args.image_base)
    n_common = min(len(linked), len(native))

    ln_map = disasm(cd, linked, args.start)
    nv_map = disasm(cd, native, args.start)

    # walk native instructions in order; for each, find the linked instruction at same addr
    nv_list = sorted(nv_map.items())
    print(f'{"NATIVE":56s} || {"LINKED":56s}')
    shown = 0
    for addr, (mn, op, byt) in nv_list:
        lmn, lop, lbyt = ln_map.get(addr, (None, None, None))
        ltxt = (f'{addr:08X} {mn:<8s} {op}' if lmn is not None else f'{addr:08X} (gap/none)')
        ntxt = f'{addr:08X} {mn:<8s} {op}'
        differ = (lbyt is None) or (lbyt != byt)
        mark = '   *' if differ else ''
        if args.only_diff and not differ:
            continue
        print(f'{ntxt:<56s} || {ltxt}{mark}')
        shown += 1
    # linked-only trailing instructions (linked longer than native)
    for addr, (mn, op, byt) in sorted(ln_map.items()):
        if addr not in nv_map:
            if args.only_diff:
                pass
            print(f'{"(no native here)":56s} || {addr:08X} {mn:<8s} {op}   *')
    print(f'\n{shown} native instructions shown')


if __name__ == '__main__':
    main()
