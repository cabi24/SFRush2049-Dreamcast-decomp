from pathlib import Path
p=Path('src/fmath.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/fmath.best.c').read_text().replace('float mag = sqrtf(v[2]*v[2]+v[0]*v[0]+v[1]*v[1]);','float mag = func_8C0262D2(v[0], v[1], v[2]);');p.write_text(s)
