# Round 6 lane2 research summary

122 actual targets: 59 existing-body probes and 63 real reconstructions; no placement-dependency count. Three slots remain short of the original 125-target quota. Camera's five exact gains were independently accepted on master; only two source-changing targets contributed to the fresh-target count, with other gains incidental.

Research completes 35 pak reconstructions, 21 HUD callbacks/helpers, three shot/defense bodies and four real functions in two native units. Source order, SDK calls, native structure sizes and callback identities were investigated rather than inventing missing stubs. Eight HUD data externals remain unresolved: lapCountPos, lapTimeBGColor, lapTimePos, playerColor, radarHeight, radarPos, radarWidth and wrongWayTime.

Research compilation succeeded for 26 owned units but is not full acceptance: old spans may regress; incomplete translation units, data layout and unresolved HUD symbols remain. The speculative HUDextra race-state else-if and UpdateCarObj probes were restored. Existing compiler/32-bit layout checks are diagnostic evidence, not a gameplay-correctness guarantee.

This normalized summary describes the immutable worker packet and its historical verification. The archived patch is a source-only delta against accepted checkpoint2, not a request to promote source. No fresh combined build is claimed by this packaging step.
