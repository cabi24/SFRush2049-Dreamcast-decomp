# Campaign60 lane3 progress - 10/10 complete

RESULT: +370 strictly exact text/literal bytes, +2 exact functions, +0 data. Retained only src/Init.c change. All prior exact spans verified preserved. Nine other source files restored byte-identically to private baselines.

| Unit | Status | Delta text/functions | Attempts and residual |
|---|---|---|---|
| gamerecords | bounded/restored |0/0|FindWinner941 baseline; outer-int-temp916, outer-char-temp941, short-indices893. Register pressure/loop/pool differences; no exact gains. |
| HAL_text | bounded/restored |0/0|Init21 baseline; semaphore-return22, volatile-index19, volatile-inited35. HAL_SetFont1 byte depends preceding _HAL_SetFont4-byte early placement. |
| headlights | bounded/restored |0/0|Forward480 baseline; array-uvs656, late-car487, product-order489; preserved Start308. Register and stack allocation differs. |
| highscorescr | bounded/restored |0/0|update379 baseline; char-exit379, short-player371, bool-exit381. Missing local _SetHighScoreState body forces external indirect call versus target internal tail branch. |
| hud | bounded/restored |0/0|updateFrame417 baseline; unrolled-time427, short-slot424,unsigned-slot417. Target first4 timer calls unrolled; source compiler rolls them and allocates16 versus12 stack bytes. All5 exact prefix functions preserved throughout. |
| Init | retained success |370/2|end-first regresses, argument-start exactly matches func_8C039280136 and Pal60234, volatile-start regresses. Selected argument-start result and verification.json. |
| unk_8C03A360 | bounded/restored |0/0|first helper6/78 and loop2/64 differences baseline. direct-static6/2, frame-temp6/2, direct-store-loop-result58/62. Register selection near indirect calls differs. |
| maxpath | bounded/restored |0/0|FindInterval7 differences all later-function-address pool values; body instructions exact. IntervalPosChange2521 baseline; velocity-array2349, compare-order2521,short-path2533. Larger frame and context placement unresolved. |
| mdrive | bounded/restored |0/0|munge_gLink_data115 baseline; declaration-order115, short-indices444, separated-increment189. Global/constant register choices and postincrement stores differ. |
| memsys | bounded/restored |0/0|InitMemory71 baseline; volatile-mainheap41,nested-allocation72,heap-local27. Bootstrap pointer/register lifetime and merged pool placement unresolved. Nested-allocation failed candidate has unsequenced global access and is explicitly rejected, never retained. |

CHANGES: Init startup now assigns aligned start inside first argument while independently evaluating the same aligned address in size argument. This is sequenced safely: start is not read by the second argument. Target evaluates heap-end arithmetic first, materializes aligned start in r0 and preserves its source local stack home. It eliminates the baseline r14 preservation and restores original constant-pool order. The shared pool lies inside Pal60's mapped extent; correcting its3 literal bytes also makes Pal60 exact. No function changes after func_8C039280.

VALIDATION: Native frozen SHC5.1r11 with -speed -noinline and existing fixed flags. Imported build_unit only; C.BUILD/OBJ_DIR/LINK_DIR point build/campaign60-lane3/<unit>/<label>. No canonical main, objdiff/global report/receipt writes. Strict full fixed-address comparisons; no masks. check.py independently asserts every baseline exact function/pool/data span remains identical. Selected Init source SHA2568d97456af1ff7ba975be5039015a286475580b4f161f63ef4d3551fb5f8ae279; object07528dd3dc85a52b2253f35863445b6d595381c6b0ed3e52c9fc061b76407557; abs e8dddf17ad6a16bc66cb42106761450435f1458c4428763dc5ae914f51b49294. Raw emitted credit only, no fill/gap/BSS credit.

Each variant subfolder retains source.c/result.json; corresponding private build holds objects/link artifacts. Reproduce: python -B matching-evidence/2026-10-18/campaign60-lane3/run.py Init LABEL; python -B matching-evidence/2026-10-18/campaign60-lane3/check.py Init argument-start.

BLOCKERS: Source-shape stop rule reached for remaining chosen functions. No tooling blocker. No shared headers/maps/externs/tooling/HANDOFF/global artifacts changed. No downloads/installs/security bypass/MAT3 writes/game/full builds/commits/pushes.
