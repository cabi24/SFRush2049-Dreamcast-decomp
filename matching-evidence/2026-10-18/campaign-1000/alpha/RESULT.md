# Lane alpha checkpoint

RESULT: 11 additional source-changing attempts with native compiler, fixed-address linker and strict comparison results. Zero new exact functions and zero new credited bytes. 489 of the 500 alpha attempts remain pending. Baselines and diagnostics are not counted. The 1000-attempt campaign is incomplete.

CHANGES: Only this private alpha packet was written. No shared source, header, map, extern, report or objdiff change was made by this lane. Expanded candidate sources, native objects, absolute links, reports and SHA-256 manifests are under runs/. queue.csv reserves A0001-A0500; events.jsonl is append-only. Unassigned pending IDs are reservations, not invented evidence-backed assignments.

VALIDATION: run.py imports the existing dc_build implementation and redirects SRC, BUILD, OBJ_DIR and LINK_DIR to this packet before calling build_unit. It never calls the shared build main/split/objdiff writers. Frozen native SHC5.1r11 and LNK remain unchanged. Each run retains source.c, report.json, hashes.json, obj and link artifacts. Reproduce using the corresponding saved source as candidates/UNIT/UNIT.c and run `python -B matching-evidence/2026-10-18/campaign-1000/alpha/run.py UNIT NEW_LABEL`. Do not reuse a label: output creation deliberately fails if the run already exists.

| IDs | Target | Evidence and disposition |
|---|---|---|
| A0001-A0003 | cheatscr / animText | Format-pointer lifetime, output-pointer-before-call, and buffer lifetime. Baseline51 differing bytes; trials171/843/842. Baseline4 exact functions preserved, but latter trials regress pool credit1482 to1470 and emit28 out-of-map bytes. Reject all; three-change cap. |
| A0004-A0006 | dc_zoid / func_8C06EF10 | Ordinary packet qualifier unchanged; explicit register vertex local47 to23 diff bytes; ordinary vertex local remains23. No full match. Three-change cap. Target needs r3 vertex copy and repeated r2 loads, while candidate selects r7 and alternating temporaries. |
| A0007 | dc_zoid / func_8C06EECC | Reversing maximum comparison operands reproduces target cmp/hs + bt/s and reduces11 to8 differing bytes. Remaining8 are literal-load displacements; no whole-function credit. |
| A0008 | dc_zoid / func_8C06EDFC | Same target-proven operand order at three maxima reduces43 to34 differences. Remaining early pointer/zero temporary register selection plus downstream pools. No credit. |
| A0009-A0011 | dc_zoid / func_8C06EF40 | Cache face index, for-loop, explicit bottom-test labels. Final initial branch allows compiler to put shared literal pool inside this function, like target. EF10 linked size120 becomes46; EF40 starts EF3E versus EF40 and size284 versus290. Still differs; three-change cap. |

The renderer candidate after A0011 is diagnostic only and is not selected for promotion. Its improvements are independently reviewable, not strict gains. All baseline source snapshots are retained. Controls baseline already has7 exact functions/1742 text bytes from earlier work; none belong to this lane. dc_misc baseline's first2-byte residual is pool displacement, and B82C contains unresolved diagnostic comparisons; no speculative body was added.

BLOCKERS: CoS prime routing failed exactly: `AGENTS_BUSY: no agent family belongs to this conversation. Other primes and their workers are isolated. Call agents action=spawn to create workers owned by this chat.` This prevents delivering the requested worker report to the original prime and verifying live ownership through that family. It does not block workspace operations. Do not create a replacement family to impersonate the requested worker. The task remains resumable here, with remaining eligible targets requiring further evidence/ownership review.

Diagnostic environment: calib/sh4dis.py import failed `ModuleNotFoundError: No module named 'capstone'`; dependency-free pilot/tools/sh4.py worked after renaming the private diagnostic script from inspect.py to words.py to avoid standard-library shadowing. No installation or alternate compiler launcher was used.
