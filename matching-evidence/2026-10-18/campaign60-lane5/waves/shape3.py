from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/waves');s=(p/'baseline.c').read_text().replace(' int i;\n while (*(volatile signed char *)&gRenderInProgress) {}',' int i;\n volatile signed char *busy = &gRenderInProgress;\n while (*busy) {}',1);Path('src/waves.c').write_text(s)
