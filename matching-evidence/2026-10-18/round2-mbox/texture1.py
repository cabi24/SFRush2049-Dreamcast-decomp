from pathlib import Path
p=Path('matching-evidence/2026-10-18/round2-mbox');s=(p/'baseline.c').read_text();s=s.replace('    int i; /* r26 */','    register int i; /* r26 */').replace('    tTexture *ptr; /* r31 */','    register tTexture *ptr; /* r31 */');Path('src/mbox.c').write_text(s)
