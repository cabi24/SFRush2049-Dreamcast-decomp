from pathlib import Path
p=Path('src/camera.c');s=p.read_text();a=s.index('void init_view3(tCarData *car)\n{');b=s.index('/* 0x800061b4',a);part=s[a:b].replace('    unsigned int state;\n','').replace('    state = gGameState;\n','').replace('if (state & 8)','if (gGameState & 8)');p.write_text(s[:a]+part+s[b:])
