from pathlib import Path
p=Path('src/checkpoint.c');s=p.read_text().replace('    num_left = 0;\n    num_dead = 0;','    num_dead = 0;\n    num_left = 0;');p.write_text(s)
