from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/unk_8C055F38');s=(p/'baseline.c').read_text().replace('    if (_builtin_fabsf(x) > YMAX) {','    f = x;\n    if (_builtin_fabsf(f) > YMAX) {');Path('src/unk_8C055F38.c').write_text(s)
