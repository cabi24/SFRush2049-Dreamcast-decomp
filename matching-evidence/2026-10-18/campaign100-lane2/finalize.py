from pathlib import Path
p=Path('matching-evidence/2026-10-18/campaign100-lane2');s=Path('matching-evidence/2026-10-18/campaign60-lane2/validate.py').read_text().replace('campaign60-lane2','campaign100-lane2').replace(' game gameoverscr',' game').replace("label='selected' if u in ('fmath','game') else 'baseline'","label='final'").replace("if label=='baseline':assert", "assert").replace("'assignments':10,'completed':10", "'assignments':17,'completed':17");s=s.replace("'object_sha256':sha(root/'obj'/(u+'.obj')),", "'compiler_inputs':{str(f.relative_to(root)):sha(f) for f in root.rglob('*') if f.is_file() and f.suffix.lower() in ('.c','.i','.src')},'object_sha256':sha(root/'obj'/(u+'.obj')),");(p/'validate.py').write_text(s)
(p/'README.md').write_text('''# Campaign100 lane2

All17 assignments addressed with bounded evidence. No retained source change; strict function/text/data delta0. The nine `.original.c` snapshots are the entry source state, including prior campaign wins. Every rejected trial was restored.

Use `python matching-evidence/2026-10-18/campaign100-lane2/run.py UNIT LABEL` from repository root for isolated native SHC5.1r11 compile/link. This calls build_unit only and redirects all generated outputs below build/campaign100-lane2. Do not rerun baseline because its original snapshot is guarded.

Results are recorded per trial JSON and in progress.md. No moved-function, mask, alignment, fill or BSS credit is claimed.

Strongest fresh lead: forces1. Starting from drivsym.original.c, drivsym2.py adds traction=1.0f as all four dotireforce final arguments, recovering target fr13 liveness. Then drivsym4.py replaces default poortract assignment with explicit else, recovering target conditional sequence. Finally drivsym5.py changes wind multiplication operand order, recovering most wind body. Baseline1063 differing bytes, traction391, explicitelse464, wind-order62; no whole-span credit and restored at three-shape cap. Target residual at8C024114 uses m base plus88 and offset92 instead of field-base plus4; at8C024164 and8C024174 onward drag expression has different register/load scheduling. Reproduce recipes sequentially only from owned original snapshot. Private trial binary remains available.

Game _UpdateGame target at8C027E04 branches to loop comparison; game1.py changes allSolid loop to explicit bottom test. Differences1265 to758 without strict credit; restored.

Final nine native rebuilds use final label; validate.py independently rereads ABS images, raw-compares every old exact function/non-BSS segment, checks source snapshots and hashes source, compiler inputs, objects and ABS. manifest.json is final validation record. No canonical build/audit files modified.
''')
