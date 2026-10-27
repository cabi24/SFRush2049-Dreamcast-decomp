# Round 6 checkpoint 2 — campaign still ongoing

This checkpoint adds **436 emitted code/literal bytes and one exact function**
(`multiinit`) in mdrive, preserving every prior credited byte and exact function.
Together with checkpoint 1, the campaign has added **1,414 bytes and ten exact functions**.
The 500-target request is **not yet complete**; research and conservative target-count
reconciliation remain in progress, with an explicitly reported credible-target shortfall.

The source changes are ordinary target-backed lifetime repairs in predecessors:
`init_cars` establishes the model/car bases before clearing their objects and retains
those bases; `func_8C03DD58` retains its observed six-car count across link writes rather
than rereading the aliasable global. Neither predecessor is claimed exact merely because
its repair recovers `multiinit`. No new volatile qualifiers, padding, flags, map edits or
scorer changes were introduced.

Totals: **89,978 / 569,664 game code/literal bytes**, **159,309 / 1,123,492 image bytes**,
and **579 / 1,555 exact game functions**. See the receipt for precise percentages.
A full 114-unit frozen build, strict emitted-byte audit, all-old-span/function preservation
and 22 host tests passed. Independently changing the third SetResurrectionData argument
inside multiinit produced three differing bytes; restored source rebuilt and audited cleanly.

Receipt: `evidence/linux-round6-checkpoint2-20261027.json`.
As in checkpoint 1, tool hashes describe the separately reviewed explicit pinned Linux
verification lane, not the native wrappers retained in this source-only publication.
No full-game execution, raw Windows/Linux artifact parity, CI pass or completion of all
500 attempts is claimed. No CI watcher was started.
