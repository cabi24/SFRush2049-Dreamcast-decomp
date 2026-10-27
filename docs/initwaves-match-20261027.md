# InitWaves strict match — 2026-10-27

## Source and semantics

The only game-source change is one empty busy-wait in src/waves.c: while becomes
do/while around the same existing volatile signed-char read. An empty body means
both forms evaluate the same volatile expression once before the next statement
and repeatedly while nonzero. There is no added volatility, fake padding, copied
original bytes, helper, flag change or altered field layout.

## Verification

- Target: US v1.001 executable, SHA-256
  336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688.
- SHC 5.1r11; H-series LNK 6.0E from r13; frozen repository compiler flags.
- wibo 1.2.0 Linux x86_64, SHA-256
  13f86a2d618f0dbe67179d349625345eabf9b46450295cb4c904e49f6aff85af.
- Independent whole-TU baseline/candidate/deliberately incorrect control/restored
  builds gave 9, 0, 2, 0 differing bytes. Target interval is 8C064FDC..8C0651AA.
- That entire 462-byte mapped interval is exact; two linker-fill bytes excluded.
- Waves text credit 1,850 -> 2,310; exact functions 20 -> 21; data remains 630.
- Every earlier accepted byte span and exact function was preserved. Independent
  candidate/restored/worker emitted section/address/data runs agreed.
- Parent full 114-unit build and actual emitted-byte audit passed, as did all six
  progress-credit tests. Only waves improved. No overlap or comparison masks.

Final game code/literals:79,014/569,664 (13.8703%). Image:148,345/1,123,492
(13.2039%). Game functions:519/1,555. Units:29 exact,85 partial.

## Evidence and reproduction boundary

The new source is published independently of the Linux host-tool changes.
evidence/linux-round1-integration-20261027.json records full source/header/input
and verification-tool hashes, output hashes and credited address spans. Its
launcher/tool hashes correspond to the separately reviewed Linux implementation
commit 8dee72f5752ca059a5fb91746cd34e40dcc7d0bd, not native wrappers at this commit.
The original evidence/strict-integration-receipt.json is retained as the historical
native baseline; it does not verify the newly changed waves source.

Use the separate Linux-workflow branch instructions to reproduce that environment,
or run native canonical builds and regenerate a fresh native audit receipt.
Private target/compiler packages are required and are never included here.

This proves fixed-address generated-byte equality, not whole-image relink, gameplay
execution, original C spelling, or raw Windows/Linux object-file identity.
Unmatched round-one candidates are excluded and are research only.
