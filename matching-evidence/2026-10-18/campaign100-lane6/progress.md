# Campaign100 lane6:16 target assignments

Strict fixed-address whole-range equality, frozen native SHC5.1r11. Binaries private build/.

| Task | Unit | Target | Status | Attempts | Delta | Residual |
|---|---|---|---|---|---|---|
|1|zip|flush_pending|blocked|0|0|92raw bytes exact at68038,target68030; earlier zip layout shifts8bytes; moved is uncredited|
|2|zip|read_buf|blocked|0|0|68raw bytes exact at68094,target6808C; same8byte predecessor shift; moved is uncredited|
|3|zoid|SetObjectParent|bounded|3|shared win below|scoped pointer leaves19byte register residual but fixes pool; no fourth shape|
|4|zoid|func_8C068C68|won|3|unit+4funcs/+160text|for-loop with terminal break matches94bytes; restores three neighbouring functions; old exact spans preserved|
|5|dc_unk_8C069E34|MIDI initialization function|blocked|0|0|target has branchless SDK status tst operations; unknown failure/diagnostic contract; no vacuous checks added|
|6|dc_unk_8C069E34|unmatched initialized-data inventory|inventory blocker|0|0|ONE mapped function,no initialized data and pool already exact88bytes: no distinct second unmatched function/data target exists|
|7|dc_sound_8C069F78|func_8C06A320|blocked|0|0|target tst of free-node pointer atA32C without failure branch; omitted diagnostic contract unknown|
|8|dc_sound_8C069F78|func_8C06A39C|blocked|0|0|three branchless SDK status tst sites atA3AC,A3CA,A3DA; no manufactured checks|
|9|dc_audio64_init_8C06ADD4|func_8C06AF60|won|1|+1func/+100text|explicit time-address local reproduces r7 hoisting and100byte full match; all old exact spans preserved|
|10|dc_audio64_init_8C06ADD4|func_8C06B1C8|bounded restored|2|0|explicit prefix-address local adds4bytes/regresses neighbours; signed error temp identical; restored unsigned original|
|11|dc_blit_8C06B2EC|BlitZDist|bounded restored|1new|0|named OR-local inline shape regressed standalone helper; did not repeat old int-argument/reversed-OR shapes|
|12|dc_blit_8C06B2EC|BlitXLU|bounded restored|1new|0|same new inline-local hypothesis fails; restored baseline3/9byte residual wrappers|
|13|dc_misc_8C06B80C|func_8C06BA20|blocked|0|0|26byte body already correct apart from pool displacement and2byte predecessor shift; no padding inserted|
|14|dc_misc_8C06B80C|func_8C06BAD4|bounded restored|1|0|builtin strcmp gives expected56byte body but still shifted14bytes and pool displacement differs; no strict gain, restored|
|15|dc_misc_8C06C08C|func_8C06C3D0|bounded with strict neighbouring gain|3|unit+2funcs/+24text|explicit unit counter with stack residency restores C2A6/C2B2 call displacements; main body remains partial; old exact spans preserved|
|16|dc_misc_8C06C08C|func_8C06CA44|bounded restored|3|0|integer-bound,explicit gate,base-local forms fail full address match; original for-loop restored|
