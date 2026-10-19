# Controls and drones round 2

Strict native SHC5.1r11 / frozen speed,noinline unit builds; no canonical output writes.

## Retained result

UpdateRushControls at 8C01E8F8..8C01EAEA is now fully byte exact (498 emitted bytes).
Controls: 5 -> 6 exact functions, 1190 -> 1688 emitted exact text bytes, 104 data bytes unchanged.
All five inherited exact functions preserved. Drones remains 2/4 and 46 emitted text bytes;
src/drones.c was restored byte-for-byte to its starting SHA256.

The decisive source shape splits the chained levels/edges/autoEdges zero assignment into
three statements, with the player local declared outside the loop. This changes only
register allocation, aligning the shared zero value, player pointer, counter and stick base
registers throughout all four unrolled bodies. No flags, maps, layouts or externs changed.

## Bounded unsuccessful trials

- ReadShifter: inversion via goto select reduced five different bytes to two (RTS versus
  common-epilogue BRA at 8C01E4D4). Named common return label changed numerous earlier
  branches into returns and shrank the function. Boolean gear test returned to baseline.
- ClearRushControls: declaration order, explicit shared zero, and long counter/increment
  ordering did not solve the r5/r6 zero/counter swap. The newly learned split-assignment
  mechanism is a strong follow-up, but three trial shapes had already been used.
- FindPlayerNum: commuted bit test unchanged; an edges pointer gave 76 differing bytes;
  explicit byte offset gave 80 differing bytes and size118 versus target114. The latter
  made SetControls and SetPlayerControls moved-exact, useful evidence that their bodies
  are already correct when upstream placement is fixed. All finder trials restored.
- AssignDrones: explicit first-loop model pointer (2310 differing bytes), scoped first
  short counter/chained counter initialization (2267), scoped second-phase k counter
  (2268) failed to produce an exact function. Baseline2268. All restored.
- LinearInterp is displaced by the preceding large function's size; no source changes.

## Reproduction

From C:\rush2049-dreamcast:

    python -B matching-evidence/2026-10-18/round2-controls-drones/run.py controls controls-final
    python -B matching-evidence/2026-10-18/round2-controls-drones/verify.py

verification.json captures source, expanded input, object and absolute-link SHA256 values,
the exact new address range, and assertions for full target equality, raw emitted credit,
and inherited exact-function preservation. All build artifacts are private in this packet.
Do not count this packet as integrated until the prime refreshes/audits canonical outputs.

No blockers. No commit/push, shared header/map/tool edits, installs or game execution.
