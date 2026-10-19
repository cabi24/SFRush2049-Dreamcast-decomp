from pathlib import Path
p=Path('src/camera.c');s=Path('matching-evidence/2026-10-18/campaign100-lane2/camera.original.c').read_text();a=s.index('void init_view3(tCarData *car)\n{');b=s.index('/* 0x800061b4',a);part=s[a:b].replace('unsigned int state','int state');p.write_text(s[:a]+part+s[b:])
