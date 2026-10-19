from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/stree');s=(p/'slist-mask-first.c').read_text().replace('cnt = *vlp++ & 0x1f;','cnt = *vlp++ & ~0xe0;');Path('src/stree.c').write_text(s)
