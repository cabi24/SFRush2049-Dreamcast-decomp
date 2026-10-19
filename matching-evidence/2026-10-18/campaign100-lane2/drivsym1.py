from pathlib import Path
p=Path('src/drivsym.c');s=p.read_text();s=s.replace('    float temp[3]; /* stack */\n    float temp1[3]; /* stack */','    float temp1[3]; /* stack */\n    float temp[3]; /* stack */',1);p.write_text(s)
