from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/targets');s=(p/'baseline.c').read_text().replace('    int i; /* r0 */\n    int superGroup; /* r21 */\n    int ranweap; /* r6 */\n    char *name;','    char *name;\n    int i; /* r0 */\n    int superGroup; /* r21 */\n    int ranweap; /* r6 */',1);Path('src/targets.c').write_text(s)
