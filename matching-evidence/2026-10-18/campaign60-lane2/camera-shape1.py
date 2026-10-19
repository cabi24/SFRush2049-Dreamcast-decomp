from pathlib import Path
p=Path('src/camera.c');s=p.read_text();a=s.index('void init_views(void)');b=s.index('/* 0x80006138',a);s=s[:a]+s[a:b].replace('    unsigned int state;\n','').replace('        state = gGameState;\n','').replace('if (state & 8)','if (gGameState & 8)')+s[b:];p.write_text(s)
