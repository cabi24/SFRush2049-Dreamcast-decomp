# Campaign60 lane6 final handoff

RESULT: all10 assigned units assessed with private frozen native baselines. Retained
15 new fixed-address exact functions and1400 definite text/literal bytes across zoid,
Audio64 setup and LCD drawing. Queue initialization additionally produces an exact88-byte
literal pool; held separately for integrator review because HANDOFF explicitly excluded an
earlier88-byte packet. No function credit is claimed for queue initialization.

| Unit | Before exact funcs/text/data | Final exact funcs/text/data | Delta |
|---|---|---|---|
| zoid | 7 /440 /0 | 14 /932 /0 | +7 funcs,+492 text |
| dc_audio64_init_8C06ADD4 | 2 /108 /317 | 9 /824 /317 | +7 funcs,+716 text |
| dc_misc_8C06C08C | 4 /436 /24 | 5 /628 /24 | +1 func,+192 text |
| dc_unk_8C069E34 | 0 /0 /0 | 0 /88 /0 | +88 pool only, review separately |

CHANGES:

- zoid: TrueObjParent compares next against the original short argument before assigning
  next to the traversal variable. Removing the redundant short alias reproduces target86bytes
  and restores four following exact functions. A target-backed local pointer for the first
  sibling-removal arm makes _RemoveObjectTree274bytes (still differs), restoring the two
  following removal wrappers. All earlier exact spans preserved.
- Audio64 setup: filename's null initialization shares the already-zero size local. This
  reproduces target B060..B06C (size store and shared-zero delay slot) and removes four
  extra bytes from setup. Setup and six neighbouring functions/pools become exact.
- LCD C1E6: mixed signature reordered to(dest,float,float,int,int), updating its sole C caller.
  SH4 uses separate integer and FP argument banks, so the register ABI is unchanged; the
  compiler now spills FP args first exactly as target C1F0..C1FC.192-byte function exact.
- Queue init: typed pointer comparison/base+1 and node++ replace equivalent integer-cast
  pointer arithmetic. Native pool now exact88bytes; unresolved branchless SDK tst instructions
  still prevent a full-function match. No empty diagnostic checks were introduced.

VALIDATION:

Private commands from C:\rush2049-dreamcast:

    python -B matching-evidence/2026-10-18/campaign60-lane6/run.py all baseline
    python -B matching-evidence/2026-10-18/campaign60-lane6/run.py UNIT final
    python -B matching-evidence/2026-10-18/campaign60-lane6/final_review.py

The final label was independently rebuilt for all four changed units. final-review.json and
each UNIT/final/review.json record exact function deltas, source/object/ABS hashes, and a
byte-for-byte recheck of every previously published exact text/data span. All preserved.
The six unsuccessful units' current source hashes equal their native baseline source hashes.
Generated binary/object/preprocessed artifacts are confined to build/campaign60-lane6/.
Text sources/results/listings/reviews remain in this evidence directory. Global report,
strict receipt, objdiff, headers, externs, map, tools and unrelated source were untouched.

BLOCKERS / bounded attempts:

- ZIP: split chained match_length/prev_length initialization regressed19to18 exact; restored.
  Target allocator/zero-register assignment differs; first physical opcode at7EA2 usesr11
  for allocator versusr13. Shared tail size differences remain; no speculative padding used.
- ZOID: removal's first32differing bytes include sentinel scheduling, pool displacements and
  final count reread. It remains274bytes and uncredited; only its restored wrappers count.
- Queue and sound: target has branchless status/range tests whose assert/diagnostic contract
  is unknown. Sound's first wrapper matches actual operations but call/pool displacement is
  shifted by the incomplete following body; five stream bodies remain missing. Parked.
- Audio64: simple declaration reordering did not affect output; shared-zero shape won.
  AEB2 still lacks target's branchless tst of SDK return atAEBC and stays20vs22bytes.
- Blit: int parameter variants identical; explicit reversed OR regressed helper9vs10exact.
  Both restored. Remaining ZDist/XLU residuals3/9bytes are inline register allocation.
- GDFS: B80C differs only in literal displacements. B82C target has extra branchless error
  comparisons and a final attempt-count tst with unknown failure behavior; no vacuous tests
  fabricated. Existing source unchanged.
- LCD: C1E6 won; later LCD/peripheral bodies still move because C3D0 and subsequent bodies
  differ. No new SDK semantics inferred beyond the mapped field/call contracts.
- Pak: local buffer lifetime variant gave no new equality and was restored. Init has pool
  shifts and target ClearPak address reloads rather than a saved address;35tail bodies remain
  external, so a whole-unit contract is incomplete.
- Kamui/vibration: request-first declaration variant gave no new equality and was restored.
  First function differs in stack slot allocation and scheduling before SDK calls; no SDK
  behavior invented to force the frame.0functions/0bytes still exact.

No compiler/flag variation, masks, assembly, original-byte fallbacks, fill/BSS credit,
installs/downloads, full builds, game execution, MAT3 edits, commits or pushes.
