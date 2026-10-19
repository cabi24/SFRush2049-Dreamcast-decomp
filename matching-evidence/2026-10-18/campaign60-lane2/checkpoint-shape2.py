from pathlib import Path
p=Path('src/checkpoint.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/checkpoint.original.c').read_text().replace('    num_left = 0;\n    num_dead = 0;','    num_left = num_dead = 0;');p.write_text(s)
