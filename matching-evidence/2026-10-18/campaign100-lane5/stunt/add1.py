from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/stunt');s=(p/'baseline.c').read_text().replace('0 < plyr->numstunts','plyr->numstunts > 0');Path('src/stunt.c').write_text(s)
