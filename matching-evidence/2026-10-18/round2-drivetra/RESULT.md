# Drivetra bounded follow-up, 2026-10-18

RESULT: no new exact functions or credited bytes. Final source restored to baseline;
6/10 functions and 794 text/literal bytes remain exact under full fixed-address comparison.
No shared report, map, header, extern, audit receipt, objdiff configuration or other unit changed.

Baseline source SHA256: bdd4530d5eed19703fbd40e99a2994e62d00182690e331ae4526b9999322b399.
Native baseline: python -B matching-evidence/2026-10-18/round2-drivetra/run.py baseline
Harness calls dc_build.build_unit with private BUILD/OBJ_DIR/LINK_DIR and frozen compiler/flags.
Each label retains source.c, result.json, raw object, linked ABS and a dependency-free diagnostic listing.

The new hypothesis follows calib/drivetra/notes.md's pointer-helper lead: target explicitly
forms low_ptr+1 and spills low_ptr while expanding interpolation. Three bounded shapes tested:

| Label | Hypothesis | Torque bytes | Torque differing bytes | Exact functions / credit |
|---|---|---:|---:|---|
| baseline | existing scalar inline interp | 280 | 177 | 6 / 794 |
| pointer-helper | static inline helper taking two short pointers, direct arithmetic | 302 | 270 | 6 / 794 |
| nested-helper | two-pointer helper calling inline scalar interp | 280 | 273 | 6 / 794 |
| single-pointer-helper | single-pointer helper calling interp(a[0],a[1],...) | 270 | 271 | 6 / 794 |

All variants failed strict equality and were reverted. The pointer helper adds an r10 save at
0x8C0230B8; nested and single-pointer helpers add both r10/r9 saves. This contradicts the
target's four-register save set and fails before the interesting row interpolation block.
The two-pointer direct helper does produce explicit add #2 pointer materialization, but it
keeps the base pointer in a callee-save register rather than the target's inline temporary stack slot.
No further variants after the three-shape cap.

Residual baseline: first raw difference 0x8C0230BC is a literal displacement caused by the
short torque body. First non-pool instruction difference 0x8C0230C2: target mov.l r1,@r15,
ours mov.l r1,@(4,r15). Torque target is290 bytes; baseline280. Its shortfall moves interp
to0x8C0231C8 (target0x8C0231D2) and engine to0x8C0231DC (target0x8C0231E6).
drivetrain's lone differing byte is its BSR engine displacement at0x8C022CB6.
No new codegen claim promoted. Existing early exact functions are preserved in all variants.

Validation diagnostics: list_words.py uses the existing partial dependency-free pilot SH4
decoder solely for diagnosis; full fixed-address unmasked comparison uses dc_build itself.
Initial diagnostic script name inspect.py shadowed Python inspect and failed; renamed to
list_words.py, rerun successfully. This diagnostic failure did not affect native comparisons.
No installs, downloads, flag changes, full builds, game execution, commits or pushes.
