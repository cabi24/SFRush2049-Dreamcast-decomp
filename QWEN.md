# Qwen handoff - Rush 2049 Dreamcast

Snapshot: 2026-10-18 19:19 America/Chicago. This file is a repository handoff, not a request to restart completed campaigns or proof that any worker is currently running.

## Goal and reading order

Recover maintainable C that reproduces the US v1.001 Dreamcast executable at its original addresses, using GameCube MAT3 as a read-only semantic reference. Work in C:\rush2049-dreamcast.

Read BUILDING.md, include/README.md, tools/README.md, calib/lessons.md, pilot/lanes/toolchain/calibration.md, and relevant source/evidence before editing. HANDOFF.md contains important preservation/layout rules, but its large unit table and headline baseline are historical. CODEX.md describes an earlier pilot before toolchain provisioning. Current saved build/report.json and a successful fresh audit outrank old prose counters. Follow any applicable AGENTS.md if present.

## Current saved state

Observed HEAD: 24e37b1, "Match dc_audio64_init_8C06ADD4: 1244 text + 317 data bytes, 15 functions exact".
Saved build/report.json generated field: 2026-10-18T19:04:47 (timestamp has no explicit timezone).

| Metric | Saved value |
|---|---:|
| Exact functions | 417 / 4,717 total; 417 / 1,555 game |
| Exact game text/literals | 62,498 / 569,664 bytes (10.9710%) |
| Exact whole image | 131,829 / 1,123,492 bytes (11.7339%) |
| Source units | 112 |
| Mapped units | 157; 122 game |
| Fully exact units | 26 |

These values were read from the saved report for this handoff; no new compiler run or audit was performed to write this document. Before claiming a new total, audit the current checkout.

The working tree contains substantial uncommitted matching work and evidence from several campaigns, plus changes to objdiff.json and evidence/strict-integration-receipt.json. Do not reset, clean, overwrite, or indiscriminately stage it. The latest Audio64 source and HANDOFF update are committed, but that does not mean the earlier campaign changes are committed. Inspect git status/diff and include only reviewed source/text evidence in any eventual commit. No remote push was performed by this conversation.

## Target and tools

- pilot/orig/1ST_READ.BIN: 1,123,492 bytes, SHA256 336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688.
- Raw little-endian SH-4, nominal load base 0x8C010000, file-backed end 0x8C1224A4 exclusive. Code and literals are interleaved; do not treat every byte as instructions.
- Original retail/prototype media and archives must remain untouched. Never execute game binaries.
- Compiler: native Hitachi SHC5.1r11. Linker: H-series LNK.EXE 6.0E, supplied by existing reviewed tools.
- Frozen complete flag family: -comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const -speed -noinline.
- dc_build.py supplies the configuration. Do not add -aggressive=2, -extra=asm=1800, -pic=0, or sweep flags.
- C89 declarations/comments; minimal DC headers. MAT3 types/offsets are hypotheses until DC evidence proves them.
- C:\rush2049-mat3 is read-only. Do not resume GameCube or N64 matching.
- No downloads, installs, security-setting changes, full-image builds, watchers or publication are needed for ordinary unit work.
- If native execution is denied, report the exact command/reason and stop that action. Never bypass using WSL/wibo or another launch path. Older calibration prose mentioning such fallbacks is historical.

## Build and audit

Run from the repository root, using PowerShell-safe commands separately:

```powershell
git status --short
python tools/dc_build.py --unit UNIT --no-objdiff
python -B tools/audit_report.py
git diff --check
```

dc_build.py runs splitting and writes shared build/report.json; --no-objdiff suppresses only objdiff regeneration. Inspect unit status, diagnostics and link JSON even if the command exits 1: partial units are common, and exit status alone does not establish a compiler failure. Full native outputs are in build/obj and build/link. audit_report.py validates saved raw objects/ABS, current source/expanded headers, and disjoint emitted address sets; it updates the report and receipt without compiling. It must succeed before publishing totals.

Optional -v listing failed under the default Python with ModuleNotFoundError: No module named 'capstone'. Compilation/linking and strict comparison work without -v. Existing pilot/tools/sh4.py provides a dependency-free partial decoder; validate its limitations and use raw little-endian words as evidence. One worker found objdiff formatted SH words byte-swapped relative to raw target words; do not trust that display without checking bytes. No dependency installation was attempted.

Headless Ghidra is available through tools/ghidra.ps1 and tools/ghidra_scripts/DumpRanges.java. Use existing project RushDC and task-private output; do not silently alter shared project state.

## Matching and credit rules

1. Read implementation, callers, mapped bounds, target instructions and literal dependencies.
2. Establish a current native baseline. Diagnose source semantics, types, code generation and layout separately.
3. Change source shape only with evidence. Verify full target equality at the real address, including branches, pool contents/displacements, generated epilogues and initialized data.
4. Preserve every previously credited function/pool/data span. Restore only your own unsuccessful changes.
5. Record command, source/header hashes, object/ABS hashes, old/new ranges, credited byte delta and remaining mismatch.
6. Park after three evidence-backed source-shape variants per stuck function or roughly 45 minutes without useful evidence. A genuinely new finding can justify a later investigation; don't repeat failed algebraic rewrites.
7. Count actual native source-emitted bytes in wholly exact owned ranges. No copied target bytes, assembly fallback, fuzzy/masked comparisons, moved functions, artificial padding/placement tricks, linker fill, gaps, alignment, initialized record padding, or BSS credit.
8. Internal literal islands within a differing function are not independently credited. A separately mapped external pool can count only if its entire original-address range matches and emitted runs prove ownership.
9. A completed investigation can be a documented blocker. Do not describe an arbitrary task-count quota as recovered functions or compiled attempts.

