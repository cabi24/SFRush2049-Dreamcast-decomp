# Password bounded matching pass, 2026-10-18

RESULT: No new strict matches. The canonical source was never edited.

Baseline reproduced 5/6 exact functions, 884 earned text/literal bytes and
305 initialized-data bytes using a private native compile and fixed-address
link. StorePasswordElement at 8C044CA4..8C044D0C remains 26 bytes different
out of 104, with the correct linked start and size. All other functions
remain exact in the baseline. No masks or target-byte substitutions were used.

Three source-only variants were tested, reaching the stop rule:

| Variant | Hypothesis | Strict outcome |
|---|---|---|
| loop-update | Move gNumBits increment into the for-loop update expression alongside b++ to alter scheduler ordering | Identical residual: 26/104 bytes; 5 exact functions, 884 text, 305 data |
| body-counter | Move b++ into the loop body before data >>= 1 so the source order matches the target's early counter increment | Identical residual: 26/104 bytes; 5 exact functions, 884 text, 305 data |
| split-index | Materialize bit = gNumBits & 7 before index = gNumBits >> 3, reflecting the target's interleaved bit and address preparation | Worse: StorePasswordElement becomes 98 bytes, 90/104 differ at fixed addresses; all later functions move; 0 exact functions/text, 305 data |

The target and baseline agree through 8C044CDE. All remaining differences
are in 8C044CE0..8C044CFE. Target copies r14 to r3 at CE0, and applies
the r3 mask between the index shifts. The target increments the loop
counter and shifts data before OR/store, then reloads and increments
gNumBits and branches with BF. Baseline performs the OR/store earlier and
uses a delayed BF/S with the gNumBits write in its delay slot. This pass
did not identify a source shape reproducing that schedule.

VALIDATION: `python -B matching-evidence/2026-10-18/work-password/run.py LABEL`
for LABEL baseline, loop-update, body-counter, split-index. Each private
result.json records full exact function results. run.py imports existing
dc_build functions but never calls main, redirects BUILD/OBJ_DIR/LINK_DIR
to the private variant directory, and compiles a private expanded source.
Compiler remains native SHC 5.1r11 with frozen base flags and
--no-default-flags -- -speed -noinline. The existing native linker uses
the map's real P/D/B/C starts and existing external symbol definitions.
No shared build, report, receipt, map, headers, objdiff or HANDOFF writes.

Raw word comparison: `python -B matching-evidence/2026-10-18/work-password/words.py`.
Capstone is absent in this Python environment; no install or bypass was attempted.

CHANGES: Added only this report, the private harness, word comparison helper
and their private compiled/source/results packets beneath work-password.
Source src/password.c remains unchanged with SHA-256
29900864cca050cb808034abf25087beced6976632afd3dd0325980026b7e124.

BLOCKERS: Source scheduling residual remains; three-variant stop rule reached.
Private output files are evidence only and are not integrated progress.
