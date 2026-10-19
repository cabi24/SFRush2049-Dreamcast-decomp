from pathlib import Path
p=Path('src/model.c');s=p.read_text().replace('#define scalef(a,b) ((a)*(b))','#define scalef(a,b) ((b)*(a))');p.write_text(s)
