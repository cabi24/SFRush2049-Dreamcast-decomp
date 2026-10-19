from pathlib import Path
p=Path('src/camera.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/camera.original.c').read_text();a=s.index('void init_views(void)');b=s.index('/* 0x80006138',a);s=s[:a]+s[a:b].replace('state = gGameState','state = *(volatile unsigned int *)&gGameState')+s[b:];p.write_text(s)
