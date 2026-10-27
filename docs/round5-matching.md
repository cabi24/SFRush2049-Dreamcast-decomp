# Round 5 matching results — 2026-10-27

## Verified result

Four workers attempted 100 distinct new game-function targets, 25 per worker.
Targets that already had correct instruction bodies but were merely displaced were
replaced with documented fresh targets rather than counted as artificial probes.
Auxiliary predecessor repairs and incidental gains are distinct from the target count.

The integrated accepted source adds **3,980 emitted game code/literal bytes and
35 exact functions** over public master `cb6dbdcfc49ea4f94e4f104ef5f679527a274fd1`:

| Unit | New emitted bytes | New exact functions | Source repair |
|---|---:|---:|---|
| game | 462 | 3 | Natural empty for-loop replaces inherited goto loop in _CheckButtons; nearby code/pool placement also recovers _EndGame and CarSetup. |
| save | 1,756 | 10 | Existing SaveGhost local arrays reordered to match observed stack layout; no array size or payload changes. |
| HAL_text | 1,362 | 18 | Reset order, call/assignment shapes, semaphore guard and target-retained validity comparisons. |
| stree | 400 | 4 | DecodeVlist byte-mask/return shape and VectorToSurfEdge denominator-first evaluation/explicit returns. |

Current totals:
- Game code/literals: **88,564 / 569,664 bytes (15.5467%)**.
- Whole image: **157,895 / 1,123,492 bytes (14.0539%)**.
- Exact game functions: **569 / 1,555** (4,717 mapped functions including runtime/SDK).
- Every previously credited address and all 534 previously exact functions are preserved.

The target retains font-invalid-sentinel and texture-capacity comparisons with no
error branch. These are coherent validity predicates, independently reviewed against
the target and analogous existing empty guards in waves/powerups. The original
assertion or diagnostic mechanism is unknown: no invented assertion facility,
volatile qualifier, error side effect or padding was introduced. _SetupFontData
remains a nonexact dependency, not an additional exact-function claim.

## Verification and limits

After cloud workspace recovery, the independently selected pinned Linux workflow
was reconstructed and first reproduced the entire preceding 114-unit baseline:
all source hashes, all credited spans, and all exact-function counts agreed.
The final four-source integration was then rebuilt across all 114 units and checked
by the strict unmasked emitted-byte auditor. All 22 host tests passed, including
six byte-credit regression tests. Source/map/header/compiler flags were preserved
except the four listed source files; no scoring or map changes were used to obtain gains.

Independent negative controls were rejected and the accepted source restored:
- _CheckButtons loop bound changed: 2 differing bytes.
- SaveGhost string-array declaration swap: 126 differing bytes.
- HAL_GetFontSpacing validity sentinel changed: 13 differing bytes.
- DecodeVlist mask changed: 1 differing shared-pool byte, mapped inside
  VectorToSurfEdge; the full-TU comparison lost 158 credited bytes even though
  DecodeVlist's own instruction bytes stayed equal. This explicitly demonstrates
  why a function-only shape comparison is insufficient.

The receipt is `evidence/linux-round5-integration-20261027.json`.
Its tool hashes identify the separately reviewed reconstructed Linux verification
lane, not the native wrappers retained by this source-only public commit.
Compiler SHC v5.1r11, H-series LNK 6.0E and frozen flags remain unchanged.
The explicitly selected wibo 1.2.0 loader is pinned to SHA-256
`13f86a2d618f0dbe67179d349625345eabf9b46450295cb4c904e49f6aff85af`.
There is no automatic fallback after a denial. Historical native receipts remain
historical; no raw Windows/Linux object parity, whole-executable relink, gameplay
execution or CI pass is claimed. No CI watcher was started.

Platform and screen probes produced useful instruction-shape research but no new
fixed-address credit. Their source-only candidates and normalized outcomes belong
in the separate draft research PR, not accepted source. No ROM bytes, binaries,
raw assembly dumps, private paths or credentials are included.
