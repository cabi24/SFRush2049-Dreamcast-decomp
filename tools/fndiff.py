"""Side-by-side byte comparison of a function: linked .abs range vs target image range.

    python tools/fndiff.py build/link/zoid.abs pilot/orig/1ST_READ.BIN \
        --start 8C068BB8 --end 8C068C50 [--linked-end 8C068C68]

The .abs carries absolute addresses already (LNK FORM A); the target image is indexed by
addr - --image-base (default 0x8C010000). Prints a 16-byte/row side-by-side with differing
offsets flagged, then the list of differing offsets in the common region.
"""
import argparse
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from sysrof_abs import read_abs


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


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('abs_file')
    ap.add_argument('target_bin')
    ap.add_argument('--image-base', type=lambda x: int(x, 16), default=0x8C010000)
    ap.add_argument('--start', type=lambda x: int(x, 16), required=True)
    ap.add_argument('--end', type=lambda x: int(x, 16), required=True, help='native end (exclusive)')
    ap.add_argument('--linked-end', type=lambda x: int(x, 16), help='linked end (exclusive); default --end')
    args = ap.parse_args()
    linked_end = args.linked_end if args.linked_end is not None else args.end

    linked = abs_range(args.abs_file, args.start, linked_end)
    native = file_range(args.target_bin, args.start, args.end, args.image_base)
    print(f'start {args.start:08X}  native [{args.start:08X},{args.end:08X}) {len(native):#x} B'
          f'   linked [{args.start:08X},{linked_end:08X}) {len(linked):#x} B')

    n = max(len(linked), len(native))
    common = min(len(linked), len(native))
    diffs = [i for i in range(common) if linked[i] != native[i]]
    print(f'differing bytes in common region ({common}): {len(diffs)}')

    for i in range(0, n, 16):
        lr = linked[i:i + 16]
        nr = native[i:i + 16]
        lrow = '  '.join(f'{c:02X}' for c in lr)
        nrow = '  '.join(f'{c:02X}' for c in nr)
        flags = '  '.join(f'+{j:02X}' for j in range(i, min(i + 16, n))
                          if j < common and linked[j] != native[j])
        print(f'{i:04X}  {lrow:<47s} | {nrow:<47s} {flags}')

    if len(linked) != len(native):
        longer, shorter = (linked, native) if len(linked) > len(native) else (native, linked)
        lo_name = 'linked' if len(linked) > len(native) else 'native'
        tail = longer[common:]
        print(f'\n{lo_name} tail beyond common region ({len(tail):#x} B):')
        for i in range(0, len(tail), 16):
            row = tail[i:i + 16]
            print(f'{common + i:04X}  ' + '  '.join(f'{c:02X}' for c in row))

    print(f'\ndiff offsets: {", ".join(f"{d:#x}" for d in diffs) if diffs else "(none)"}')


if __name__ == '__main__':
    main()
