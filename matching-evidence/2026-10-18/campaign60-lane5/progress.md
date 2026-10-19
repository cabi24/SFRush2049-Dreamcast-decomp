# Campaign 60 lane 5 - complete

| Unit | Status | Function delta | Text delta | Findings |
|---|---|---:|---:|---|
| stree | complete | +4 | +558 | Split outside assignment/test fixes earlier literal pool positions. Three trials; downleaf increment shape discarded. Four new exact functions; earlier FVecInterp/pool/data retained. |
| stunt | complete | +1 | +152 | Scoped absW initialization to else branch; fixes _CheckAirTime pool. Direct global form regressed old matches and discarded. _CheckRoll remains65 differing bytes; existing6 exact functions retained. |
| unk_8C055F38 | complete | +0 | +0 | Three failed shapes: register parameter, inline modff/fabs, swapped stack declaration order. Restored. Baseline155 differing bytes; unchanged56 literal credit. Emission size includes56 pool bytes so claimed58 extra body bytes in initial progress was overly simplistic. |
| targetpaths | complete | +0 | +0 | Ordered acceleration multiply statements reduced placement difference but no exact gain. Restored. First unmatched PlacePathTargetPos still495 differing bytes, shifted downstream functions. |
| targets | complete | +0 | +0 | Hoisting SetupTarget name assignment unchanged. Restored. initTargetStructs differs only in26 pool displacement bytes; SetupTarget first block schedules pool4 bytes early; broad register/stack differences remain. |
| TB_text | complete | +3 | +336 | Three destination-lifetime shapes. Declaring saved source before destination pointer fixes Strcat and upstream/downstream pools. +Strlen58, Strcat228, Sprintf50. Prior10 exact functions and329 data bytes retained. |
| videoscr | complete | +0 | +0 | Signed direction local yields unchanged code. Restored. Broad updateVideoScreen function-pointer caching/register and early size drift block moved validOption; no isolated safe win. |
| visuals | complete | +1 | +282 | Explicit next=i+1 local preserves separate target index calculations and fixes InitEnvs plus adjacent112 pool bytes. Removing register index had no effect. All earlier44 literal and600 data bytes preserved. |
| waves | complete | +0 | +0 | Three busy-wait shapes (direct global, assigned local, pointer local) leave9/7/6 differing bytes in InitWaves; unaccepted/restored. All differences are register selection near entry/calls; old20 functions retained. |
| unk_8C065F2C | complete | +1 | +346 | Walking part-mask pointers, matching target pointer increments, eliminates redundant zero no-clip OR test. Fixes following fog function and literal pool (+346). First visibility function remains181 differing bytes; old258 text and12223 data bytes preserved. |

Total: +10 exact functions and +1674 text/literal bytes, no initialized-data delta. All previous exact function/pool/data address sets preserved.

Validation: python matching-evidence/2026-10-18/campaign60-lane5/run.py UNIT LABEL; final native links for retained5 units. python matching-evidence/2026-10-18/campaign60-lane5/finalize.py validates hashes, old exact-span subsets and every resulting exact byte against original target. Receipt stores source/object/absolute/preprocessed hashes.

Only src/stree.c, src/stunt.c, src/TB_text.c, src/visuals.c and src/unk_8C065F2C.c retain changes. Other5 restored byte-for-byte. Private build outputs are under build/campaign60-lane5/. No shared reports, headers, maps, tools, receipts or canonical objdiff modified. No masks, original-byte injection, fill/BSS credit, full builds, installs or commits.
