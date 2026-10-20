#!/usr/bin/env python3
"""Compare two compiler outputs of the same source, ignoring only the compiler's date stamp.

    python tools/objcmp.py a.obj b.obj          # SYSROF objects (shc.exe -object=)
    python tools/objcmp.py a.elf b.elf          # rof2elf outputs

SHC writes the compile time as twelve ASCII digits (YYMMDDhhmmss) into the SYSROF unit header
block (type 0x04) and the module block (type 0x06), and every block ends in a checksum byte that
covers it, so two compiles of the same source are never byte-identical. rof2elf copies the same
stamp into the ELF .comment section ("Source Date: YYMMDDhhmmss"). This tool blanks exactly
those fields and compares everything else byte for byte: block framing, record order, section
contents, symbols, relocations. Exit status 0 means identical apart from the stamp; otherwise the
first differing offsets are printed. Nothing else is masked.
"""
import re
import sys

STAMP_RE = re.compile(rb'\d{12}')


def normalize_sysrof(data):
    """Return the SYSROF stream with each block's checksum byte dropped and the 12-digit date
    stamp in the 0x04/0x06 header blocks replaced by zeros. Framing follows sysrof_abs.py:
    [type|0x80 if last-part][size][payload...][checksum]."""
    out = bytearray()
    off = 0
    stamps = []
    while off + 2 <= len(data):
        fmt, size = data[off], data[off + 1]
        if size < 3:
            raise ValueError(f'bad block size {size} at {off:#x}')
        block = bytearray(data[off:off + size])
        if (fmt & 0x7F) in (0x04, 0x06):
            for m in STAMP_RE.finditer(bytes(block[2:-1])):
                stamps.append((off + 2 + m.start(), m.group(0).decode()))
                block[2 + m.start():2 + m.end()] = b'\0' * 12
        out += block[:-1]  # drop the checksum byte (it covers the stamp)
        off += size
        if (fmt & 0x7F) == 0x7F:
            break
    out += data[off:]
    return bytes(out), stamps


def normalize_elf(data):
    """Blank the 'Source Date: YYMMDDhhmmss' stamp rof2elf writes into .comment."""
    stamps = []
    m = re.search(rb'Source Date: (\d{12})', data)
    if m:
        stamps.append((m.start(1), m.group(1).decode()))
        data = data[:m.start(1)] + b'\0' * 12 + data[m.end(1):]
    return data, stamps


def normalize(data):
    if data[:4] == b'\x7fELF':
        return normalize_elf(data)
    return normalize_sysrof(data)


def compare(path_a, path_b):
    """(identical_modulo_stamp, differing_offsets_in_normalized_streams, stamps_a, stamps_b)."""
    a, sa = normalize(open(path_a, 'rb').read())
    b, sb = normalize(open(path_b, 'rb').read())
    if a == b:
        return True, [], sa, sb
    diffs = [i for i in range(min(len(a), len(b))) if a[i] != b[i]]
    if len(a) != len(b):
        diffs.append(min(len(a), len(b)))
    return False, diffs, sa, sb


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    same, diffs, sa, sb = compare(sys.argv[1], sys.argv[2])
    stamp = lambda s: ','.join(v for _o, v in s) or '-'
    if same:
        print(f'identical apart from the date stamp ({stamp(sa)} vs {stamp(sb)})')
        return 0
    print(f'DIFFER: {len(diffs)} normalized offsets differ, first {", ".join(f"{d:#x}" for d in diffs[:16])}')
    return 1


if __name__ == '__main__':
    sys.exit(main())
