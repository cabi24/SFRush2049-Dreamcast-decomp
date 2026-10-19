# Campaign100 lane6 final result

RESULT:16 assignments addressed, including one explicitly documented inventory blocker.
Seven new whole-function fixed-address matches and284 text/literal bytes retained. No data gain.

| Unit | Baseline functions /text /data | Final functions /text /data | Gain |
|---|---|---|---|
| zoid |14 /932 /0|18 /1092 /0|4functions,160text|
| dc_audio64_init_8C06ADD4 |9 /824 /317|10 /924 /317|1function,100text|
| dc_misc_8C06C08C |5 /628 /24|7 /652 /24|2functions,24text|

CHANGES:

- ZOID SetObjectParent retains its cached pointer only before the call to ObjPrevObj; accesses
  after that call re-index the array. This restores the target pool and SetObjectDef. The fresh
  C68 target uses a structured traversal loop and terminal break rather than a goto, keeping
  mask/window/base in the target's callee-saved registers. C68 is fully exact94bytes; ObjectFlags
  and SetObjectFlags also regain their target placement. SetObjectParent still has19differing
  register-allocation bytes and is not credited. Existing removal residual remains27bytes.
- Audio fade AF60 explicitly retains a pointer to dcAudioTime before converting milliseconds.
  This reproduces target address hoisting into r7 across unsigned-to-float conversion and makes
  the complete100-byte function exact. Every prior exact Audio64 span is preserved.
- LCD C3D0 retains a separate unit counter with stack residency, matching the target's observed
  unit slot instead of deriving the unit only inside the SDK argument. C3D0 itself remains
  partial; its corrected layout restores only the two12-byte wrappers C2A6 and C2B2. CA44's
  unsuccessful changes are restored. No partial function receives credit.

VALIDATION:

All eight units received new private native baselines. The three retained units were rebuilt
again under label final after failed changes were restored. Native commands:

    python -B matching-evidence/2026-10-18/campaign100-lane6/run.py all baseline
    python -B matching-evidence/2026-10-18/campaign100-lane6/run.py UNIT final
    python -B matching-evidence/2026-10-18/campaign100-lane6/final_review.py

final-review.json records source, raw object, ELF, ABS and compiler-input hashes. It verifies
current source hashes and independently expands current headers to check every saved compiler
input. check.py rechecks every entire previously exact function/pool/data range against each
new ABS at the original address. All old ranges are preserved. The other five source files
are byte-for-byte restored to their fresh native baseline source snapshots. Full strict
unmasked comparisons are the only source of credit. No global report was modified.

New exact functions: ZOID_SetObjectDef, func_8C068C68, ZOID_ObjectFlags,
ZOID_SetObjectFlags, func_8C06AF60, func_8C06C2A6, func_8C06C2B2.

BLOCKERS:

The complete16-entry task ledger is progress.md. ZIP flush_pending/read_buf were located at
8C068038/8C068094 with byte-identical92/68-byte bodies, but each is8bytes after its target:
these earn zero fixed-address credit. GDFS BA20 already has the correct operations but is
2bytes early. The fresh strcmp intrinsic hypothesis for BAD4 reproduces target56-byte code
shape yet remains14bytes early with a differing pool displacement; no gain and restored.

Queue initialization, MIDI free-node A320 and SDK buffer wrapper A39C have branchless status
or pointer tst remnants without a recovered failure contract. No vacuous checks were added.
Queue has onlyONE mapped function, no initialized-data ranges and its88-byte pool was already
exact at baseline, so its second assignment is an inventory blocker, not a second claimed
function or an invented initialized-data object.

Audio B1C8 prefix-address and signed-temp shapes did not preserve/improve equality; restored.
The fresh named-inline-local OR hypothesis for BlitZDist/XLU regressed the standalone helper;
restored, without repeating prior int-argument or reversed-OR variants. Three bounded LCD
counter/bound shapes were evaluated; CA44 remains uncredited and its original loop is restored.

Generated binaries/preprocessed files remain under build/campaign100-lane6/. Text evidence,
source snapshots and reproducible harnesses remain in this directory. No shared headers,
map, externs, project tools, HANDOFF, objdiff, audit receipts or global reports were written.
No installs, downloads, policy changes, assembly/masks/target-byte fallbacks, fill/BSS credit,
MAT3 writes, game execution, full builds, commits or pushes.
