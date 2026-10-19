from pathlib import Path
p=Path('matching-evidence/2026-10-18/round2-mbox');s=(p/'baseline.c').read_text();a=s.index('tTexture *MBOX_FindTexture_WORK');b=s.index('/* Anonymous mapped entry',a);t=s[a:b].replace('signed char lastModel','register signed char lastModel');s=s[:a]+t+s[b:];Path('src/mbox.c').write_text(s)
