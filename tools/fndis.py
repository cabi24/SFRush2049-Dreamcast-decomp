"""Side-by-side SH-4 disassembly of an address range: linked .abs against the target image.

    python -B tools/fndis.py build/link/zoid.abs pilot/orig/1ST_READ.BIN \
        --start 8C068BB8 --end 8C068C50 [--linked-end 8C068C68] [--linked-start 8C068BB8]

Both columns are decoded from their OWN bytes with calib/sh4dis.py (every FPU and system
instruction), aligned by instruction and annotated with the literals that PC-relative loads
read, exactly like tools/fncmp.py; the first differing word is marked `<<<`. Prefer fncmp.py
when the unit is in build/report.json (it finds the function, its linked address and the pool
for you); this form only needs an .abs file and a raw range. Run without `python -I` (capstone).
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dc_common as C  # noqa: E402
import fncmp  # noqa: E402


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('abs_file')
    ap.add_argument('target_bin', help='target image (checked against the pinned SHA-256 when it is the project one)')
    ap.add_argument('--image-base', type=lambda x: int(x, 16), default=0x8C010000)
    ap.add_argument('--start', type=lambda x: int(x, 16), required=True, help='target start')
    ap.add_argument('--end', type=lambda x: int(x, 16), required=True, help='target end (exclusive)')
    ap.add_argument('--linked-start', type=lambda x: int(x, 16), help='linked start; default --start')
    ap.add_argument('--linked-end', type=lambda x: int(x, 16), help='linked end (exclusive); default --end')
    ap.add_argument('--context', type=int, help='show only differing words with N lines of context')
    ap.add_argument('--out', help='write output to this file instead of stdout')
    args = ap.parse_args()
    l0 = args.linked_start if args.linked_start is not None else args.start
    l1 = args.linked_end if args.linked_end is not None else l0 + (args.end - args.start)

    data = open(args.target_bin, 'rb').read()
    base = args.image_base
    tget = lambda a: data[a - base] if 0 <= a - base < len(data) else None
    sections, image = fncmp.abs_image(args.abs_file)
    names = fncmp.Names(C.load_map())
    linked = fncmp.make_side(image.get, l0, l1 - l0, f'LINKED ({os.path.basename(args.abs_file)})')
    target = fncmp.make_side(tget, args.start, args.end - args.start, f'TARGET ({os.path.basename(args.target_bin)})')
    linked_lits = fncmp.scan_literals(image.get, [(a, a + l) for n, a, l in sections if n == 'P'])
    target_lits = fncmp.scan_literals(tget, [(args.start, args.end)])
    out = open(args.out, 'w') if args.out else None
    emit = (lambda s: print(s, file=out)) if out else print
    fncmp.render(linked, target, names, out=emit, context=args.context,
                 header=f'target {args.start:08X}..{args.end:08X}  linked {l0:08X}..{l1:08X}',
                 linked_lits=linked_lits, target_lits=target_lits)
    if out:
        out.close()


if __name__ == '__main__':
    main()
