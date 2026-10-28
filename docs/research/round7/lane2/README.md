# Round 7 lane 2 — no accepted gains

Baseline: `6fb2d7e` (Linux backend restoration). Final source-only research state changes two private declarations; all source-body experiments were reverted.

## Results and counting
- **0 accepted matches or bytes.** Still 579 exact game functions; 89,978 game bytes; 159,309 image bytes.
- **8 distinct qualifying target attempts:** six fresh changed-body targets and two materially new canonical-ABI declaration revisits. Neutral spelling experiments do not add attempts.
- 32 placement-only targets were inspected, not counted. `_AnimateKnockdown` was inspected but blocked by an unexplained unused native 48-byte frame. No dummy local or padding was added.
- `enginetorque` trials are retained transparently but excluded: historical pointer-lifetime source shapes already exist in round2 evidence. No claim that the candidate quota or 500-function request was completed.
- No negative control for accepted gain exists because no gain is proposed.

## Research-only declaration correction
`src/timer.c` defines `volatile float gFrameSeconds`. The mdrive and targetpaths private declarations omitted volatile. Only `src/mdrive.c` and `src/targetpaths.c` consume those headers; both consumers and all 114 units were rebuilt. Native stopped-car code reads frame time within the lap loop; the definition and native read support testing consistent object qualification, not arbitrary volatile tuning. No object size/layout/type-width changes were made.

The qualifier probe worsened strict stopped-car mismatch from 325 to 328 bytes and path animation from 2018 to 2021. This is research only, not a matching promotion. It does not establish recovery of the original header. Incidental changes in `_PlacePathTargetPos` are not counted separately.

## Verification
Full 114-unit build, strict receipt audit and 22 host tests passed. All baseline exact function identities, per-unit exact counts and credited spans were preserved. `preservation.json` and `research-receipt.json` record totals and hashes.

## Evidence interpretation
`attempt-ledger.json` records actual hypotheses and before/after strict reports. Sequential trial “before” fields may refer to the preceding rejected variant; baseline values are given below. Raw object-change hashes may include metadata; they alone do not establish a changed body. `holds.json` is relaxed read-only relocation triage, never strict proof or credit. Source-only variants are provided for replay; private target bytes and raw disassembly are deliberately excluded.

Baseline → best observed differing bytes (no exact gains): FindTempNameIdx 5→5; SaveGhostInLimbo 37→36; RemovePlayerScores 42→42; NumCoinsFound 187→187; CompressGhost 488→331; CalcUnlockedStuff 878→878. Improving mismatch count is not acceptance. All six source bodies were restored.

Replay: apply a single variant to its baseline, use the build procedure in BUILDING.md, inspect the strict report, then revert. Header replay uses `research-header.patch`, full build and `tools/audit_report.py`. Do not apply rejected variants together or add relaxed matching rules.

Publication note: this archive retains selected text summaries and source-only research patches, not the private full proof directory. Accepted patches are already on master and omitted here. Raw worker ledger paths are not replay dependencies; the final normalized ledger will be supplied separately.
