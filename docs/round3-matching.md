# Round 3 accepted matching results — 2026-10-27

Fifty distinct functions received actual source-probe builds across four isolated
lanes (13 audio,13 gameplay,12 spatial,12 screens). Only independently verified
improvements are integrated.

## Strict gains

- Audio: +520 emitted bytes,2 exact functions (func_8C016210 and func_8C016538);
  full-word volume-setter contract and target-ordered instance-index rollover.
- Stunt: +1,244 emitted bytes,3 exact functions (_CheckRoll,_CheckSpin,_CheckFlip);
  truncation-safe float spellings and target-backed axis/pointer lifetimes.
  _CheckMulti remains nonexact; its natural pointer-sum dependency restores the
  pool schedule, not a separate function-match claim.44 shared pool bytes are
  additional to the three function spans and must not be counted twice.
- Visuals: +792 emitted bytes,3 exact functions (InitTrackDynamicObjs,
  InitWheelDynamicObjs,InitDynamicTextures); canonical unsigned model-count
  contract and removal of an unsupported register hint.
- Pak screen: +374 emitted bytes,1 exact function (_ControlFileDel); target
  rereads require separate increment/decrement statements before the comparisons.

Total: +2,930 emitted bytes,+9 exact functions. All earlier accepted text/data
addresses and exact-function counts are preserved. Candidate full114-unit build,
strict emitted-byte audit and six progress-credit tests passed. Independent
negative controls for the volume type, rollover value, visual loop bound, stunt
literal and delete-selection step all failed; restored candidates pass.
No compiler flags, maps, scoring masks, fabricated padding, or original-byte credit.

Final game code/literals:84,326/569,664 (14.8028%). Whole image:
153,657/1,123,492 (13.6767%). Game functions:533/1,555. Units:29exact,85partial.

Rejected candidates remain research. Optional extra36-byte _FindFile result is
technically exact but withheld: it required unsupported goto scaffolding in the
nonmatching caller. It is not included in these totals or source changes.

Verification is fixed-address unit compilation/linking against the pinned retail
executable, not a whole-game relink/runtime test or raw native-Windows artifact
equivalence claim. Original SHC5.1r11 and LNK6.0E(r13), frozen flags, explicit
pinned wibo1.2.0 are the build environment. Private executables are not published.

Dedicated proof: evidence/linux-round3-integration-20261027.json. It records
source/header/input/output hashes and disjoint emitted credited spans; tool hashes
belong to the separately reviewed Linux workflow8dee72f, not native wrappers in
this source-only commit. Historical native receipt is not current-source proof.
Private target/compiler artifacts and disassembly are excluded.
