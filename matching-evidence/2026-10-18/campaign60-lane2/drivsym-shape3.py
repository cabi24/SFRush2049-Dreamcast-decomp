from pathlib import Path
p=Path('src/drivsym.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/drivsym.original.c').read_text().replace('for (i = 0, td = m->tires; i < 4; i++, td++) {','for (i = 0; i < 4; i++) {\n        td = &m->tires[i];');p.write_text(s)
