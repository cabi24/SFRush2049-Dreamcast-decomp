from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/stree');s=(p/'baseline.c').read_text().replace('        snum -= cnt + 1;\n        next &= 0x1fff;','        next &= 0x1fff;\n        snum -= cnt + 1;');Path('src/stree.c').write_text(s)
