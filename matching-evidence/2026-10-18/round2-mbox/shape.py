from pathlib import Path
p=Path('matching-evidence/2026-10-18/round2-mbox');s=(p/'baseline.c').read_text();s=s.replace('    unsigned int camflags;\n    camflags = (camlink << 8) ^ 0xf00;\n    dlidx = ZOID_AddObject(obj, mat, parent, flags | camflags);','    dlidx = ZOID_AddObject(obj, mat, parent, flags | ((camlink << 8) ^ 0xf00));');Path('src/mbox.c').write_text(s)
