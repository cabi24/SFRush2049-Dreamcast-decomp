from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/targets');s=(p/'baseline.c').read_text();s=s.replace('    if (fromWorld) {\n        name = obj;\n        dlflags', '    name = obj;\n    if (fromWorld) {\n        dlflags',1).replace('    } else {\n        name = obj;\n        dlflags = 0;', '    } else {\n        dlflags = 0;',1);Path('src/targets.c').write_text(s)
