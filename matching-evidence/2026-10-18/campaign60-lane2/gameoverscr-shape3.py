from pathlib import Path
p=Path('src/gameoverscr.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/gameoverscr.original.c').read_text().replace('    int *requested = &newState;\n','').replace('*requested','newState');p.write_text(s)
