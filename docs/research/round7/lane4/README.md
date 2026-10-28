# Round 7, lane 4: accepted match and renderer research

Baseline: `6fb2d7e`, 114 compiled units, 579 exact functions, 89,978 game bytes,
159,309 whole-image bytes. The selected 20 fresh targets are **not** 20 attempts.
`ledger.json` records 10 genuine distinct attempts: eight changed-body targets,
two newly reconstructed renderer functions. Ten other selections remain honest
holds. No historical target was added merely to meet a quota.

## Accepted independently replayable change

Already accepted on master: `gfx::func_8C02E124` replaces the <=40,000 guarded
block with its equivalent >40,000 early return. Native unsigned branch direction
and store-register lifetime match; no threshold or side effect is changed.
Result: **580 exact functions, 90,044 game bytes, 159,375 image bytes** (+1/+66/+66).
Full 114-unit build and strict `tools/audit_report.py` pass. All 579 previous exact
function identities and credited spans remain; 22 tool tests pass.
`final-accepted-receipt.json` is the final restored accepted-only workspace.

Negative control: changing the threshold to 40,001 changes the constant pool,
losing 36 credited bytes, but this function's body remains exact. This is a pool
control, **not** proof that the body's exactness depends on that literal byte.
The accepted body change itself fixes two register-encoding bytes. The parent
also independently replayed accepted and negative snapshots.

For builds, follow the repository BUILDING.md; the independently verified Linux backend is not included in this source-only archive.
No flags, map, shared headers, comparison masks or scorer changes were made.

## Research-only renderer reconstruction

`renderer-research.patch` is independent of the accepted gfx patch and adds two
real bodies to the existing renderer unit; it does not move or merge units.
Combined with the accepted patch it passes all 114-unit compilation/linking and
strict audit, with the **same** totals as accepted-only. No reconstructed byte is
claimed as a match. `research-preservation.json` verifies zero lost old exact
functions or credited spans. No gameplay/runtime validation was performed.

| Native function | Role | Linked research location | Linked size | Target size |
|---|---|---:|---:|---:|
| 8C06F4D0 | Blit draw traversal | 8C06F300 | 688 | 730 |
| 8C0705EA | Polygon draw traversal | 8C06F5B0 | 1500 | 1462 |

The isolated blit reconstruction was 708 bytes; adding Polys changes literal
pool placement to 688 bytes. Neither body moved to its correct target location.
The report calls Polys "missing" because its compiled body ends before its
native mapped address, even though the symbol and complete compiled body exist.
Moved/body similarity is not exact-match credit.

### Evidence and ABI boundaries

`references/mat3/decomp/src/NGC/ngcZoid.c` supplies recognizable Blits/Polys
control/data flow, **not** an authoritative Dreamcast ABI. Target inspection
corrected materially different GC behaviors. The public research contains only
source and evidence summaries, never target bytes or raw disassembly.

Blit traversal:
- `gBlitInstanceList` is a backing array with 32-byte stride, not a global pointer.
  Existing independently mapped zoidblit setters establish palette+0, texture+4,
  texindex+8, x/y/z+A/C/E, width/height+10/12, alpha/flags/hide+14/15/16,
  top/left/bottom/right+18/1A/1C/1E. Size assertion checks 32 bytes.
- `gPalDefPtrs` is an array of 8-byte table entries. Its first word is the palette
  backing pointer; palette stride is24 and start/data pointer+20. Remaining table
  word and leading palette bytes are opaque; their semantics are not invented.
- Native texture stride48 and width/height+10/12 are shared with existing types;
  native bytes+14/+15 and signed halfword+16 feed BlitColors parameters and
  palette selection. The private view's names `format`/`size` are provisional:
  argument order (byte15 then byte14) is target-backed, not original spelling.
- Native custom flag80 invokes the texture field as callback with palette arg;
  z/attribute/scissor/rectangle/texture-draw call order and arguments are retained.
  Callback calling convention/type spelling remains inferred from observed call.
- Fixed four-byte fallback color at8C0C9750 is existing writable state, not an
  allocation or inserted byte array. Target comparison at8C06F686 has no observed
  control-flow effect; the retained empty comparison records that oddity and is
  not a padding objective. Exact source spelling and intended check are unknown.
- Native flip flag8 polarity differs from donor. Width/crop and alpha paths use
  native fields; texture and palette lifetime validity remain caller assumptions.

Polygon traversal:
- Existing `tPoly` stride104, vertices24, `tCamera` stride152 and position+36
  define native layout. No shared ABI/header was changed. New private projected
  vertex view is40 bytes: xyz, clip+12, sx/sy/sz/uv and color+36.
- `PolyInstanceList` and `gCamera` are backing arrays; `gTexDefPtrs` is an 8-byte
  table whose first word points to48-byte textures, with strip pointer+44.
- Native gUpsideDownTrack and gMirrorWaves are byte loads (8C0709CE/9D6), not
  donor ints. Local declarations retain signed-byte width.
- Cache floats at7C002C00/+4/+8/+C/+6C are existing SH4 cache state. Native
  fill-rectangle arguments are integer screen coordinates, color bytes and float
  depth. Matrix transforms call072654/072604; push/pop call070BCC/070BF8;
  poppush uses pop followed by existing depth increment at8C0C974C.
- Native matrix choices reference existing matrices8C19EBC0/EBF0. The apparently
  unreachable second flag8000 branch is retained because it appears natively;
  no donor cleanup or additional branch semantics were invented.
- Native pass slot/list state, QACR0/1, store-queue address masks, header transfer,
  counters, clip threshold, winding and writeback follow the target. Helper
  prototypes are inferred from register/data use; semantic naming and all alias
  behavior are not independently runtime-validated.
- GC delay filtering, WorldToScreen/use_z_value conversion, no-clip gating and
  final diagnostic sprintf are absent from the native function and not copied.

Remaining large renderer targets FE24/C32/71588 are inspection-only holds:
packed geometry and hierarchy traversal/EndFrame SDK ABI and platform differences
need further target-backed reconstruction. No donor-only placeholders were made.

## Other trials and exclusions

`patches/` contains alternative **failed experimental** source changes. They are
not cumulative patches and must not be applied together or promoted as matches.
`replayed-targets.json` gives before/after target comparisons and source hashes.
Fixed-address comparisons can be misleading for moved bodies: MBOX_HideObject
still starts two bytes early, and the HUD body lives far from its native address.

ClearBattlePoints is a compiler-local symbol absent from the object symbol list.
Its real baseline body starts8C095346. Changing the loop to pointer/end inequality
changes20 bytes in a34-byte window (which includes the changed successor boundary
because the loop shortened). Its report's target-address diff alone is unusable.
_AddBattlePoint's register-j variant changes54 bytes in its296-byte body at8C095368.
`hud-register-body-change.json` supplies linked ABS hashes for independent private
replay. The local-pointer variant compiles byte-identically and is **excluded**;
the ClearBattlePoints pointer-for variant is likewise excluded. Unchanged builds,
inspection, and incidental downstream shifts add zero attempts.

Three source shapes were examined for each preexisting target except the clear
loop. Not every source spelling counts as an emitted-body trial. The supported
headline is ten distinct target attempts, not an inflated trial-shape sum.

Private compilation logs, target comparisons and original binaries stay under
the ignored worktree build directory. Only this source/evidence packet is intended
for parent review/publication. The lane worktree has been restored to accepted
`src/gfx.c` only; its untracked `tools/shc` symlink must never be staged.

Archive scope: accepted patches, full receipts and raw reports are omitted. Proof-file references above describe privately retained worker evidence, not files shipped here. Research alternatives are independent, not cumulative.
