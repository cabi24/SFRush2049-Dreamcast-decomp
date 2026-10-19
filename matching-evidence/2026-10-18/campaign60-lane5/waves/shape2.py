from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/waves');s=(p/'baseline.c').read_text().replace(' int i;\n while (*(volatile signed char *)&gRenderInProgress) {}',' int i;\n signed char busy;\n while ((busy = gRenderInProgress) != 0) {}',1);Path('src/waves.c').write_text(s)
