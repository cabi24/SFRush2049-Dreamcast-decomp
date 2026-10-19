from pathlib import Path
p=Path('src/gameoverscr.c');s=p.read_text().replace('    register int *state = &gGameOverState;\n    int *requested = &newState;\n','').replace('*state','gGameOverState').replace('*requested','newState');p.write_text(s)
