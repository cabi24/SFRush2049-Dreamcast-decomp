# Round 7: 30 qualifying attempts, five exact gains

The requested 500-target round is partially fulfilled. Four workers completed 30
qualifying attempts (8/8/4/10); 470 requested slots remain unfilled. The initial 152
fresh candidate selections were not completed attempts. Many had already correct
relocated bodies and lacked a new justified predecessor/ABI repair. We did not add
padding, fake stack locals, arbitrary qualifiers or repeated capped shapes to fill quota.

## Accepted and published

Five newly exact functions add 240 game code/literal bytes and 276 image bytes:
Audio64 formatter func_8C06B298; peripheral-status helpers func_8C06CA98 and
func_8C06CAA8; gfx func_8C02E124; pakerr _AnimDim. Partial predecessor repairs are
not themselves counted exact. Totals are 90,218 / 569,664 game bytes (15.8371%),
159,585 / 1,123,492 image bytes (14.2044%), and 584 / 1,555 exact game functions.

The parent independently replayed all accepted snapshots and negative controls.
Full 114-unit builds, strict unmasked emitted-byte audits, preservation of all 579
pre-round exact functions/every credited span, and 22 host tests passed. See
[checkpoint 1](round7-checkpoint1.md) and [checkpoint 2](round7-checkpoint2.md).
No full executable relink, gameplay validation, raw native/Linux object parity or
CI pass is claimed. No CI watcher was started.

## Research and next evidence needed

Two native renderer functions, the blit traversal at 8C06F4D0 and polygon traversal
at 8C0705EA, were reconstructed with target-corrected MAT3 ngcZoid references.
They compile/link together but remain at wrong addresses and contribute zero exact
credit. Their private ABI assumptions and native platform drift are documented.
Other research preserves unsuccessful source trials and two canonical-qualifier
consistency probes, not automatic source promotions.

A mapped-function BSR scan excluding declared pools found no cross-unit calls among
1,417 mapped-target sites. The seven naive cross-unit hits were literal data. This
bounded evidence does not support blindly merging translation units. Progress now
needs genuine source completeness, native ABI and predecessor/pool layout evidence.

## Conservative accounting

- Eight timer records are excluded: the historical UpdateFrameTimer inverted-guard trial had no materially new evidence beyond an already known reload mismatch, and the seven dependent helper hypotheses lacked a preserved pretrial target-specific artifact. Their output is auxiliary research, not quota completion.
- Historical enginetorque pointer-lifetime retries are excluded.
- Neutral source shapes, failed compiles, unchanged emitted bodies, inspection-only rows, relocated equality, and incidental movement are not separately counted.
- AudEmitEffect's identical fixed-address diff count did not mean neutral code. Bounded replay of frozen sources proves 12 emitted instruction-byte changes in its 394-byte linked body. No exact gain follows.
- loadPathData has two changed emitted literal-displacement bytes; its other successful shape is neutral. updatePauseStateCleanup has eight changed bytes in a 128-byte prefix window. Independent ABS replay verified the worker hashes and deltas.
- ClearBattlePoints has a changed loop at its actual compiled address. The 34-byte comparison has 20 differences and includes the shortened body's successor boundary; it is not a claim of 20 changed own-body bytes. _AddBattlePoint's register-index trial has 54 changed bytes in its 296-byte body. Its pointer-local trial is neutral and excluded. Independent ABS hash/delta checks passed.
- The two ABI revisits concern private declarations of gFrameSeconds missing the volatile qualifier present in the canonical timer.c definition. Their native-access hypotheses and changed successful builds are recorded; they are not accepted matching gains.


The final normalized research ledger records 24 fresh body probes, two explicit
shared-pool placement probes, two reconstructions and two historical ABI revisits.
All 30 addresses are unique and were nonexact in the immutable baseline. Fresh means
absent from the reconstructed public history/exclusion, not proof of no older private
study. Candidate selection, inspection, neutral output and controls are not quota credit.
