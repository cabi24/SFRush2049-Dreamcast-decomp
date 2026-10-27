> Historical lane report. Accepted matching source is separately on master. Packet filenames below describe worker-local review materials; this compact archive contains only the selected research patch.

# Round 5 presentation lane

## Result

- **25 actual fresh attempts:** 15 HAL text, four visuals, six parent-approved spatial-tree targets. All addresses are unique and absent from the prior-target exclusion list.
- **12 of the 25 are now strict original-address exact.** Ten additional auxiliary/collateral functions also became exact: **22 new exact functions total**.
- Full 114-unit rebuild and unchanged strict audit: **86,346 game bytes; 155,677 image bytes; 556 exact game functions**. Lane gain: **1,762 bytes and 22 functions** over baseline.
- Every previously credited address and all 534 previously exact functions remain credited. Zero comparison masks or overlapping credited spans.
- No visuals changes retained. Rejected experimental HAL and spatial-tree shapes were restored before final verification.

## Accepted source

`presentation-accepted.patch` changes only `src/HAL_text.c` and `src/stree.c`. The two unit patches are also supplied separately.

HAL repairs the inlined initialization guard, font-wave lookup/load/lock sequence, chained state reset, and font-data setup. Repeated target-observed residual font validity comparisons and one texture-capacity comparison are preserved without inventing an error branch, volatile read, padding, or a historical assertion implementation. The read-only MAT3 source supplied semantic context but did not establish these comparisons' source spelling. Independent review accepted the comparisons as target-retained validity checks.

The HAL-only structural variant, without residual comparisons, gains **80 bytes and two exact functions** (288 exact text bytes, three exact functions in the unit). Full HAL gains **1,362 bytes and 18 exact functions** (1,570 exact text bytes, 19 exact functions). The structural and incremental comparison patches are separate for review. The spatial-tree changes add **400 bytes and four exact functions** independently of that distinction.

Spatial-tree repair preserves the packed-run mask and return-value lifetime in `DecodeVlist`. The auxiliary `VectorToSurfEdge` repair evaluates the denominator first and spells the final comparison as explicit returns. This also resolves shared literal placement for `DecodeSlist` and `surfpos`.

## Attempt accounting

The ledger records nine original selections replaced because their own instruction shapes were already correct apart from address/call/pool layout. Those originals are **not** counted as fresh source attempts, even where collateral layout repair made them exact. Three replacements stayed in the owned HAL unit; six came from explicitly authorized spatial-tree ownership expansion. Prior targets such as `HAL_StartText` and `_SetupFontData`, and auxiliary `VectorToSurfEdge`, count only as repairs, never as fresh attempts.

Neutral source spellings are recorded separately and do not qualify as actual probes. Every counted target has a compiled, target-grounded source change affecting its own body or argument literal. At most three shapes per function were tested. Probe snapshots include strict difference counts, linked address/size, and qualitative aligned-body observations; unmeasured relocated difference counts are explicitly null. Moved-only or fuzzy matches receive no credit.

## Research and verification

`patches/presentation.patch` is a source-only delta against the accepted tree, collecting compiled nonexact experiments for the 13 remaining targets. Individual shapes were compiled in the ledger's named snapshots; **the combined research collection is not an accepted or credited build**. Per-unit research patches are also supplied. It should not be applied to the accepted matching source merely to reproduce the lane total.

The final accepted full build and strict audit passed. A local negative control changing the decoded return increment produced ten differing bytes in `DecodeVlist` and 73 in `VectorToSurfEdge`; restoring the source returned both to exact. Parent independently replayed the accepted candidates and additional negative controls. All raw target evidence, private binaries and local proof paths remain outside this sanitized package.

The research patch passes `git apply --check` against the accepted tree; accepted source passes `git diff --check`. No compiler, map, flags, common headers or scoring tools changed. No publication or commit was performed by this worker.
