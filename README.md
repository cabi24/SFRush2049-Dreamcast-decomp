# San Francisco Rush 2049 (Dreamcast) decompilation

## Round 7 checkpoint 2 — 2026-10-28, ongoing

Round 7 now adds **240 game code/literal bytes, 276 image bytes and five exact
functions**, preserving all prior matches. Current total: **90,218 / 569,664 game
bytes (15.8371%)**, **159,585 / 1,123,492 image bytes (14.2044%)**, and
**584 / 1,555 exact game functions**. Full 114-unit builds, strict emitted-byte
audits, independent negative controls and 22 host tests passed.
The requested 500 attempts are not complete; viable fresh targets are scarce and
historical revisits require new evidence. See [checkpoint 2](docs/round7-checkpoint2.md).

## Round 7 checkpoint 1 — 2026-10-28, ongoing

The new matching round has independently verified **106 additional emitted bytes and
three exact functions**, preserving every prior credited address and all 579 prior
exact functions. Current total: **90,084 / 569,664 game bytes (15.8135%)**,
**159,415 / 1,123,492 image bytes (14.1892%)**, **582 / 1,555 exact game functions**.
Full 114-unit builds, strict audits, negative controls and 22 host tests passed.
The requested 500-target campaign is ongoing; the remaining fresh candidate pool is
limited and historical revisits require new evidence. No 500-attempt completion is claimed.
See [checkpoint evidence and limits](docs/round7-checkpoint1.md).

## Round 6 results — 2026-10-27

