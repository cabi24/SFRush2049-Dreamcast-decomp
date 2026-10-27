> Historical lane report. Accepted matching source is now separately on master; worker-local publication statements below describe the original lane stage. This archive adds no credit.

# Simulation lane: 25 actual targets

Accepted: **+2,218 game/image bytes and +13 exact functions**. Full 114-unit rebuild and unchanged emitted-byte audit passed. No prior exact function or exact pool/data span was lost.

Six of the 25 final target rows became newly exact; seven additional functions became exact incidentally. Correct-but-displaced original selections were replaced rather than counted as attempts.

| ID | Unit / function | Address | Probe outcome | Credit |
|---:|---|---|---|---:|
| 51 | drivsym / regular | `8C0238EE` | 47 differing bytes after alias removal versus 2 originally; restore. Original misses are local BSR destinations displaced by force/torque predecessors. | 0 |
| 52 | drivsym / calcaa | `8C0250AC` | Relative mismatch increased from 0 to 27 bytes and body grew 4 bytes; restore. Baseline arithmetic was already exact but displaced. | 0 |
| 53 | maxpath / TargetSteerPos | `8C03BAB0` | Body changed; 23 relative differences remain. No fixed-address match; restore. | 0 |
| 54 | save / SaveServer | `8C04B746` | Zero-test form was neutral and not counted; pointer-local probe changed body but worsened 19 to 231 fixed-address differences. Restore. | 0 |
| 55 | save / _ReadGhostHeader | `8C04DCB2` | Fixed-address body changes from 1 difference to 0. Accepted; no direct ReadGhostHeader edit needed. | 1 |
| 56 | save / _ReadGhostData | `8C04DDC6` | Fixed-address body changes from 1 difference to 0 after natural SaveGhost layout repair. Accepted. | 1 |
| 57 | save / SavePlayerCoins | `8C04C890` | 227 and 175 fixed-address differences versus 89 baseline; target parameter spill unresolved. Both restored. | 0 |
| 58 | save / DeleteGhost | `8C04E52A` | Fixed-address body changes from 1 difference to 0; same PS_DeleteFile literal value. Accepted. | 1 |
| 59 | save / SaveGhost | `8C04E2F4` | SaveGhost becomes exact at its original address; old accepted spans preserved. Accepted. | 1 |
| 60 | save / _ReadTraining | `8C04DC14` | Target comparison opcode obtained, but scratch operands differ; 4 then 6 differences versus 3 after accepted layout repair. Restore. | 0 |
| 61 | maxpath / AdjustSpeed | `8C03BC20` | Second probe has 0 relative body differences but is displaced. Research only, zero credit. | 0 |
| 62 | maxpath / AdjustSteer | `8C03BD38` | Clamp region improves but constant reload/register schedule remains; 56 then 49 relative differences. Restore. | 0 |
| 63 | maxpath / SetDroneResPathIndex | `8C03C460` | Address-generation shape improves; pooled predecessor value and downstream pool displacements remain. Research only. | 0 |
| 64 | resurrect / FindSurfaceUvsInPlace | `8C049750` | 0 relative differences, but entry remains four bytes early. Research only. | 0 |
| 65 | maxpath / SetNewPathIndex | `8C03C5CE` | 0 relative body differences, but displaced. Research only. | 0 |
| 66 | resurrect / FindDroneResPt | `8C049168` | Body changes and shrinks four bytes, but allocation/frame mismatches remain. Restore. | 0 |
| 67 | resurrect / FinishResurrecting | `8C04A06C` | Compiler does not produce target scalar shape; body grows and 222 relative differences remain. Restore. | 0 |
| 68 | maxpath / TraceToMain | `8C03C8A6` | Isolated probe preserves linked address and size and changes its emitted pool byte; source recursion instructions unchanged. Still displaced with pool alignment differences; no credit. | 0 |
| 69 | maxpath / TraceToIndex | `8C03C9D4` | Second probe matches all instructions; 2 literal displacements remain at a displaced address. Research only. | 0 |
| 70 | resurrect / FindBestQuat | `8C049A08` | Second probe yields 0 relative body differences, still four bytes early. Research only. | 0 |
| 71 | resurrect / CheckCrash | `8C049FA4` | Case ordering improves; flag scratch register and restart-store schedule still differ. Research only. | 0 |
| 72 | resurrect / BlinkResurrectingCars | `8C04A3DC` | Body changes but allocation/loop differences remain (209 relative differences). Restore. | 0 |
| 73 | maxpath / TargetSpeed | `8C03B9B4` | Body changes, but literal-island placement and clamp scheduling remain wrong. Restore. | 0 |
| 74 | game / _EndGame | `8C028D5C` | Shared-pool repair produces 0 fixed-address differences. Register-loop negative probe gives 15 differences and is restored. Accepted dependency repair. | 1 |
| 75 | game / _CheckButtons | `8C028E5A` | Exact at the original address; also restores the preceding shared pool and callers. Accepted. | 1 |

## Accepted source

- `_CheckButtons`: ordinary empty signed-short for loop, replacing inherited goto shape. This restores `_EndGame`, `CarSetup`, and shared literals at original addresses.
- `SaveGhost`: reorder existing local declarations to target stack offsets, without changing storage sizes. This restores the function, two ghost-reader BSRs, deletion literal load, and six later wrappers/helpers.

## Research only

`patches/simulation.patch` is separate and is not retained in the accepted tree. It contains useful but noncreditable instruction-shape corrections in maxpath/resurrect. Four bodies become byte-identical only at displaced addresses. TraceToIndex retains two literal displacements; SetDroneResPathIndex retains a predecessor-owned pool mismatch. Do not promote these based on relative equality.

The accepted source has only the game/save changes. All other probes were restored. No flags, maps, headers, auditors, linkers, raw bytes, padding, or linkage were changed. No publication was performed.

## Reproduction scope

Use the authorized pinned Linux/wibo build wrapper with the frozen configuration, rebuild all 114 units, then run the unchanged emitted-byte auditor. The final simulation-only totals are 86,802 / 569,664 game bytes, 156,133 / 1,123,492 image bytes, and 547 / 1,555 exact game functions.

Detailed hypotheses, replacements, fixed-address differences, and diagnostic relative differences are in `ledger.json`. Private compiler/disassembly evidence remains outside these deliverables.
