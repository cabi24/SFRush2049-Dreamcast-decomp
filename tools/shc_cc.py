#!/usr/bin/env python3
"""Compile one C file with a Hitachi SHC package using the decomp.me Dreamcast recipe
and convert the SYSROF object to an ELF32 (little-endian SH) relocatable object.

Recipe (mirrors decomp.me cromper `DREAMCAST_CC`):
  - source is converted to CRLF
  - the package's bin/ is copied into a private work dir; SHC_LIB=. SHC_TMP=. (SHC 5.0 insists on this)
  - shc.exe <src> -comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis
            -string=const [-aggressive=2 -extra=asm=1800 -pic=0 ...] -object=<obj>
    (shc-v5.0r10 does not accept -fpu=single / -aggressive=2)
  - rof2elf.py <obj> <elf> --isa=sh4

Launch mode: native only. A Windows policy denial is terminal; report it.

Example:
  python tools/shc_cc.py -v shc-v5.1r11 src/interp.c -o build/interp.elf --no-default-flags -- -speed -noinline
Extra SHC options go after `--`; `--no-default-flags` drops the decomp.me flag set entirely.
"""
import argparse
import hashlib
import os
import shutil
import subprocess
import sys
import tempfile

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROF2ELF = os.path.join(TOOLS, 'rof2elf', 'rof2elf.py')
BASE_FLAGS = ['-comment=nonest', '-cpu=sh4', '-division=cpu', '-fpu=single', '-endian=little',
              '-macsave=0', '-sjis', '-string=const']
DECOMPME_DEFAULT_EXTRA = ['-aggressive=2', '-extra=asm=1800', '-pic=0']


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('source')
    ap.add_argument('-o', '--output', required=True, help='output ELF object path')
    ap.add_argument('-v', '--version', default='shc-v5.1r11', help='package dir name under tools/shc (default shc-v5.1r11)')
    ap.add_argument('--launch', choices=['native'], default='native')
    ap.add_argument('--no-default-flags', action='store_true', help='omit -aggressive=2 -extra=asm=1800 -pic=0')
    ap.add_argument('--keep', action='store_true', help='keep the work directory (prints its path)')
    ap.add_argument('--keep-obj', help='also copy the raw SYSROF .obj to this path')
    ap.add_argument('shc_flags', nargs='*', help='extra shc options (after --)')
    args = ap.parse_args()

    pkg = os.path.join(TOOLS, 'shc', args.version)
    bindir = os.path.join(pkg, 'bin')
    shc = os.path.join(bindir, 'shc.exe')
    if not os.path.isfile(shc):
        sys.exit(f'no shc.exe in {bindir}')

    flags = list(BASE_FLAGS)
    if not args.no_default_flags:
        flags += DECOMPME_DEFAULT_EXTRA
    if args.version == 'shc-v5.0r10':
        flags = [f for f in flags if f not in ('-fpu=single', '-aggressive=2')]
    flags += args.shc_flags

    work = tempfile.mkdtemp(prefix='shc_', dir=os.path.join(tempfile.gettempdir(), 'rush2049-dc'))
    try:
        for name in os.listdir(bindir):
            p = os.path.join(bindir, name)
            if os.path.isfile(p):
                shutil.copy(p, work)
        src_text = open(args.source, 'rb').read().replace(b'\r\n', b'\n').replace(b'\n', b'\r\n')
        open(os.path.join(work, 'dos_src.c'), 'wb').write(src_text)
        argv_tail = ['dos_src.c'] + flags + ['-object=dos_src.obj']

        def run_native():
            env = dict(os.environ, SHC_LIB='.', SHC_TMP='.')
            return subprocess.run([os.path.join(work, 'shc.exe')] + argv_tail, cwd=work, env=env,
                                  capture_output=True, text=True, timeout=120)

        # Native-only safety gate: a denial is terminal. No WSL/wibo fallback.
        try:
            r = run_native()
            mode_used = 'native'
        except OSError as exc:
            sys.exit('[NATIVE_EXECUTION_BLOCKED] package=' + shc + ' source=' + args.source + ' action=compile winerror=' + str(getattr(exc, 'winerror', None)) + ' reason=' + str(exc))

        obj = os.path.join(work, 'dos_src.obj')
        banner = ' '.join((r.stdout + r.stderr).split())
        if r.returncode != 0 or not os.path.isfile(obj):
            sys.exit(f'[shc_cc] {args.version} ({mode_used}) failed rc={r.returncode}: {banner}')
        if args.keep_obj:
            shutil.copy(obj, args.keep_obj)
        os.makedirs(os.path.dirname(os.path.abspath(args.output)) or '.', exist_ok=True)
        c = subprocess.run([sys.executable, '-I', ROF2ELF, obj, os.path.abspath(args.output), '--isa=sh4'],
                           cwd=work, capture_output=True, text=True)
        if c.returncode != 0:
            sys.exit(f'[shc_cc] rof2elf failed: {(c.stdout + c.stderr).strip()}')
        print(f'[shc_cc] {args.version} ({mode_used}) shc.exe sha256={sha256(shc)[:16]} flags={" ".join(flags)}')
        print(f'[shc_cc] wrote {args.output}')
        if args.keep:
            print(f'[shc_cc] work dir kept: {work}')
    finally:
        if not args.keep:
            shutil.rmtree(work, ignore_errors=True)


if __name__ == '__main__':
    os.makedirs(os.path.join(tempfile.gettempdir(), 'rush2049-dc'), exist_ok=True)
    main()
