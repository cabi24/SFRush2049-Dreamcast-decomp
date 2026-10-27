# Round 6 checkpoint 1 — ongoing 500-target campaign

This is an independently verified source checkpoint, **not completion of the
500-target campaign**. Four workers continue with disjoint unit ownership.
The round starts from public master `1eb880efdded9b7e8425f8f5025c46c03df21668`.

## Accepted gains

| Unit | New emitted code/literal bytes | New exact functions | Evidence-backed source change |
|---|---:|---:|---|
| asincos | 8 | 2 | Register-qualified y/g lifetimes in farcsine restore the two tiny fasin/facos wrappers; farcsine itself remains nonexact. |
| audio | 174 | 1 | Early-return game-state guard recovers StartListeners register allocation. |
| audiosfx | 228 | 1 | Register scan index and pre-call voice pointer in func_8C016044, retaining the observed post-call global rereads. |
| camera | 568 | 5 | Fixed car-arena expression, direct state test and chained view/saveview stores fix init_views/init_view3 and nearby pool-dependent functions. |
| Total | 978 | 9 | No changed flags, maps, scorer or common headers. |

Totals: **89,542 / 569,664 game code/literal bytes (15.7184%)**,
**158,873 / 1,123,492 image bytes (14.1410%)**, **578 / 1,555 exact game functions**.
All 569 previously exact functions and every previously credited byte are preserved.

The integrated source passed a full 114-unit frozen compiler/linker build, the
strict unmasked emitted-byte audit, and all 22 host tests including six credit
controls. Independent deliberately wrong controls were rejected: fasin flag
(1 differing byte), StartListeners state mask (1), init_view3 default view (1),
and the audio scan bound (170). Each accepted source was restored and rebuilt.

Receipt: `evidence/linux-round6-checkpoint1-20261027.json`.
Tool hashes identify the separately reviewed explicit pinned Linux workflow;
the native wrappers retained in this source-only public commit are not claimed
to be those Linux wrappers. No raw native/Linux object parity, whole-game relink,
gameplay runtime test or CI pass is claimed. No CI watcher was started.

Research and unresolved reconstructions remain separate. Historical source
studies do not automatically make a target unworkable: renewed probes require
specific new evidence, while exhausted shapes are not repeated. Changed-body,
new-source reconstruction, and genuine target-specific placement-dependency
attempts will be reported separately. Inspection-only work, incidental movement
and unchanged recompiles do not fulfill the requested target count. Final
500-target outcomes and research publication are still pending.
