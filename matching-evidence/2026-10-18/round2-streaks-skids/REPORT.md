# Round 2 streaks / skids

RESULT: +52 strictly exact emitted text/literal bytes; zero new functions. All existing exact functions preserved. Only src/streaks.c retained change; src/skids.c restored byte-for-byte baseline.

Streaks: splitting byte subtraction into an int intermediate removes the 0x00E9 constant from 8C098F36 and makes full pool 8C098F32..8C098F68 strictly equal at its real address. 52 emitted bytes count; two linker-fill bytes do not. Four exact functions remain; exact_text_bytes1116 ->1168, data16 unchanged. _AnimateStreakPolys remains differing26/188 bytes versus baseline23/188: residual is register allocation and constant subtraction in the alpha update, not a new exact function. Prior exact function/pool/data ranges are preserved. Existing B extent mismatch unchanged.

Three evidence-backed shapes: int intermediate (retained strict pool gain); compound subtraction unsigned cast (61 diff bytes, shifts later functions -4; discarded); plain compound subtraction (same61 diff and shifts; discarded). Target retains fade23 in r0 and subtracts it from signed-byte reload; baseline emits add233 through a word pool. Int temp avoids modulo-byte optimization/pool constant but retains unsigned extension and add-23.

Skids: _GetSkid baseline86/294 diff. Assignment order reversal87; candidate reuse86; direct removal163 plus regression of _UpdateSkid1 byte. All discarded. Target stores farthest candidate in r14 throughout scan and camera in r10; baseline uses r10 for candidate and r1 camera, also two-byte loop scheduling difference.

AnimateSkids baseline208/240 diff. Direct release132 (saved private, discarded); for-nexts form206 (discarded); explicit bottom-test goto232 (discarded). Target has a4-byte stack next pointer, hoists separate remove/update callbacks, keeps255 in r11 and192 in r12. All three shapes exhausted. No exact gain, restored skids baseline SHA2565d857fbd7f8bee92adaab40cd5331962c4a3e9ba547972044daef7502c8dead6.

VALIDATION: native frozen SHC5.1r11, -speed -noinline, isolated build_unit runner; no canonical main/global reports. Each named variant directory contains source.c, result.json, private object and fixed-address linked artifacts. verification.json records selected source/object/abs hashes and independent raw SYSROF pool-byte equality assertions. Reproduce: python -B matching-evidence/2026-10-18/round2-streaks-skids/run.py streaks LABEL; python -B matching-evidence/2026-10-18/round2-streaks-skids/verify.py.

BLOCKERS: no tooling blocker. Missing function matches parked at three shapes each. Shared headers/maps/externs/global outputs untouched. No full builds, execution, downloads, commits, or pushes.
