from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/visuals');s=(p/'baseline.c').read_text().replace('        gEnvirons[i].next = &gEnvirons[i + 1];','        int next = i + 1;\n        gEnvirons[i].next = &gEnvirons[next];');Path('src/visuals.c').write_text(s)
