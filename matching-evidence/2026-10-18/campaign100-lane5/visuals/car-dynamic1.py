from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/visuals');s=(p/'baseline.c').read_text().replace('    int i; /* r27 */\n    char partName[20]; /* stack */\n    int objIndex;','    int objIndex;\n    int i; /* r27 */\n    char partName[20]; /* stack */');Path('src/visuals.c').write_text(s)
