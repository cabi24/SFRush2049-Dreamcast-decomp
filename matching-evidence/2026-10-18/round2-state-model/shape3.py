from pathlib import Path
p=Path('src/state.c');s=Path('matching-evidence/2026-10-18/round2-state-model/state.original.c').read_text();a=s.index('    } else if (gGameState & 0x100) {');s=s[:a]+s[a:].replace('int p;','signed short p;',1);p.write_text(s)
