#!/usr/bin/env python3
"""Compile one C file with a Hitachi SHC package using the decomp.me Dreamcast recipe
and convert the SYSROF object to an ELF32 (little-endian SH) relocatable object.

Recipe (mirrors decomp.me cromper `DREAMCAST_CC`):
  - source is converted to CRLF
  - the package's bin/ sits in the work directory; SHC_LIB=. SHC_TMP=. (SHC 5.0 insists on this)
  - shc.exe <src> -comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis
            -string=const [-aggressive=2 -extra=asm=1800 -pic=0 ...] -object=<obj>
    (shc-v5.0r10 does not accept -fpu=single / -aggressive=2)
  - rof2elf.py <obj> <elf> --isa=sh4 (run in-process since 2026-10-20; same code, same output)

Work directories (2026-10-20): the package bin/ is copied once into a persistent slot under
%TEMP%/rush2049-dc/shc-work/<version>/slotN/ and verified by SHA-256 on every call; a compile
only writes dos_src.c there and reads dos_src.obj back. Each slot is held with an exclusive
file lock for the duration of one compile, so concurrent callers take different slots and never
collide. (The old per-call temp copy cost about 1.5 s per compile: the freshly copied
executables were scanned on every launch.) The compiler's own view is unchanged: binaries in
the current directory, SHC_LIB=. SHC_TMP=., the same argv.

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
import time

TOOLS = os.path.dirname(os.path.abspath(__file__))
ROF2ELF = os.path.join(TOOLS, 'rof2elf', 'rof2elf.py')
BASE_FLAGS = ['-comment=nonest', '-cpu=sh4', '-division=cpu', '-fpu=single', '-endian=little',
              '-macsave=0', '-sjis', '-string=const']
DECOMPME_DEFAULT_EXTRA = ['-aggressive=2', '-extra=asm=1800', '-pic=0']
WORK_ROOT = os.path.join(tempfile.gettempdir(), 'rush2049-dc', 'shc-work')
# files the compiler reads/writes in its work directory for one compile
COMPILE_FILES = ('dos_src.c', 'dos_src.obj')


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        for chunk in iter(lambda: f.read(1 << 20), b''):
            h.update(chunk)
    return h.hexdigest()


# ---------------------------------------------------------------- slots

def _try_lock(fd):
    """Exclusive non-blocking lock on an open file; released when the fd is closed or the
    process ends. Returns True when acquired."""
    try:
        if os.name == 'nt':
            import msvcrt
            msvcrt.locking(fd, msvcrt.LK_NBLCK, 1)
        else:
            import fcntl
            fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
        return True
    except OSError:
        return False


def package_manifest(bindir):
    """{file name: sha256} of every file in the package bin/ (sorted by name)."""
    return {name: sha256(os.path.join(bindir, name))
            for name in sorted(os.listdir(bindir)) if os.path.isfile(os.path.join(bindir, name))}


def ensure_slot_binaries(slot, bindir, manifest):
    """Make the slot hold exactly the package binaries: copy any file that is missing or whose
    hash differs. Returns the number of files copied."""
    copied = 0
    for name, digest in manifest.items():
        dst = os.path.join(slot, name)
        if not os.path.isfile(dst) or sha256(dst) != digest:
            shutil.copyfile(os.path.join(bindir, name), dst)
            if sha256(dst) != digest:
                sys.exit(f'[shc_cc] copy of {name} into {slot} does not verify')
            copied += 1
    for name in COMPILE_FILES:
        p = os.path.join(slot, name)
        if os.path.exists(p):
            os.remove(p)
    return copied


def acquire_slot(version, bindir, work_root=WORK_ROOT):
    """Lock the first free persistent slot for this package version, verify its binaries and
    return (slot_dir, lock_fd, files_copied)."""
    manifest = package_manifest(bindir)
    vroot = os.path.join(work_root, version)
    os.makedirs(vroot, exist_ok=True)
    n = 0
    while True:
        slot = os.path.join(vroot, f'slot{n}')
        os.makedirs(slot, exist_ok=True)
        fd = os.open(os.path.join(slot, '.lock'), os.O_RDWR | os.O_CREAT)
        if _try_lock(fd):
            try:
                copied = ensure_slot_binaries(slot, bindir, manifest)
            except BaseException:
                os.close(fd)
                raise
            return slot, fd, copied
        os.close(fd)
        n += 1
        if n > 64:
            sys.exit(f'[shc_cc] no free compiler slot under {vroot} (64 busy?)')


def rof_to_elf(obj, out):
    """`python -I rof2elf.py <obj> <out> --isa=sh4`, called in-process (saves one interpreter
    start per compile; rof2elf is standard-library Python and deterministic)."""
    from pathlib import Path
    sys.path.insert(0, os.path.dirname(ROF2ELF))
    import rof2elf  # noqa: E402
    rof2elf.make_elf(Path(obj), Path(out), rof2elf.EhdrFlags.from_string('sh4'), 0)


# ---------------------------------------------------------------- main

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('source')
    ap.add_argument('-o', '--output', required=True, help='output ELF object path')
    ap.add_argument('-v', '--version', default='shc-v5.1r11', help='package dir name under tools/shc (default shc-v5.1r11)')
    ap.add_argument('--launch', choices=['native'], default='native')
    ap.add_argument('--no-default-flags', action='store_true', help='omit -aggressive=2 -extra=asm=1800 -pic=0')
    ap.add_argument('--keep', action='store_true', help='leave dos_src.c/.obj in the slot (prints its path)')
    ap.add_argument('--keep-obj', help='also copy the raw SYSROF .obj to this path')
    ap.add_argument('--work-root', default=WORK_ROOT, help=f'persistent slot root (default {WORK_ROOT})')
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

    t0 = time.perf_counter()
    work, lock_fd, copied = acquire_slot(args.version, bindir, args.work_root)
    try:
        src_text = open(args.source, 'rb').read().replace(b'\r\n', b'\n').replace(b'\n', b'\r\n')
        open(os.path.join(work, 'dos_src.c'), 'wb').write(src_text)
        argv_tail = ['dos_src.c'] + flags + ['-object=dos_src.obj']

        def run_native():
            env = dict(os.environ, SHC_LIB='.', SHC_TMP='.')
            return subprocess.run([os.path.join(work, 'shc.exe')] + argv_tail, cwd=work, env=env,
                                  capture_output=True, text=True, timeout=120)

        # Native-only safety gate: a denial is terminal. No WSL/wibo fallback.
        t1 = time.perf_counter()
        try:
            r = run_native()
            mode_used = 'native'
        except OSError as exc:
            sys.exit('[NATIVE_EXECUTION_BLOCKED] package=' + shc + ' source=' + args.source + ' action=compile winerror=' + str(getattr(exc, 'winerror', None)) + ' reason=' + str(exc))
        t2 = time.perf_counter()

        obj = os.path.join(work, 'dos_src.obj')
        banner = ' '.join((r.stdout + r.stderr).split())
        if r.returncode != 0 or not os.path.isfile(obj):
            sys.exit(f'[shc_cc] {args.version} ({mode_used}) failed rc={r.returncode}: {banner}')
        if banner:
            print('[shc_cc] compiler diagnostics: ' + banner)
        if args.keep_obj:
            shutil.copy(obj, args.keep_obj)
        os.makedirs(os.path.dirname(os.path.abspath(args.output)) or '.', exist_ok=True)
        try:
            rof_to_elf(obj, os.path.abspath(args.output))
        except Exception as exc:
            sys.exit(f'[shc_cc] rof2elf failed: {type(exc).__name__}: {exc}')
        t3 = time.perf_counter()
        print(f'[shc_cc] {args.version} ({mode_used}) shc.exe sha256={sha256(shc)[:16]} flags={" ".join(flags)}')
        print(f'[shc_cc] wrote {args.output}  slot={os.path.basename(work)} copied={copied} '
              f'setup={t1 - t0:.3f}s compile={t2 - t1:.3f}s rof2elf={t3 - t2:.3f}s')
        if args.keep:
            print(f'[shc_cc] work dir: {work}')
    finally:
        if not args.keep:
            for name in COMPILE_FILES:
                p = os.path.join(work, name)
                if os.path.exists(p):
                    os.remove(p)
        os.close(lock_fd)


if __name__ == '__main__':
    main()
