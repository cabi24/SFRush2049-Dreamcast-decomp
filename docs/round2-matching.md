# Round 2 accepted matching results

Forty distinct targets completed in four isolated lanes. Only reviewed gains are
integrated: ShowHUDExtra (+382 emitted bytes), modescr uninitScreen (42),
initButtons (710), uninitButtons (72), updateButtons (1084), plus 20 emitted
tail-pool bytes and 72 external literal-pool bytes. Total +2,382 bytes and
five exact functions. animText remains nonmatching; its emitted literal pool
credit is separate, not a function match.

Full independent 114-unit compile/link and strict emitted-byte audit passed.
All prior accepted text/data addresses and function counts are preserved.
Negative controls for ShowHUDExtra, mode teardown, cursor constant and a
literal pool failed; restored candidates pass. Six progress-credit tests pass.
No flags, boundaries, masks, padding or copied original bytes were introduced.

Current game code/literals:81,396/569,664 (14.2884%). Image:150,727/1,123,492
(13.4159%). Game functions:524/1,555. Units:29 exact,85 partial.

Reference strategy: N64-shaped pane half-size expressions enabled ShowHUDExtra;
MAT3/source structure plus target-backed declaration/cursor store-order evidence
enabled mode-screen matches. References remain read-only reconstructions.

Only src/hudextra.c, src/modescr.c and private include/modescr_types.h change.
Rejected physics/shots/HUD/frontend experiments remain separate research, not
canonical code. No whole-game relink/runtime or native raw-object parity claim.

Verification environment: original SHC 5.1r11 and H-series LNK 6.0E (r13), frozen
flags, explicitly selected wibo 1.2.0 Linux x86_64. Tool hashes in the dedicated
receipt correspond to the separately reviewed Linux workflow8dee72f, not the
native wrappers in this source-only master commit. Private compiler/target files
are not published. The historical native strict-integration receipt predates these
source changes. Reproduce using the separate Linux workflow or fresh native builds.
