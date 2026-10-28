# Round 7 platform lane

## Result

8 qualifying fresh target attempts. Eight timer records are excluded as auxiliary recycled-shape probes, not qualifying historical or placement attempts. This is not 54 completed targets or eight independent compiler invocations. Of 54 initially selected fresh targets, 46 remain held because bodies already match after relocation or require exhausted predecessor/callee/pool repairs. The lane added 100 exact emitted bytes and three exact functions, independently replayed by the parent.

Accepted changes:
- dc_status_8C06CA98: retain the native signed-byte conversion of the boolean in a full-width temporary. This removes the extra callee-saved temporary and restores the native shared pool position, yielding two exact helpers plus the 12-byte pool: +40 bytes, +2 functions. Final CAB4 helper still has two register-selection bytes different.
- dc_audio64_init_8C06ADD4: pass the existing varargs alignment selection directly as the formatter argument. Native r6 selection replaces the redundant local/move. +60 bytes, +1 function; all 16 functions in that unit are now exact. This does not claim a portable replacement for standard va_start; it preserves the existing pinned target-ABI expression and only changes its expression context.

## Controls and retained research

- Status negative control changes the second helper sentinel from -3 to -2: one differing byte, 12 exact bytes/one function lost. Restored afterward.
- Audio64 negative control changes alignment mask 3 to 7: one differing byte, 60 exact bytes/one function lost. Restored afterward.
- AudEmitEffect command pointer register lifetime reproduces the native hoisted callback/mask register assignment. No strict fixed-address gain; preserved in research.patch only. Replaying archived PR6 validators in a separate context does not unlock a gain and is not counted again.
- LCD copy/index and peripheral call-register probes did not improve accepted source. Failed variants were restored.
- Timer guard inversion changes the predecessor but remains two bytes short; a scoped elapsed temporary is neutral. The seven helper hypotheses lack preserved pretrial provenance, and the missing reload deficiency was already known historically. The historical revisit and seven helper records therefore earn no qualifying count. No fabricated reload, added volatile qualifier, padding or map/flag change was used.

## Verification and limits

Full 114-unit build, strict audit, old-function/credited-span preservation and 22 host tests were run against the accepted final sources. The parent independently preserved the accepted verification receipt on master. No game execution, native-Windows/Linux object parity, full executable relink or CI pass is claimed.

The call-graph investigation found no cross-unit BSR to mapped function starts after excluding declared pools. Broad TU merging is not supported by that scan. Existing near-identical relocated bodies do not count as new attempts or exact functions merely because their source was read.

Publishable files: accepted.patch (parent promotes), research.patch, ledger.json, this README, verification.json. Private raw comparison listings, tool archives, target binary and full build directories must not be published.

## Baseline provenance correction

The initial ledger generator read the canonical mutable report after accepted changes had been promoted. Its four accepted-unit `before` records were therefore incorrect. The corrected generator uses frozen baseline and retained-final reports, not the canonical live report. The baseline snapshot came from lane2's preserved round7 baseline and its summary and all 114 source hashes agree with the frozen round7 baseline receipt. Corrected differences are CA98: 2, CAA8: 1, CAB4: 27 (linked size 44), and B298: 26. All other before records are unchanged. Every selected probe report's source hash matches its preserved variant, and every retained-final source hash matches the final audit receipt. See `ledger-correction-audit.json` (publishable) for per-target provenance and report hashes. Frozen full reports and the superseded ledger are private verification inputs, not recommended publication artifacts. Final normalized count is eight qualifying fresh targets and three accepted exact-function gains. The eight timer records are transparently retained as uncounted auxiliary repeats.

## AudEmitEffect evidence replay

A bounded replay of the frozen original and saved register-pointer variant confirms 12 emitted instruction bytes differ across 12 instruction words in the 394-byte linked body [8C0153F2,8C01557C), excluding trailing shared literals and object metadata. Fixed-address target difference count remains 390, so this is a genuine attempted source/code change, not an exact gain. Source and rebuilt body were restored, and the strict audit passed. Publishable hashes, byte count and scope are in `audio-emitted-body-replay.json`; raw replay ABS files remain private. This verification adds no matching attempt.

Publication note: this archive retains selected text summaries and source-only research patches, not the private full proof directory. Accepted patches are already on master and omitted here. Raw worker ledger paths are not replay dependencies; the final normalized ledger will be supplied separately.
