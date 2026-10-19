from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/TB_text');s=(p/'baseline.c').read_text().replace('    } else while ((*s1++ = *s2++) != 0) {}\n    return s;','    } else {\n        unsigned char c;\n        do { c = *s2++; *s1++ = c; } while (c);\n    }\n    return s;',1);Path('src/TB_text.c').write_text(s)
