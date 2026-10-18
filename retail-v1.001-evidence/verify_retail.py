"""Verify the pinned Rush 2049 US v1.001 executable/IP and resource pointers.
No game execution. Optional extraction is restricted to scratch outside the original root.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path


class Disc:
    def __init__(self, gdi, root):
        self.tracks = []
        self.handles = []
        for line in gdi.read_text().splitlines()[1:]:
            number, lba, kind, sector_bytes, filename, offset = line.split()
            if int(kind) != 4:
                continue
            path = (gdi.parent / filename).resolve()
            if root not in path.parents:
                raise ValueError('Track path escapes original input root')
            handle = path.open('rb')
            self.handles.append(handle)
            self.tracks.append({'lba': int(lba), 'size': int(sector_bytes),
                                'offset': int(offset), 'handle': handle,
                                'sectors': (path.stat().st_size - int(offset)) // int(sector_bytes)})

    def close(self):
        for handle in self.handles:
            handle.close()

    def read(self, lba, size):
        output = bytearray()
        while len(output) < size:
            track = next((t for t in self.tracks if t['lba'] <= lba < t['lba'] + t['sectors']), None)
            if track is None or track['size'] != 2352:
                raise ValueError('Expected recorded data-track extent')
            count = min((size - len(output) + 2047) // 2048, track['sectors'] - (lba - track['lba']))
            track['handle'].seek(track['offset'] + (lba - track['lba']) * 2352)
            raw = track['handle'].read(count * 2352)
            if len(raw) != count * 2352:
                raise ValueError('Short data-track read')
            for offset in range(0, len(raw), 2352):
                if raw[offset + 15] != 1:
                    raise ValueError('Expected Mode 1 sector')
                output.extend(raw[offset + 16:offset + 2064])
            lba += count
        return bytes(output[:size])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument('--out', type=Path, help='Optional extraction directory outside input root')
    args = parser.parse_args()
    root = args.root.resolve()
    evidence = Path(__file__).resolve().parent
    summary = json.loads((evidence / 'summary.json').read_text(encoding='utf-8'))
    table = json.loads((evidence / 'resource_filename_table.json').read_text(encoding='utf-8'))
    gdi = (root / summary['source_gdi']).resolve()
    if root not in gdi.parents:
        raise ValueError('Input GDI must stay inside original root')
    disk = Disc(gdi, root)
    try:
        exe = disk.read(summary['executable']['lba'], summary['executable']['size'])
        ip = disk.read(45000, 32768)
    finally:
        disk.close()
    digests = {'1ST_READ.BIN': hashlib.sha256(exe).hexdigest(),
               'IP.BIN': hashlib.sha256(ip).hexdigest()}
    if digests['1ST_READ.BIN'] != summary['executable']['sha256'] or digests['IP.BIN'] != summary['ip_sha256']:
        raise ValueError('Image content differs from the pinned target')
    for entry in table['entries']:
        pointer = struct.unpack_from('<I', exe, entry['pointer_offset'])[0]
        string_offset = pointer - 0x8C010000
        if string_offset != entry['string_offset'] or not 0 <= string_offset < len(exe):
            raise ValueError('Filename pointer mismatch')
        end = exe.find(b'\0', string_offset)
        if end < 0 or exe[string_offset:end].decode('ascii') != entry['filename']:
            raise ValueError('Filename string mismatch')
    if struct.unpack_from('<I', exe, table['end_exclusive'])[0] != 0:
        raise ValueError('Resource table terminator differs')
    result = {'build': summary['build'], 'gdi': str(gdi), 'hashes_match': True,
              'sha256': digests, 'resource_pointers_checked': len(table['entries'])}
    if args.out:
        output = args.out.resolve()
        if output == root or root in output.parents:
            raise ValueError('Extraction output must be outside original game root')
        payloads = {'1ST_READ.BIN': exe, 'IP.BIN': ip,
                    'verification.json': (json.dumps(result, indent=2) + '\n').encode('utf-8')}
        for name, payload in payloads.items():
            path = output / name
            if path.exists() and path.read_bytes() != payload:
                raise ValueError('Refusing to replace differing scratch file: ' + str(path))
        output.mkdir(parents=True, exist_ok=True)
        for name, payload in payloads.items():
            (output / name).write_bytes(payload)
        result['extracted_to'] = str(output)
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
