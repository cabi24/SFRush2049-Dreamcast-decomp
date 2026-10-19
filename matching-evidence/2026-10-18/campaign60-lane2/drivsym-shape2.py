from pathlib import Path
p=Path('src/drivsym.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/drivsym.original.c').read_text().replace('rolloff * (-m->brakegain[i] * m->brake)','rolloff * (m->brake * -m->brakegain[i])');p.write_text(s)
