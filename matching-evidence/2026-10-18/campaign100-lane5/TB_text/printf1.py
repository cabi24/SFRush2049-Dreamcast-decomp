from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane5/TB_text');s=(p/'baseline.c').read_text();s=s.replace('void TB_DrawText(signed short x, signed short y, void * text)\n{','#pragma inline(TB_CenterText,TB_RightText,TB_GetFontHeight)\nvoid TB_DrawText(signed short x, signed short y, void * text)\n{',1);Path('src/TB_text.c').write_text(s)
