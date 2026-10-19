# Campaign 60 lane 1

| Assignment | Status | New credit | Residual |
|---|---|---:|---|
| timer | assessed, baseline retained | 0 | Target UpdateFrameTimer reloads counter address in else; 2-byte code delta shifts pool affecting every function. Signed-operand shape unchanged. ABI-sensitive declaration changes parked. |
| unk_8C0102A4_lz | completed, baseline retained | 0 | Three shapes (register bits, scoped copy, mask expression); 83/101/100 diff bytes vs baseline101. Register allocation not solved. |
| unzip | completed, baseline retained | 0 | Plain l shape regressed prior alloc_zip_mem exact via shared-pool shift; restored. Existing stack-qualified l remains unproven, unsuitable for further blind changes. |
| asincos | completed, baseline retained | 0 | New decoder evidence suggests g expression should avoid intermediate move; combined g lowers180 to172 differing bytes but no strict range. Prior trials exhausted, restored. |
| audio | completed, baseline retained | 0 | StartListeners remains2-byte r2/r3 mismatch. Reversed loop, slot negation, explicit state comparison tried; none exact, reversed loop regresses shared pools. |
| audiomgr | assessed, baseline retained | 0 | AudioInit first code almost exact but function pointer and downstream pool addresses wrong. Four-byte unsourced gap before maintenance cannot be filled by a fabricated function. ProcessCommand target has initialization guard; guard shape alone no strict credit, restored. |
| audiosfx | completed, baseline retained | 0 | Three shapes first bank function: >=256, allocation size argument, explicit return status; diff24 to22 but no exact range. Target at8C015FBE loads original size stack0 (after push offset4), whereas original source passes available stack4 (after push offset8); semantic correction proposal retained only in private snapshot. |
| audioscr | assessed, baseline retained | 0 | First function stack16 target vs12 built and substantial source-shape drift moves every later function. validOption matches bytes only at moved address; no credit. Arbitrary stack padding or target stub relocation rejected. |
| blit | completed, strict improvement retained | 204 text bytes /1 function | NewBlit pointer register lifetime evidence: register blit plus ordinary drawX makes exact. 9/10 to10/10,1118 to1322 text bytes; data4 unchanged. Prior DrawBlits match retained. |
| gfx | completed, baseline retained | 0 | Previously exhausted config and assignment forms. New separate converted local lifetime shape based on r1 reuse unchanged2-byte callback residual; restored. |

All ten bounded assignments assessed. Only additional canonical change is NewBlit register placement. Native frozen SHC5.1r11 build_unit, private paths under build/campaign60-lane1; no global report/audit/objdiff writes. Selected blit full fixed-address unmasked equality validated, including all pools/data/BSS;1322 emitted text bytes exclude2 fill bytes. Hashes in hashes.json. Prior lane1 report contribution is only204 new bytes: earlier DrawBlits80 belongs to previous round.
