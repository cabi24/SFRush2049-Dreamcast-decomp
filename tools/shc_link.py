"""Fixed-address mini-link with the Hitachi H-series linkage editor, then strict comparison.

    python shc_link.py --obj a.obj [--obj b.obj ...] --start P=8C03A1B8 [--start C=...] \
        [--lnk-version shc-v5.1r13] [--out-dir DIR] [--name NAME]

Runs LNK.EXE with a generated subcommand file (INPUT/OUTPUT/FORM A/START/PRINT/EXIT), reads the
absolute SYSROF output with sysrof_abs.py, writes <name>.abs/.map/.raw.json and compares every
located data run against the pinned target image byte for byte (no masking). The target is
pilot/orig/1ST_READ.BIN at 0x8C010000. A pass is an exact local link for those runs only.
"""
import argparse, hashlib, json, os, shutil, subprocess, sys, tempfile

TOOLS = os.path.dirname(os.path.abspath(__file__))
DC = os.path.dirname(TOOLS)
sys.path.insert(0, TOOLS)
from sysrof_abs import read_abs  # noqa: E402

TARGET = os.path.join(DC, 'pilot', 'orig', '1ST_READ.BIN')
BASE = 0x8C010000


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--obj', action='append', required=True, help='SYSROF object (repeatable, link order)')
    ap.add_argument('--start', action='append', default=[], help='SECTION=HEXADDR, e.g. P=8C03A1B8 (repeatable)')
    ap.add_argument('--lnk-version', default='shc-v5.1r13', help='package whose LNK.EXE to use')
    ap.add_argument('--out-dir', default=os.path.join(DC, 'calib', 'link'))
    ap.add_argument('--name', default='link')
    ap.add_argument('--entry', help='ENTRY symbol (optional)')
    ap.add_argument('--define', action='append', default=[], help='SYMBOL=HEXADDR for undefined externals (repeatable)')
    args = ap.parse_args()

    bindir = os.path.join(TOOLS, 'shc', args.lnk_version, 'bin')
    lnk = next((f for f in os.listdir(bindir) if f.lower() == 'lnk.exe'), None)
    if not lnk:
        sys.exit(f'no LNK.EXE in {bindir}')
    os.makedirs(args.out_dir, exist_ok=True)
    work = tempfile.mkdtemp(prefix='lnk_', dir=os.path.join(tempfile.gettempdir(), 'rush2049-dc'))
    try:
        for f in os.listdir(bindir):
            p = os.path.join(bindir, f)
            if os.path.isfile(p):
                shutil.copy(p, work)
        inputs = []
        for i, o in enumerate(args.obj):
            local = f'in{i}.obj'
            shutil.copy(o, os.path.join(work, local))
            inputs.append(local)
        sub = [f'INPUT {",".join(inputs)}', f'OUTPUT {args.name}.abs', 'FORM A']
        for s in args.start:
            sec, addr = s.split('=')
            sub.append(f'START {sec}({addr.upper()})')
        for d in args.define:
            name, val = d.split('=')
            sub.append(f'DEFINE {name}({val.upper()})')
        if args.entry:
            sub.append(f'ENTRY {args.entry}')
        sub += [f'PRINT {args.name}.map', 'EXIT']
        open(os.path.join(work, 'link.sub'), 'wb').write(('\r\n'.join(sub) + '\r\n').encode())
        env = dict(os.environ, SHC_LIB='.', SHC_TMP='.')
        r = subprocess.run([os.path.join(work, lnk), '-SUBCOMMAND=link.sub'], cwd=work, env=env,
                           capture_output=True, text=True, timeout=120)
        log = ' '.join((r.stdout + r.stderr).split())
        abs_path = os.path.join(work, f'{args.name}.abs')
        if r.returncode != 0 or not os.path.exists(abs_path):
            sys.exit(f'[shc_link] LNK failed rc={r.returncode}: {log[-400:]}')
        for ext in ('.abs', '.map'):
            src = os.path.join(work, args.name + ext)
            if os.path.exists(src):
                shutil.copy(src, os.path.join(args.out_dir, args.name + ext))
        sections, runs = read_abs(abs_path)
        target = open(TARGET, 'rb').read()
        report = {'name': args.name, 'lnk': f'{args.lnk_version}/{lnk}', 'subcommands': sub,
                  'objects': [{'path': o, 'sha256': hashlib.sha256(open(o, 'rb').read()).hexdigest()} for o in args.obj],
                  'sections': [{'name': n, 'address': f'{a:08X}', 'length': l} for n, a, l in sections],
                  'runs': [], 'all_exact': True}
        for sec, addr, chunk in runs:
            off = addr - BASE
            want = target[off:off + len(chunk)] if 0 <= off < len(target) else b''
            exact = want == chunk
            ndiff = sum(1 for a, b in zip(chunk, want) if a != b) + abs(len(chunk) - len(want))
            first = next((i for i, (a, b) in enumerate(zip(chunk, want)) if a != b), None)
            report['runs'].append({'section': sections[sec][0] if sec is not None and sec < len(sections) else sec,
                                   'address': f'{addr:08X}', 'size': len(chunk), 'exact': exact,
                                   'diff_bytes': ndiff, 'first_diff_offset': first,
                                   'sha256': hashlib.sha256(chunk).hexdigest()})
            report['all_exact'] &= exact
            print(f'[shc_link] {sections[sec][0] if sec is not None else "?"} {addr:08X} +{len(chunk):#x}: '
                  + ('EXACT' if exact else f'{ndiff} bytes differ (first at +{first:#x})'))
        json.dump(report, open(os.path.join(args.out_dir, args.name + '.json'), 'w'), indent=1)
        print(f'[shc_link] report: {os.path.join(args.out_dir, args.name + ".json")}  all_exact={report["all_exact"]}')
        return 0 if report['all_exact'] else 1
    finally:
        shutil.rmtree(work, ignore_errors=True)


if __name__ == '__main__':
    sys.exit(main())
