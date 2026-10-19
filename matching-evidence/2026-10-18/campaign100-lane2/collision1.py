from pathlib import Path
p=Path('src/collision.c');s=p.read_text().replace('    force = 40000.0f;\n    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];','    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];\n    force = 40000.0f;');p.write_text(s)
