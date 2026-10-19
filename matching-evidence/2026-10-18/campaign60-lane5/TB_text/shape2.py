from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/TB_text');s=(p/'baseline.c').read_text().replace('unsigned char *s1=str1; unsigned char temp[256];','register unsigned char *s1=str1; unsigned char temp[256];');Path('src/TB_text.c').write_text(s)
