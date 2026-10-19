# Round 2: state and model

RESULT: model is fully exact (3/3 functions), adding SetCatchup and 1,402 emitted text/literal bytes. State remains 5/6, unchanged source.

## Reproduction

From C:\rush2049-dreamcast:

```
python matching-evidence/2026-10-18/round2-state-model/run.py model final
python matching-evidence/2026-10-18/round2-state-model/record.py
```

run.py calls dc_build.build_unit directly with BUILD, OBJ_DIR and LINK_DIR redirected to build/round2-state-model/<unit>/<label>. It does not invoke dc_build.main, global report updates, split or objdiff. Frozen native SHC5.1r11 flags are inherited from the existing builder. No masks, assembly, original-byte emission, toolchain variations, game execution or full-image build were used.

## Model source evidence

The target loads 0.1f into FR10 at 8C040D0C/8C040D14 before the leader-search loop. A block-local catchup_scale initialized before high_index reproduces this hoist, improving SetCatchup from 1,245 differing bytes and a -2-byte size delta to only45 differing bytes and zero size delta.

The residual consists of five repeated blend tails in the compiler-unrolled loop. Target multiplies target_catchup by0.03 first and fuses0.97 times old catchup into that accumulator. Writing old*0.97 +target*0.03 reproduces this sequence exactly; C expression order controls the compiler's right-first evaluation and FMAC selection.

Final independent raw comparison: all1,532 text bytes at8C040C24–8C041220 equal target, and initialized data byte8C0BEE80 equals target. Earned text credit is1,520 (12 fill/gap bytes excluded), up from118. SetCatchup's mapped function span is1,398 bytes; additional literal credit accounts for the remainder. Initialized data credit stays1.

## State residual and rejected trials

Baseline initGameState has6 differing bytes, no size delta. At8C0510A6 target MOV #0,R11 differs from MOV R13,R11. Four remaining byte differences swap temporary pointer spill slots across the final two ZOID_SetFog calls (8C05127A,8C051282,8C051298,8C0512A0).

1. Reuse function-local i for both controller-loop branches: unchanged6-byte residual.
2. Introduce explicit blue/green pointers before fog calls:107-byte function residual, -12-byte TU delta; damages later matches.
3. Make the second branch's loop index signed short:536-byte function residual, +12-byte TU delta; damages later matches.

All three were rejected; state source restored byte-for-byte to its entry SHA256. Stop rule reached; no further state trials were run.

## Files and integration

Only src/model.c is intentionally modified. src/state.c has original bytes. manifest.json contains every trial summary and exact SHA256 values for final source, raw object and linked ABS. model-final.json is the complete private build entry. Canonical report, receipt, objdiff, map, shared headers and externs were not modified by this worker. Parent integration must publish/admit the final model result through its own audit workflow; this packet does not claim global totals.
