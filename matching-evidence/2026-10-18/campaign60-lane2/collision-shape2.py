from pathlib import Path
p=Path('src/collision.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/collision.original.c').read_text();s=s.replace('    float vec[3]; /* stack */\n    float pos[3]; /* stack */\n    float posr[3]; /* stack */','    float posr[3]; /* stack */\n    float pos[3]; /* stack */\n    float vec[3]; /* stack */',1);p.write_text(s)
