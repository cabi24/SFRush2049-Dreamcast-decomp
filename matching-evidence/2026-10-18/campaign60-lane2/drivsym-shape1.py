from pathlib import Path
p=Path('src/drivsym.c');s=p.read_text().replace('rolloff * (-m->brakegain[i] * m->brake)','(-m->brakegain[i] * m->brake) * rolloff');p.write_text(s)
