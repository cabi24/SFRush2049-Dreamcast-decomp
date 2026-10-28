# Round 7 checkpoint 1: ongoing campaign

Three newly exact functions add 106 emitted code/literal bytes:

- gfx: func_8C02E124, 66 bytes. A target-backed early return for duration greater
  than 40000 replaces the equivalent enclosing conditional and recovers register allocation.
- dc_status: func_8C06CA98 and func_8C06CAA8, 16 and 12 bytes, plus their 12-byte
  shared pool. The nonexact following helper keeps its observed signed-byte boolean
  conversion in an int temporary, removing redundant save/restore
  instructions and recovering the common pool placement. That helper remains two bytes off;
  it is not counted exact.

The parent independently replayed both immutable worker candidates, reviewed their source,
ran the full 114-unit frozen build and strict unmasked emitted-byte audit, and proved all
579 previous exact functions and every previously credited address remain preserved.
All 22 host tests passed. No compiler flags, map, shared headers or credit rules changed.

Independent negative controls: changing the gfx threshold to 40001 leaves the function
body exact but changes its shared literal pool, losing 36 credited bytes. Changing the
CAA8 status sentinel from -3 to -2 creates one differing body byte and loses that 12-byte
function. Both controls were restored and rebuilt before the final audit.

Totals: 90,084 / 569,664 game code/literal bytes (15.8135%), 159,415 / 1,123,492 image
bytes (14.1892%), and 582 / 1,555 exact game functions.
Receipt: `evidence/linux-round7-checkpoint1-20261028.json`.

The dedicated receipt records the independently restored, hash-pinned Linux verification
backend. Those tool hashes do not describe the native wrappers retained in this source-only
publication. No raw native/Linux object parity, full executable relink, gameplay validation
or CI pass is claimed; no CI watcher was started. The new 500-target request remains ongoing.
Candidate selection and inspection are not completed attempts or matching credit.
