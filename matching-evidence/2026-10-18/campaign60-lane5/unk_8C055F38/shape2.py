from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/unk_8C055F38');s=(p/'baseline.c').read_text().replace('    temp = modff(temp, &xn);\n    if (_builtin_fabsf(temp) >= 0.5) {','    if (_builtin_fabsf(modff(temp, &xn)) >= 0.5f) {');Path('src/unk_8C055F38.c').write_text(s)
