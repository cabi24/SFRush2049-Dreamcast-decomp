"""Minimal reader for Hitachi SYSROF *absolute* files (LNK.EXE `FORM A` output).

rof2elf.py refuses ABS files, and elfcnv 1.0G rejects what LNK 6.0E produces here, so this
extracts the located section contents directly. Block framing follows rof2elf.py: each block is
[type|0x80 if last-part][size][payload...][checksum]; OBJECT_INFO records carry an absolute start
address when bit 0x80 of the record flag is set.

    python sysrof_abs.py file.abs            # list sections and data runs
    python sysrof_abs.py file.abs --raw out.bin --start 8C03A1B8 --end 8C03A360
"""
import argparse, struct, sys
from io import BytesIO

SECTION_HEADER_INFO, SECTION_INFO, OBJECT_INFO, TRAILER_INFO = 0x1A, 0x08, 0x1C, 0x7F


def read_blocks(data):
    f = BytesIO(data)
    blocks = []
    while f.tell() < len(data):
        fmt, size = struct.unpack('<BB', f.read(2))
        f.seek(-2, 1)
        raw = f.read(size)
        btype, last = fmt & 0x7F, bool(fmt & 0x80)
        payload = raw[2:-1]
        while not last:
            fmt, size = struct.unpack('<BB', f.read(2))
            f.seek(-2, 1)
            raw = f.read(size)
            last = bool(fmt & 0x80)
            payload += raw[2:-1]
        blocks.append((btype, payload))
        if btype == TRAILER_INFO:
            break
    return blocks


def read_abs(path):
    """Return (sections, runs): sections = [(name, address, length)], runs = [(section_index, address, bytes)]."""
    data = open(path, 'rb').read()
    sections, runs = [], []
    cur = None
    for btype, payload in read_blocks(data):
        f = BytesIO(payload)
        if btype == SECTION_INFO:
            f.read(1)
            address, length, align = struct.unpack('>III', f.read(12))
            f.read(3)
            slen = f.read(1)[0]
            sections.append((f.read(slen).decode(), address, length))
        elif btype == SECTION_HEADER_INFO:
            _unit, cur = struct.unpack('>HH', f.read(4))
        elif btype == OBJECT_INFO:
            addr = None
            while f.tell() < len(payload):
                t = f.read(1)[0]
                if t & 0x80:
                    addr = struct.unpack('>I', f.read(4))[0]
                rep = struct.unpack('>I', f.read(4))[0] if t & 0x40 else 0
                n = f.read(1)[0]
                chunk = f.read(n)
                if rep:
                    chunk = chunk * rep
                if addr is None:
                    raise ValueError('object record without a start address')
                runs.append((cur, addr, chunk))
                addr += len(chunk)
    return sections, runs


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('abs')
    ap.add_argument('--raw', help='write a raw image of [--start, --end) filled with --fill')
    ap.add_argument('--start', type=lambda x: int(x, 16))
    ap.add_argument('--end', type=lambda x: int(x, 16))
    ap.add_argument('--fill', type=lambda x: int(x, 16), default=0)
    args = ap.parse_args()
    sections, runs = read_abs(args.abs)
    for i, (name, addr, length) in enumerate(sections):
        print(f'section {i} {name:8s} {addr:08X} len {length:#x}')
    for sec, addr, chunk in runs:
        print(f'  run sec {sec} {addr:08X} +{len(chunk):#x}')
    if args.raw:
        lo = args.start if args.start is not None else min(a for _, a, _ in runs)
        hi = args.end if args.end is not None else max(a + len(c) for _, a, c in runs)
        img = bytearray([args.fill]) * (hi - lo)
        for _, addr, chunk in runs:
            for i, b in enumerate(chunk):
                if lo <= addr + i < hi:
                    img[addr + i - lo] = b
        open(args.raw, 'wb').write(img)
        print(f'wrote {args.raw}: {hi - lo:#x} bytes from {lo:08X}')


if __name__ == '__main__':
    main()
