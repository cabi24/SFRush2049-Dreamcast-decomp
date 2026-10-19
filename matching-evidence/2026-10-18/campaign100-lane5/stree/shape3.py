from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/stree');s=(p/'vlist-complement-mask.c').read_text().replace('    return (vlp[0] << 8) + vlp[1];','    next = (vlp[0] << 8) + vlp[1];\n    return next;',1);Path('src/stree.c').write_text(s)
