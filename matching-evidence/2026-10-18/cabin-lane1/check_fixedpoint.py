#!/usr/bin/env python3
"""Fixed-point test: rerun tools/dc_objdiff.py and verify objdiff.json is unchanged."""
import difflib
import hashlib
import os
import shutil
import subprocess

DC = r'C:\rush2049-dreamcast'
OBJ = os.path.join(DC, 'objdiff.json')
SNAP = os.path.join(DC, 'matching-evidence', '2026-10-18', 'cabin-lane1', 'objdiff-before-regen.json')


def sha(p):
    return hashlib.sha256(open(p, 'rb').read()).hexdigest()


def main():
    before = sha(OBJ)
    shutil.copy(OBJ, SNAP)
    r = subprocess.run(['python', 'tools/dc_objdiff.py'], cwd=DC,
                       capture_output=True, text=True)
    print('rc:', r.returncode)
    print(r.stdout.strip())
    if r.stderr.strip():
        print('stderr:', r.stderr.strip()[:500])
    after = sha(OBJ)
    print(f'before: {before}')
    print(f'after:  {after}')
    print('fixed point:', before == after)
    if before != after:
        a = open(SNAP, 'rb').read().decode('utf-8')
        b = open(OBJ, 'rb').read().decode('utf-8')
        lines = list(difflib.unified_diff(a.splitlines(), b.splitlines(),
                                          'before', 'after', lineterm=''))
        print(f'{len(lines)} diff lines')
        for l in lines[:60]:
            print(l)


if __name__ == '__main__':
    main()
