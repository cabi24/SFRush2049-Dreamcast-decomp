from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign60-lane5/waves');s=(p/'baseline.c').read_text().replace('while (*(volatile signed char *)&gRenderInProgress) {}','while (gRenderInProgress) {}');Path('src/waves.c').write_text(s)