No full image has been reconstructed/verified; the percentages are the union of separate fixed-address unit links. Names and some boundaries remain reconstructions, not recovered original symbols.

## Parallel ownership

Before spawning or resuming workers, inspect actual live status and coordinate with other sessions. Do not infer activity from files or old chat announcements. Different tools/conversations can have isolated worker families; an identity/AGENTS_BUSY error is a coordination limit, not read-only filesystem mode.

Give workers disjoint src/unit.c ownership and private evidence/build directories. One integrator owns shared headers, map/units.json, src/externs.txt, tools, reports, receipts, objdiff and promotions. Workers must not run canonical dc_build.py main concurrently.

Existing campaign runners show how to import dc_build.build_unit while redirecting C.BUILD, C.OBJ_DIR and C.LINK_DIR (and private SRC/expanded compiler input where needed). Inspect each harness: importing a module is not proof it avoids global writes. Put generated binaries in build/private-name/; preserve only useful text evidence under matching-evidence. Never commit game images, compiler packages, build/, map/raw/, or binary trial packets accidentally saved under evidence.

## Campaign history and pointers

All paths below are under matching-evidence/2026-10-18/.

- work-*/ and statll-followup.md: initial bounded near-miss work, plus exact ZOID_RemovePoly (+48 bytes).
- round2-*/: three new functions and 2,032 bytes; SetCatchup completed model, UpdateRushControls and DrawBlits matched, streaks gained an external pool.
- campaign60-lane1..6/progress.md: 60 investigations, +67 functions / +8,974 audited bytes. NewBlit completed blit; pane completed; fmath recovered 24 additional functions; other gains in controls/options/pakerr/Init/stree/stunt/text/visuals/ZOID/Audio64/LCD.
- campaign100-lane1..6/progress.md: 100 investigations, +13 functions / +1,394 audited bytes. HAL_PurgeFonts, pak-error drawing, TB text inlining, ZOID and Audio64/LCD helpers.
- Subsequent committed Audio64 work at HEAD adds five more exact functions / 320 text bytes, bringing the saved total from 412 to 417.
- campaign-1000/alpha/checkpoint.json: separately observed checkpoint says attempts=11, quota=500, remaining=489, no new exact bytes/functions. It records AGENTS_BUSY: no agent family belongs to this conversation. This is ONE lane checkpoint, not evidence that 11/1000 global tasks are complete or that workers are active. Queue/manifest/events are alongside it. Re-read before resuming and coordinate ownership; do not invent the other lane's status.

Do not restart all 60/100 investigations. Read the per-target residuals and use them as the exclusion ledger.

## Proven compiler lessons

- One upstream size/pool correction can restore many downstream exact functions. fmath's direct vector dot-product expression recovered 24 functions by repairing layout. Prioritize the earliest mismatch in a pool-sharing group.
- Local lifetimes, declaration order, scoped pointers, chained versus separate assignments, and assignment-in-condition affect SHC register allocation and delay slots. Use target evidence rather than arbitrary churn.
- Separate controller zero assignments matched ClearRushControls and UpdateRushControls.
- HAL_PurgeFonts assignment inside >=0 conditions reproduces result-test/copy order.
- SetCatchup needs the scale live before leader search and the target FMUL/FMAC operand order; algebraically equivalent float expressions can differ.
- TB_text uses source-level #pragma inline(TB_CenterText,TB_RightText,TB_GetFontHeight), placed before declarations. Frozen global -noinline does not prohibit target-supported source inline pragmas.
- Float literals truncate during SHC double-to-float conversion. Do not copy MAT3 decimal constants blindly.
- Short-typed division may select __divws/__modws; int-typed division selects __divls/__modls. Preserve proved signedness/prototypes.
- Per-block address rematerialization and volatile accesses can explain target scheduling; do not sprinkle volatile solely to force code.
- The newest Audio64 evidence recognizes a branchless tst after a call as an empty source if condition. calib/lessons.md lines near its end documents the exact proven case. Older blanket wording against empty checks is incomplete: require specific target evidence; never invent missing failure behavior or indiscriminately add vacuous tests.

## Best next work

Review prefix/layout blockers instead of enlarging task quotas:
- timer UpdateFrameTimer is short by two instruction bytes and shifts the shared pool; later wrappers cannot be independently repositioned.
- drivetra enginetorque stack temporary/inline-helper allocation remains unresolved; interp/engine are displaced behind it.
- controls FindPlayerNum length mismatch prevents already moved-exact setter functions from matching at their real addresses.
- fmath NormalVector/context shifts later CreateYPRMatrix/ExtractYaw despite the earlier 25/31 matches.
- highscorescr lacks an internal _SetHighScoreState body, changing call class and predecessor sizes.
- several gamerecords/hud/large screen callbacks remain absent; address-dependent downstream bodies need actual predecessor reconstruction, not filler.
- forces1 in campaign100-lane2 improved from >1000 differing bytes to 62 in a private trial; inspect its saved shapes/residual before a justified follow-up.
- mb_wad MBSetupWad has a saved three-byte register-copy near miss; textureanim InitTextureAnimation also has a three-byte register residual; atan/statll/password remain capped.
- Audio64 is now 15/16. Latest calib/lessons.md identifies func_8C06B298 varargs setup as the residual: handwritten alignment differs from compiler-generated va_start. Inspect current source and actual compiler header/builtin support before any new trial; do not change frozen options or install tools.

Start with status/diff, coordinate active ownership, choose one evidence-backed blocker, and baseline privately. Audit only after deliberate integration. Report exact new functions/bytes separately from investigations and near misses.

