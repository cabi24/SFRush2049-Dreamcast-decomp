from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/TB_text');s=(p/'baseline.c').read_text().replace('#include "TB_text_types.h"','#pragma inline(TB_CenterText,TB_RightText,TB_GetFontHeight)\n#include "TB_text_types.h"',1);Path('src/TB_text.c').write_text(s)
