from pathlib import Path
p=Path('src/controlsys.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/controlsys.original.c').read_text().replace('static int sRegistered = 0','static volatile int sRegistered = 0',1);p.write_text(s)