**473 qualifying attempts out of 500 requested; 27 blocked pending new evidence.**
The two accepted checkpoints add **1,414 emitted bytes and ten exact functions**,
with all earlier matches preserved. Current total:
**89,978 / 569,664 game code/literal bytes (15.7949%)**,
**159,309 / 1,123,492 image bytes (14.1798%)**, **579 / 1,555 game functions**.
Full 114-unit builds, strict audits, negative controls and 22 tests pass.
See [round 6 scope, results and shortfall](docs/round6-results.md) and
[draft research PR 6](https://github.com/cabi24/SFRush2049-Dreamcast-decomp/pull/6).
Research probes, moved bodies and unattempted/capped targets are not matching credit.

## Latest verified matches — round 5, 2026-10-27

**88,564 / 569,664 game code/literal bytes (15.5467%)**,
**157,895 / 1,123,492 image bytes (14.0539%)**, **569 / 1,555 game functions**.
This 100-target, four-worker round adds **3,980 emitted bytes and 35 exact functions**,
preserving every prior credited address and exact function. Full 114-unit build,
strict audit, independent negative controls and 22 tests passed.
See [round 5 results](docs/round5-matching.md) and the dedicated
[Linux verification receipt](evidence/linux-round5-integration-20261027.json).
Verification used the separately reviewed reconstructed pinned Linux workflow;
the native wrappers and historical receipts remain unchanged in this source-only
publication. Earlier snapshots below are historical.

## Latest verified matches — round 4, 2026-10-27

**84,584/569,664 game code/literal bytes (14.8481%)**,
**153,915/1,123,492 image bytes (13.6997%)**, **534/1,555 game functions**.
The100-target round adds258 emitted bytes and1 exact function with all prior
credited addresses preserved. Full114-unit build, strict audit, negative control
and six credit tests passed. See docs/round4-matching.md and
evidence/linux-round4-integration-20261027.json. Verification used the separately
reviewed pinned Linux workflow; native wrappers and historical receipts remain
unchanged in this source-only commit. Earlier snapshots below are historical.


## Latest verified matches — round 3, 2026-10-27

**84,326/569,664 game code/literal bytes (14.8028%)**,
**153,657/1,123,492 image bytes (13.6767%)**, **533/1,555 game functions**.
The50-target round adds2,930 emitted bytes and9 exact functions with all previous
credited addresses preserved. Full114-unit build, strict audit, negative controls
and six credit tests passed. See docs/round3-matching.md and the dedicated
evidence/linux-round3-integration-20261027.json. Verification used the separately
reviewed pinned Linux workflow; native wrappers and historical native receipts
remain unchanged in this source-only commit. Older snapshots below are historical.


## Latest verified matches — round 2, 2026-10-27

**81,396 / 569,664 game code/literal bytes (14.2884%)**,
**150,727 / 1,123,492 image bytes (13.4159%)**, **524 / 1,555 game functions**.
Round 2 adds 2,382 emitted bytes and five exact functions, with all prior accepted
spans retained. Independent full 114-unit compile/link, strict audit, negative
controls and six credit tests passed. See docs/round2-matching.md and
evidence/linux-round2-integration-20261027.json. As with round 1, this verification
used the separately reviewed pinned Linux workflow; native baseline receipts and
historical counts below are retained as history, not current-source verification.


## Verified InitWaves match — 2026-10-27

InitWaves now adds **460 emitted code bytes and one exact function**. Verified totals:
**79,014 / 569,664 game code/literal bytes (13.8703%)**,
**148,345 / 1,123,492 image bytes (13.2039%)**, **519 / 1,555 game functions**.
All earlier accepted spans are preserved. Independent replay, a deliberately incorrect
control, and a full 114-unit integration build passed strict target comparison.

This verification used the explicit pinned Linux/wibo toolchain under separate review,
not the native-only wrappers at this commit. The new evidence is
`evidence/linux-round1-integration-20261027.json`; its tool hashes describe that
verification environment. `evidence/strict-integration-receipt.json` remains the
historical native baseline and predates this source change. No claim of full raw
Windows/Linux artifact equivalence or whole-game execution is made.
See `docs/initwaves-match-20261027.md` for scope and reproduction requirements.


A byte-matching decompilation of **San Francisco Rush 2049** for the Sega Dreamcast
(US v1.001, Midway, 2000). The goal is C source that the original Hitachi SHC compiler turns back
into the exact bytes of the retail executable, function by function, at the real addresses.

The GameCube port decompiled from *Midway Arcade Treasures 3* is the semantic reference for names,
types and algorithms. Its bodies are reshaped here until they compile to the Dreamcast bytes.

## Status

Strict credit only: a byte counts when it is equal to the target at its real address after a real
fixed-address link. There are no masks, no fuzzy or displaced matches, and no credit for padding,
linker fill or copied original bytes.

| Measure | Exact | Total | Share |
|---|---:|---:|---:|
| Game code and literals | 78,554 | 569,664 | 13.79% |
| Whole executable | 147,885 | 1,123,492 | 13.16% |
| Game functions | 518 | 1,555 | 33.3% |

The per-unit table is in [HANDOFF.md](HANDOFF.md). The audit receipt
[evidence/strict-integration-receipt.json](evidence/strict-integration-receipt.json) records the
source, header, compiler-input and output hashes and the exact address spans behind these numbers.

## What is not in this repository

- **No game files.** The retail executable, disc images and all other game media are git-ignored and
  never committed. You need your own copy of the US v1.001 disc.
- **No compilers or analyzers.** The Hitachi SH compiler packages come from the decomp.me compiler
  archive; Ghidra, the JDK and objdiff come from their own releases. Sources and SHA-256 hashes for
  every download are in [tools/downloads/MANIFEST.json](tools/downloads/MANIFEST.json), and
  [tools/README.md](tools/README.md) explains the setup.

Build outputs under `build/` are regenerated locally and are not tracked either.

## Target

| | |
|---|---|
| File | `1ST_READ.BIN`, raw little-endian SH-4 loaded at `0x8C010000` |
| Size | 1,123,492 bytes |
| SHA-256 | `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688` |

[retail-v1.001-evidence/verify_retail.py](retail-v1.001-evidence/verify_retail.py) verifies a disc
image and extracts the executable. Place it at `pilot/orig/1ST_READ.BIN`.

## Building

Windows is required, because the compiler runs natively. You also need Python 3.12 with `capstone`
installed, and the toolchain provisioned into `tools/` as described in `tools/README.md`.

```
python tools\dc_split.py                 # target side: per-unit objects and symbol table
python tools\dc_build.py                 # compile, link and compare every unit with source
python tools\dc_build.py --unit ll -v    # one unit, with a listing of the first differing function
python tools\fncmp.py pakerr ShowPakError   # register-accurate side-by-side of one function
python -B tools\audit_report.py          # re-verify saved outputs and rewrite the receipt
```

Open `objdiff.json` in `tools\objdiff\objdiff.exe` for an interactive diff. A full strict build plus
audit takes under a minute. [BUILDING.md](BUILDING.md) covers inputs, outputs and what "exact" means.

### Frozen compiler configuration

`shc-v5.1r11` with

```
-comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const -speed -noinline
```

and the H SERIES LINKAGE EDITOR 6.0E. The flags are frozen: functions are matched by changing the
shape of the C source, never the compiler options.

## Layout

| Path | Contents |
|---|---|
| `src/` | One C file per translation unit, plus `externs.txt` for addresses no unit defines yet |
| `include/` | Shared and per-unit headers; every struct field cites the instruction that proves its offset |
| `map/` | The unit map: 156 contiguous units and 4,726 functions covering the whole executable |
| `tools/` | Compile, link, compare and audit scripts, and the toolchain manifest |
| `calib/` | Compiler calibration, the SH-4 decoder and [calib/lessons.md](calib/lessons.md), the record of SHC code-generation lessons |
| `evidence/`, `matching-evidence/` | Audit receipts and per-round matching evidence |
| `retail-v1.001-evidence/`, `betafindings-evidence/`, `mat3-to-dreamcast-evidence/` | Target verification and analysis of the retail and prototype builds |
| `pilot/` | The original toolchain pilot and calibration lanes |

## Where to start

1. [HANDOFF.md](HANDOFF.md) for current state, resume commands and the per-unit workflow.
2. [BUILDING.md](BUILDING.md) for the build loop.
3. [map/README.md](map/README.md) for how the unit map was built and where it is weak.
4. [include/README.md](include/README.md) for the rules on proving struct layouts.
5. [calib/lessons.md](calib/lessons.md) before reshaping a function.

Much of the matching is done by AI agents. [CLAUDE.md](CLAUDE.md) holds their working rules, and
`CODEX.md` and `QWEN.md` are 2026-10-18 handoffs written for other agents. The earlier planning and
analysis record is [mat3-to-dreamcast-plan.md](mat3-to-dreamcast-plan.md), [overview.md](overview.md)
on the retail executable and [betafindings.md](betafindings.md) on the prototype builds.
