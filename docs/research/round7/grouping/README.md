# Round 7 compilation-group triage

Read-only static probe against accepted master 688e4269 and the pinned target. No compilation, source promotion or quota credit.

## Finding

Scanning mapped game-function extents for SH-4 BSR opcodes whose destinations are mapped function starts yields 1,417 sites after excluding every declared literal-pool interval. All destinations remain within the same mapped unit. The seven apparent cross-unit sites before pool exclusion are all literal data, not credible call evidence.

This is an opcode/map inventory, not a complete control-flow analysis, proof of every historical source boundary, or universal statement about compiler/linker behavior. It does not include indirect calls, non-function-start destinations or unknown functions. Nonetheless, it supplies no evidence for merging neighboring mapped TUs merely to change JSR into BSR.

## Avoid duplicate probes

The round6 ledger already explicitly covers:
- audiomgr validator reconstruction and AudRemoveListener / AudUpdateListener caller changes;
- highscorescr RankScores reconstruction and _InitScreen caller change;
- trackscr buildRaceTrackMaps reconstruction and buildTrackMaps caller change.

Those are not new round7 evidence. Reapplying the archived patches or recounting their dependent callers cannot supply fresh targets.

## Recommended bounded next check

Use the restored full baseline report to identify nonexact callers with target-local BSR and emitted indirect calls. Compare them with all four archived round6 patches before assigning work. A genuinely absent callee with new native-backed reconstruction can justify a new caller-specific trial; do not count incidental shifts, unmodified recompiles or already-tested reconstructions. Candidate completeness is presently unproven, so this investigation does not justify an extra 223 historical revisits.

The prior synthetic test already established same-TU BSR versus separate-object/DEFINE JSR for one bounded case. Repeating that test unchanged adds no new evidence. An actual mismatch between full native and reconstructed intra-unit call graphs would be the useful next result, not a synthetic quota.

## Reproduction

Run `python inspect_groups.py --target YOUR_LEGALLY_SUPPLIED_TARGET --map map/units.json --output grouping-summary.json` from the repository root (use the script path in this directory). The script outputs counts only, not target bytes or a raw call dump. No quota or match credit follows.
