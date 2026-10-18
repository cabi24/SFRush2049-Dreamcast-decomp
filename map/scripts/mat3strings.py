"""Extract C string literals from MAT3 sources -> raw/mat3_strings.json {string: [files]}"""
import os, re, json, glob
from common import RAW
SRC = r'C:\rush2049-mat3\decomp\src'
lit = re.compile(r'"((?:[^"\\n]|\.)*)"')
out = {}
for path in glob.glob(os.path.join(SRC, '**', '*.c*'), recursive=True) + glob.glob(os.path.join(SRC, '**', '*.h'), recursive=True):
    rel = os.path.relpath(path, SRC).replace(os.sep, '/')
    txt = open(path, encoding='latin-1').read()
    txt = re.sub(r'/\*.*?\*/', ' ', txt, flags=re.S)
    txt = re.sub(r'//[^\n]*', ' ', txt)
    for line in txt.split('\n'):
        if line.lstrip().startswith('#include'): continue
        for m in lit.finditer(line):
            s = m.group(1)
            try:
                s = bytes(s, 'latin-1').decode('unicode_escape')
            except Exception:
                pass
            if len(s) >= 3:
                out.setdefault(s, set()).add(rel)
json.dump({k: sorted(v) for k, v in out.items()}, open(os.path.join(RAW, 'mat3_strings.json'), 'w'), indent=0)
print(len(out))
