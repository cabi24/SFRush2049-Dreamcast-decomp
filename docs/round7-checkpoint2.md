# Round 7 checkpoint 2: ongoing campaign

This checkpoint adds 134 game code/literal bytes, 170 image bytes and two exact
functions. Combined round-seven gains are 240 game bytes, 276 image bytes and five
exact functions. Current totals: 90,218 / 569,664 game bytes (15.8371%), 159,585 /
1,123,492 image bytes (14.2044%), and 584 / 1,555 exact game functions.

- Audio64: func_8C06B298 (60 bytes), the existing varargs diagnostic formatter.
  Passing the observed argument-pointer alignment conditional directly to the formatter
  instead of through a local pointer reproduces native register selection. No ABI/layout
  change or invented stack object. All 16 functions in this unit are now exact.
- pakerr: _AnimDim (28 bytes), recovered through a target-backed ShowPakError predecessor
  repair. A real unsigned-short local retains the text-table index for two uses; chained
  clearing reflects the observed choice2-then-choice1 store order. A stale comment that
  conflated parameter spill placement with store order was corrected. ShowPakError remains
  partial and is not claimed exact. Net gain is 74 code/literal bytes plus 36 data bytes.

The parent independently replayed frozen source snapshots, ran a full 114-unit build and
strict unmasked emitted-byte audit, and checked all 579 pre-round exact functions and
previously credited spans remain preserved. All 22 host tests passed. No flags, shared
headers, map or scoring changes were made.

Independent negative controls: changing the Audio64 alignment mask from 3 to 7 creates
one differing byte and loses the 60-byte function. Changing _AnimDim return value from 1
to 0 creates four differing bytes and loses the full 28-byte function credit. Sources
were restored and rebuilt before the final audit.

Receipt: `evidence/linux-round7-checkpoint2-20261028.json`. Tool hashes describe the
separately restored, approved hash-pinned Linux backend, not the native wrappers retained
in this source-only publication. No full executable relink, gameplay validation, native/Linux
raw-object parity or CI pass is claimed. No CI watcher was started. The requested new
500-target campaign is ongoing, not completed.
