# Round7 lane3 screen results

Four distinct fresh target attempts out of38 selected candidates. Selection and inspection are not attempts. Thirty-four candidates remain blocked, chiefly already body-equivalent helpers with exhausted predecessor/pool-placement dependencies. No automatic historical retries, padding, dummy calls or flag changes were used.

## Accepted candidate

`accepted-source.patch` changes only pakerr.c: native-supported chained choice clearing and a real shared unsigned-short text-table index local in ShowPakError. ShowPakError itself remains partial (205 differing bytes versus432 baseline). It aligns _AnimDim, recovering74 game-code/literal bytes and36 data bytes,110 image bytes total, one exact function. All579 prior exact functions and every prior credited span survive.

Full114-unit build, strict audit,22 host tests and diff-check pass. Return0 negative control changes4 body bytes in _AnimDim and removes its whole28-byte exact-function credit, then source restored and re-audited. No full-image relink, gameplay or native-Windows object parity claim.

## Research only

The three other patches are independent trials, not accepted code. The two trackscr patches apply independently, not sequentially. Cleanup explicit-zero lifetime changes code but gives no fixed-address gain. loadPathData explicit track-index lifetime changes two literal-reference displacements only, no gain. TrackValid explicit range returns changes code/layout but gives no fixed-address gain. One additional LoadWave zero-variable trial was compiler-neutral and is excluded from target credit; initial type-error trials also excluded.

No raw binary, raw assembly or proprietary asset is included. Private build reports are retained separately for replay.

## Emitted-body counting evidence

`emitted-body-evidence.json` hashes actual emitted byte windows, not whole object files or timestamps. loadPathData changes exactly two emitted displacement bytes at body offsets0x0 and0xA in the successful path-index-register trial. Its fixed-address diffcount stays79, linked start stays8C08F9C0 and linked size stays80. It counts as one unsuccessful source-shape attempt, with no gain. The separate path-zero-register successful compile has zero emitted changes and is excluded, as are failed-only trials. Cleanup has independently hashed changed first128 body bytes; its reported linked-size span includes neighboring static functions and is not a reliable function length. No raw assembly or binary bytes are included.
