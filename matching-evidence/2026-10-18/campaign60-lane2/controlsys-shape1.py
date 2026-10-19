from pathlib import Path
p=Path('src/controlsys.c');s=p.read_text().replace('if (!sRegistered) {','if (sRegistered == 0) {');p.write_text(s)
