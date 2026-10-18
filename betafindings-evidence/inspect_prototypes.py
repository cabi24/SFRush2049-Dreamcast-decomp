"""Read-only verification of four explicitly catalogued Rush 2049 prototypes.
Optional extraction writes only to a separately specified directory outside the input root.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path, PureWindowsPath

BUILD_LABELS = {
    'feb17': 'Feb_17_2000_Dreamcast_Prototype',
    'apr10': 'Apr_10_2000_prototype',
    'aug1': 'Aug_1_2000_prototype',
    'aug7': 'Aug_7_2000_prototype',
}


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def safe_input(root, relative):
    target = (root / relative).resolve()
    if target == root or root not in target.parents:
        raise ValueError('Input path escapes the original game root')
    return target


class Disc:
    def __init__(self, gdi, root):
        self.tracks = []
        self.handles = []
        for line in gdi.read_text().splitlines()[1:]:
            number, lba, kind, sector_size, filename, file_offset = line.split()
            if int(kind) != 4:
                continue
            track_path = (gdi.parent / filename).resolve()
            if root not in track_path.parents:
                raise ValueError('GDI track path escapes the input root')
            handle = track_path.open('rb')
            self.handles.append(handle)
            self.tracks.append({
                'lba': int(lba), 'sector_size': int(sector_size),
                'file_offset': int(file_offset), 'path': track_path, 'handle': handle,
                'sectors': (track_path.stat().st_size - int(file_offset)) // int(sector_size),
            })

    def close(self):
        for handle in self.handles:
            handle.close()

    def read(self, lba, size):
        output = bytearray()
        while len(output) < size:
            track = next((t for t in self.tracks if t['lba'] <= lba < t['lba'] + t['sectors']), None)
            if track is None:
                raise ValueError('Requested extent crosses an absent/data-free track area')
            count = min((size - len(output) + 2047) // 2048, track['sectors'] - (lba - track['lba']))
            sector_size = track['sector_size']
            if sector_size != 2352:
                raise ValueError('This helper expects the recorded 2352-byte tracks')
            track['handle'].seek(track['file_offset'] + (lba - track['lba']) * sector_size)
            raw = track['handle'].read(count * sector_size)
            if len(raw) != count * sector_size:
                raise ValueError('Short track read')
            for offset in range(0, len(raw), sector_size):
                sector = raw[offset:offset + sector_size]
                if sector[15] != 1:
                    raise ValueError('Expected a Mode 1 sector')
                output.extend(sector[16:2064])
            lba += count
        return bytes(output[:size])


def read_c_string(data, pointer):
    offset = pointer - 0x8C010000
    if not 0 <= offset < len(data):
        raise ValueError('Resource pointer outside executable')
    end = data.find(b'\0', offset)
    if end < 0:
        raise ValueError('Unterminated resource label')
    return data[offset:end].decode('ascii')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build', required=True, choices=BUILD_LABELS)
    parser.add_argument('--root', type=Path, default=Path(__file__).resolve().parent.parent)
    parser.add_argument('--out', type=Path, help='Optional scratch directory outside the original root')
    args = parser.parse_args()
    root = args.root.resolve()
    evidence = Path(__file__).resolve().parent
    label = BUILD_LABELS[args.build]
    summary = json.loads((evidence / 'summary.json').read_text(encoding='utf-8'))
    inventory = json.loads((evidence / 'inventory.json').read_text(encoding='utf-8'))
    # Match only the four fixed catalogue labels, never glob for newly added games.
    browser_report = json.loads((evidence / label / 'browser_source_literals.json').read_text(encoding='utf-8'))
    del browser_report
    entries = [(b, i) for b in summary['builds'] for i in inventory
               if b['build'] == i['build'] and
               b['build'].split('(', 1)[1].rstrip(')').replace(',', '').replace(' ', '_') == label]
    if len(entries) != 1:
        raise ValueError('Expected exactly one known prototype')
    build, inv = entries[0]
    original_relative = PureWindowsPath(inv['gdi']).relative_to(PureWindowsPath('C:/rush2049-dreamcast'))
    gdi = safe_input(root, Path(*original_relative.parts))
    disk = Disc(gdi, root)
    try:
        exe_info = build['main_executable']
        exe = disk.read(exe_info['lba'], exe_info['size'])
        ip = disk.read(45000, 32768)
    finally:
        disk.close()
    hashes = {'1ST_READ.BIN': sha256(exe), 'IP.BIN': sha256(ip)}
    if hashes['1ST_READ.BIN'] != exe_info['sha256'] or hashes['IP.BIN'] != build['ip_sha256']:
        raise ValueError('Hash mismatch: these bytes differ from the catalogued prototype')
    pair_file = evidence / label / 'resource_pairs.json'
    checked_pairs = 0
    if pair_file.exists():
        for entry in json.loads(pair_file.read_text(encoding='utf-8')):
            name_pointer = struct.unpack_from('<I', exe, entry['name_pointer_offset'])[0]
            filename_pointer = struct.unpack_from('<I', exe, entry['filename_pointer_offset'])[0]
            if (read_c_string(exe, name_pointer), read_c_string(exe, filename_pointer)) != (entry['name'], entry['filename']):
                raise ValueError('Resource pair differs from recorded evidence')
            checked_pairs += 1
    result = {'build': build['build'], 'gdi': str(gdi), 'hashes_match': True,
              'sha256': hashes, 'resource_pairs_checked': checked_pairs}
    if args.out:
        output = args.out.resolve()
        if output == root or root in output.parents:
            raise ValueError('Extraction output must be outside the original game root')
        payloads = {'1ST_READ.BIN': exe, 'IP.BIN': ip,
                    'verification.json': (json.dumps(result, indent=2) + '\n').encode('utf-8')}
        for name, payload in payloads.items():
            target = output / name
            if target.exists() and target.read_bytes() != payload:
                raise ValueError('Refusing to replace differing scratch file: ' + str(target))
        output.mkdir(parents=True, exist_ok=True)
        for name, payload in payloads.items():
            (output / name).write_bytes(payload)
        result['extracted_to'] = str(output)
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
