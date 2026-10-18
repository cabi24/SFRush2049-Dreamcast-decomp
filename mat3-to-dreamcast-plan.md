<!-- BEGIN MAT3 TO DREAMCAST PLAN: evidence-based plan, 2026-10-18 -->
# MAT3 to Dreamcast: a bounded plan for exact matches

**Use MAT3 to recover identities, interfaces, data meaning, and candidate C; identify and reproduce the Dreamcast compiler/ABI/linker before scaling byte matching.** PowerPC bytes cannot match SH-4 bytes. MAT3 is a reference snapshot, not another matching task. Leave `GetCarsIRColor`, `Blits`, and `Blits_First` alone.

This is a plan, not execution authorization for a matching wave. This pass read MAT3 and Dreamcast evidence, checked an existing MAT3 DOL without rebuilding, and wrote this document/reports. No source edits, new compilations, installs, game execution, publication, CI watchers, or agent matching wave occurred.

## What is established

- **MAT3 baseline:** current local report has **1,841/1,844 functions, 713,440/715,968 game-code bytes (99.6469116%), 146/148 units configured from C**. The root `progress.json` and README intentionally describe an older 60.37% review snapshot. The existing built DOL passes `verify_link.py --no-build` with exactly the five recorded GC/2.7 destructor/copy-table exceptions. This verifies stored artifacts; it is not a fresh source-to-object rebuild. Do not transfer those five exceptions to Dreamcast.
- **Source pin:** local `main` HEAD read directly is `f43bbe4bbc6948ffa9ff09bf64af6d9430e8cb73`; selected current files are hash-pinned separately. Native git.exe could not read the checkout through this sandbox, so no clean-worktree claim, fetch, or sync is implied.
- **MAT3 original:** `rush_d.elf` is ELF32 big-endian PowerPC, SHA-256 `d4a06e4516ab9a75e6de780beee10ea07b8358016e4964a8baf9e8eaa282908c`. It has 12,668 symbol entries (11,132 named; 3,181 function and 7,431 object entries across the ELF), DWARF v1 `.debug` (1,926,116 bytes), `.line` (372,142 bytes), 149 compile units, and 37,095 source-line rows. All CU producer strings say `MW EABI PPC C-Compiler`. The 149 CUs and all-ELF symbols are different scopes from the 148 game units/1,844 game functions. Repeated type DIEs across CUs are not unique source types.
- **Dreamcast target:** retail US v1.001 `1ST_READ.BIN` is 1,123,492 bytes, SHA-256 `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`. It is byte-identical to August 1, raw LE SH-4 at nominal `0x8C010000`, with startup jump target `0x8C0D2480`. No usable symbols, source-line tables, types, original objects, or link map were found.
- **Strongest bridge:** all **315 MAT3 `gWaveFilenames` entries match Dreamcast filenames in the same order**, and all **315 MAT3 `gWaveSizes` values match decoded LZS sizes or raw ROM sizes**. Both arrays were verified against the original MAT3 ELF, not just reconstructed C. This anchors resource IDs; it does not prove payload equality or structure layout across platforms.

References: [latest handoff](../rush2049-mat3/HANDOFF.md), [current local build report](../rush2049-mat3/decomp/build/2049LE/report.json), [original ELF](../rush2049-mat3/rush_d.elf), [DWARF function CSV](../rush2049-mat3/rush_d_functions.csv), [types](../rush2049-mat3/decomp/include/rush/rush_types.h), [prototypes](../rush2049-mat3/decomp/include/rush/rush_protos.h), [overrides](../rush2049-mat3/decomp/include/rush/overrides.json), [Dreamcast overview](overview.md). [Read-only inspection summary](mat3-to-dreamcast-evidence/summary.json) and [source/report hash pins](mat3-to-dreamcast-evidence/reference-file-hashes.json).

## Use the shared resource table first

Dreamcast's 315-entry filename table is at file `0xB44C0` / VA `0x8C0C44C0`, ending at `0xB49AC` followed by zero. MAT3's corresponding arrays are in `waves.c` and `waveinfo.c`. That is a better initial anchor than trying to recognize anonymous SH-4 routines from PowerPC instruction similarity.

Start the future correspondence map at references to this table and `texte.lzs` (`0xA09B4`) / `debugfonttex.lzs` (`0xA0A18`), then work through resource state, loading, model setup, and callers. These are string/data addresses, not recovered function entries. All 315 source names/order/sizes are verified; **none of the seven pilot Dreamcast function addresses is being claimed as identified by this planning pass**.

February/April beta tables still help: their 155/160 `kWave...` names give earlier semantic labels and more diagnostic text. Their indices are not retail indices (April debug-font texture index 6 becomes retail 8). The MAT3-to-retail table alignment is directly verified. August 1 is the same executable as retail, so annotations on that exact image transfer at the same addresses; the two discs still differ in metadata, two unused banks, and a small audio region.

[MAT3 waves.c](../rush2049-mat3/decomp/src/GameSource/src/waves.c) (line 14); [waveinfo.c](../rush2049-mat3/decomp/src/GameSource/src/waveinfo.c) (line 3); [retail table](retail-v1.001-evidence/resource_filename_table.json); [315-row ELF-correlated agreement](mat3-to-dreamcast-evidence/resource-agreement.json); [beta catalogue](betafindings.md).

## Gate 1: compiler, ABI, assembler, and linker reproduction

### Available tools and missing pieces

Local MAT3 tools include GC 2.6 `mwcceppc.exe`, GC 2.7 `mwldeppc.exe`, other GC/Wii/X360 and ProDG **NGC** compiler bundles, dtk, objdiff, and Ninja. These are not a located Dreamcast toolchain. No `shc`, SH GCC/assembler/linker/objdump, Java/Ghidra, or LLVM tools were found on PATH or in the checked common tool roots/filtered installed-app registry. WSL's launcher exists; no usable toolchain inside a WSL distribution was established. This was a bounded inventory, not an exhaustive disk search.

**Execution dependency:** locate an existing period SH-4 compiler package, assembler, linker, matching headers/runtime libraries, and a SH-4 disassembler/analyzer. Hash exact files and record version banners/help, provenance, host requirements, library versions, and complete command lines. Do not download/install automatically or treat the existing PPC tools as an SH backend. Check objdiff/dtk architecture/object-format support before adopting them; reuse their workflow concepts and independent validation, not their PPC-specific assumptions or hard-coded flags.

Rank the first *hypotheses* as period Katana-compatible Hitachi SHC and period SH GCC/Binutils; a third family requires positive local evidence. No vendor/version is yet established. Shinobi 1.68 / Audio64 10.0.f.2 / Kamui2 16,3,4,0 are SDK fingerprints, not compiler proof, and SDK objects and gameplay objects may have different producers. Modern KOS/GCC or Renesas releases can help analysis but are not evidence of the 2000 original toolchain.

### Bounded calibration, not an unbounded flag search

1. Establish DC pilot boundaries/call sites first. Read prologues, callee saves, integer/FP argument placement, narrow returns, helper calls, delay slots, literal loads, switch lowering, and FPU mode changes. Compare independently across gameplay functions; separate known SDK routines.
2. Build a minimal standalone C environment in a **new Dreamcast workspace**: explicit integer widths, DC-specific field overlays, and declared external call/data addresses. Do not include the complete GC header tree, `dolphin.h`, PPC intrinsics, or MAT3's MSL inline math. Freeze ABI probes for char signedness, short return extension, enum/bitfield layout, double alignment, struct return, FP arguments/returns, varargs, and `sizeof`/`offsetof` actually used.
3. Train on four pilot functions below. Start with at most **16 evidence-supported compiler/version/option configurations total** (64 single-function compilations), grouped by output fingerprints. Vary only options that observed differences justify: CPU/endian/ABI, speed-vs-size, FP policy, inlining, pooling, then relaxation. Reproduce the assembler and linker too. Do not transplant `-O4,p -fp_contract on` from MAT3.
4. Rank each configuration by exact per-function emission/relocation evidence plus exact fixed-address mini-link bytes, not average fuzzy percent. A plausible family must fit a nontrivial integer/pointer case **and** FP behavior; two tiny getters or identical `rts` sequences prove little. Freeze the best configuration and test the three held-out functions without tuning it.
5. Claim a reproducible pilot configuration only when the mapped pilot spans, pools, and data pass exact local links consistently, including held-out arithmetic and global access. If several versions remain byte-indistinguishable, keep the version range rather than naming a false exact release. Full original toolchain identity remains a hypothesis until enough discriminating evidence exists.

Official option references show why ABI/FP/relaxation settings must be recorded: [GCC SH options](https://gcc.gnu.org/onlinedocs/gcc/SH-Options.html) documents CPU, endian, calling-convention, double-alignment and link-relaxation choices; the [Renesas SH option guide](https://www.renesas.com/en/document/apn/sh-compiler-application-note-2-compiler-use-guide-option-guide) describes optimization-dependent output. These are explanatory references, not proof of period flags or the game's compiler; use the selected vintage release's own documentation/help for executable commands.

[Scoped local tool inventory](mat3-to-dreamcast-evidence/tool-inventory.json) lists the checked paths and hashes; it does not declare software absent from every disk location.

## A seven-function pilot from actual matching MAT3 C

Every selected function is recorded at 100% in the current MAT3 report, and its TU is configured `Matching`. The PPC addresses/sizes below are reference identifiers only. Their sum, 1,364 PPC code bytes, is **not** a Dreamcast denominator or expected SH-4 size. Initial DC identification/emission/link results are all pending.

| Set / function | Actual MAT3 source | PPC address / bytes | Why it earns a pilot slot |
|---|---|---|---|
| Train: `bsearch` | [bsearch.c](../rush2049-mat3/decomp/src/GameSource/src/bsearch.c) (line 5) | `0x80005E50` / 224 | indirect comparator, loop, integer args |
| Train: `InsertLLE` | [ll.c](../rush2049-mat3/decomp/src/GameSource/src/ll.c) (line 22) | `0x80025374` / 340 | branching, pointer-to-pointer structures |
| Train: `interp` | [drivetra.c](../rush2049-mat3/decomp/src/GameSource/src/drivetra.c) (line 182) | `0x8000F234` / 24 | signed division, narrow return, leaf |
| Held out: `enginetorque` | [drivetra.c](../rush2049-mat3/decomp/src/GameSource/src/drivetra.c) (line 143) | `0x8000F090` / 420 | integer lookup/clamps plus floating scale |
| Train: `BodyVector` | [fmath.c](../rush2049-mat3/decomp/src/GameSource/src/fmath.c) (line 239) | `0x8009C0B4` / 124 | single-precision arithmetic/order, matrix data |
| Held out: `UpdateFrameTimer` | [timer.c](../rush2049-mat3/decomp/src/GameSource/src/timer.c) (line 45) | `0x80050CD0` / 208 | globals, volatile loads and scheduler wrapper |
| Held out: `GetWaveByIndex` | [waves.c](../rush2049-mat3/decomp/src/GameSource/src/waves.c) (line 325) | `0x8005BF6C` / 24 | direct lookup/global resource state |

Training set: `bsearch`, `InsertLLE`, `interp`, `BodyVector`. Held out until one configuration is frozen: `enginetorque`, `UpdateFrameTimer`, `GetWaveByIndex`. Keep all seven slots, even when a DC counterpart is absent, inlined, or drifted; record that outcome rather than replacing a hard case with a trivial win.

Focused fixtures once execution begins:

- `bsearch`: zero count/size, found/missing keys, odd/even counts, comparator call order and equal-key behavior. Standard-library name alone is weak identity evidence.
- `InsertLLE`: direct vs handle, singly vs doubly linked, head/middle/tail, null insertion; assert the complete next/prev graph and element count.
- `interp`: signed operands/remainders, sign extension of the short return, exact division behavior. Preserve target-defined preconditions; do not invent a divide-by-zero contract or rely on host C signed-overflow behavior.
- `enginetorque`: constants 1150/1149, 14/13, clamp edges, short table elements and stride 12, plus FP scale. This is a discriminating caller for `interp`, not a reason to copy GC `tModelData` offsets.
- `BodyVector`: nonidentity/nonorthogonal matrices distinguish it from `WorldVector`; test evaluation/rounding order, signed zero, and any allowed aliasing from DC callers.
- `UpdateFrameTimer`: paused/running, retrace-count changes, five-frame clamp, wraparound and only target-supported conversion ranges. MAT3 inferred `volatile` qualifiers are evidence to recheck, not DC facts.
- `GetWaveByIndex`: verify the actual wave-instance field and stride before testing first/last valid instances. It returns `.wave`, not the instance pointer. Do not count this tiny routine as independent compiler confirmation.

Run only the changed fixture set and relevant local link. A host semantic harness can use an available compiler; tests that depend on SH FP/ABI require a target-aware instruction harness or later approved runtime setup. These are future checks, not tests executed in this planning pass.

## Build correspondence without importing the GC backend

Maintain one row per correspondence: MAT3 CU/function/address + source hash; DC address/ranges + boundary confidence; signature/field map; string/data/resource references; callers/callees; constants; inlining/splitting/merging; version-drift notes; evidence status; owner. A MAT3 name is a proposed label until DC evidence supports it.

Use a repeatable **identification score /100**: target behavior/control-flow/side-effect sequence 30; independently identified call-neighborhood 25; DC field/stride/signedness pattern 20; uncommon constants/table/string references 15; resource-ID agreement or a second independent caller contract 10. Common zero/one constants and generic returns earn no uniqueness points. Require >=80, at least two independent evidence categories, and no unresolved contradiction to accept an identity; 60-79 is a candidate; lower stays unknown. Tiny routines additionally need their caller's argument/data contract. Record a runner-up and require a 20-point lead. The score is a prioritization rule, not proof; a conflicting side effect/return/layout vetoes acceptance until explained.

Normalize names and platform seams for **search only**: abstract `GDFS_*`, allocator, timer, controller, graphics and audio wrappers into semantic roles, while retaining the actual DC callees and effects in candidate C. Follow both callers and callees and compare arrays/tags/state transitions. Allow one-to-many or many-to-one source/compiler correspondence without counting overlapping DC bytes twice.

**Real drift examples already visible in MAT3:** `_ReadWave` adds `LoadTextureBinFile`, `LoadGeoBinFile`, and `ReMapTexToDCDef`; `fakegdfs.c::_LoadFile` opens `roms/%s` through `appFile*`; `paksysdc.c::PS_SetRumblePercent` calls `NGCRumblePad` despite the filename's `dc` suffix. Preserve the game-level intent, then reconstruct DC loading, model setup and I/O from DC instructions. No backend guess should become a patched-in GC behavior or an assumed GC bug fix. Maintain a drift ledger: source port change, platform wrapper, struct-layout change, compiler lowering, or unresolved; seek two independent anchors before choosing one.

[Resource loader seam](../rush2049-mat3/decomp/src/GameSource/src/waves.c) (line 304); [fake GDFS](../rush2049-mat3/decomp/src/GameSource/src/fakegdfs.c); [memory-card wrapper seam](../rush2049-mat3/decomp/src/GameSource/src/paksysdc.c) (line 675); [GC math inline](../rush2049-mat3/decomp/include/msl_math_inline.h) (line 40); [NGC rumble control](../rush2049-mat3/decomp/src/NGC/ngcPad.c) (line 285).

## Four distinct acceptance states

| State | Evidence required | What it does not establish |
|---|---|---|
| **I: identified** | Accepted correspondence, exact DC ranges, callers/data contract | Equivalent C or any byte match |
| **E: equivalent C** | DC-traced behavior and focused fixtures/invariants; intentional differences from MAT3 documented | Original source spelling, compiler identity or exact bytes |
| **O: object emission / byte comparison** | Generated object hashes; emitted code/data/pools; explicit typed relocations and addends; exact nonrelocated bytes or exact mini-link span at target addresses | A historical original-object match: no original DC `.o` exists to compare |
| **L: linked verification** | Exact unmasked fixed-address local linked spans first; later complete 1,123,492-byte image with pinned SHA-256, layout and startup | Earned source coverage for any original blobs still passed through |

Do not label a relocation-masked/fuzzy score as an exact object match. With only a flat target, true original-object identity is **not currently measurable**. Synthetic target objects/relocations are analysis artifacts; record that distinction. Local exact linkage is the bridge from generated objects to observable target bytes. Preserve both raw and any normalized exploratory diffs; strict acceptance uses no arbitrary address masks. Call/data relocations must resolve to the correct symbol/target and addend. Pin tools, flags, source/header hashes, object hashes and mini-link maps for every accepted result.

Handle these explicitly:

- **Boundaries and pools:** branch delay slots, fall-through/tail calls, compiler inlining, alignment/padding, PC-relative literal reach and shared literal pools. Functions are not simply bytes through the first `rts`; pools are not code. Record noncontiguous owned spans and shared dependencies in an interval manifest before scoring.
- **Relocation/link behavior:** endianness and relocation width/type, call range/relaxation, pool deduplication/reordering, object/TU order, string merging, jump tables, section placement, common/BSS layout and initialization/copy tables. Do not apply MAT3's ELF/PPC relocation masks or five-byte DOL exception policy to SH-4.
- **Static data:** compare emitted constants, strings, tables and padding as well as instructions; keep pool/data coverage separate and assign shared bytes once. GC resource-size agreement does not prove GC/DC payload byte order. BSS has no stored bytes to score; verify address/size/initialization semantics.
- **ABI/FP:** DC argument/result registers, stack alignment, callee saves, narrow promotions, struct layout/bitfields, FP precision/FPSCR modes, rounding/contraction, helper-library calls and aliasing. PPC r2/r13 small-data, paired-single/`__frsqrte` code and MSL expansion choices do not transfer. `BodyVector` arithmetic order must be established on DC, not cosmetically normalized.

## Negative controls and stop rules

Controls should prevent attractive false positives:

1. `TB_Vector3Normalize` uses the GC-specific `msl_math_inline.h` reciprocal-square-root expansion; its semantics are a clue, its PPC inline implementation is not a DC template. `BodyVector` versus transposed `WorldVector` must also fail the wrong matrix-layout fixture.
2. `NGCRumblePad`/GX/Bink backends are known platform controls. A `dc` source filename or similar wrapper name cannot make them a DC exact match. MAT3 `zip.c`/`unzip.c` implement DEFLATE-style paths, while verified DC assets use the separate LZSS-style decoder: never substitute one because the file extension sounds right.
3. In disposable fixtures, change a torque stride/constant, a literal-pool byte, or a relocation target; strict checks must fail. Test a wrong-endian/ABI configuration once to ensure rejection. Distinguish tiny setters/getters by their actual fields/constant values and callers, not epilogue bytes.

Time-box the first execution phase: locate tools and freeze the correspondence/ABI manifest before any wide matching lane. After 16 justified configurations or about two focused hours with no nontrivial exact mini-link, stop flag permutations and revisit compiler family, ABI, function identity, libraries or source drift. For a function, after three evidence-backed source-shape changes or 45 minutes without new strict equal bytes, park it with a residual diagnosis. Do not repeat MAT3's large register-allocation brute-force searches. Reopen only on a new concrete observation. Compiler/call ABI failure is a lane-wide stop; a drifted function is an isolated stop, not permission to weaken the match criterion.

## Staged parallel lanes and one integration owner

These are future ownership assignments, not agents launched now.

| Lane | Exclusive ownership | Deliverable / dependency |
|---|---|---|
| **A: toolchain/ABI** | Compiler inventory, version/flag trials, ABI probes, private calibration outputs | Hashed reproducible environment and proposed configuration; needs identified DC spans |
| **B: correspondence/data** | DC interval/call graph/resource map and drift evidence; no canonical C | Seven candidate mappings plus boundary confidence; starts alongside A |
| **C: semantic C/fixtures** | One assigned TU/function set per worker in private copies; no global header edits | Candidate bodies, minimal field overlay proposals, focused fixtures, mismatch notes; begins after B freezes ownership and A can compile |
| **D: verification/integration** | Accepted manifest, canonical DC headers/build/link config, interval ownership, strict verifier | Reviews A/B/C packages; is the only writer of shared configuration and only runner of integration links |

Stage 0 runs A/B independently using immutable references. Stage 1 freezes toolchain/ABI, seven slots and address ownership. Stage 2 uses C workers on non-overlapping sets (`bsearch/ll`, `drivetra`, `fmath`, `timer/waves`) with small patches and separate output roots. A single worker owns both `interp` and `enginetorque` in `drivetra.c`. Shared headers/static pools belong to D; workers submit proposals instead of editing them. Reserve shared address intervals explicitly. Stage 3 integrates one TU or coherent pool-owning group, reruns its fixtures and local link, then updates metrics. Only once section/TU/global layout is established does D perform full-image verification. No automated watcher or repeated full-game builds during source experiments.

MAT3 stays read-only. Reuse/adapt the *design* of `candidate.py`, `cwmatch.py`, `datacheck.py`, and `verify_link.py` in a new DC tools directory with architecture-specific logic and separate paths; do not call their canonical mutation/configure/mark/build workflows against MAT3. One integrator owns source promotions and records lessons for DC rather than changing MAT3 headers or its progress.

Useful workflow references: [candidate.py](../rush2049-mat3/decomp/tools/candidate.py), [cwmatch.py](../rush2049-mat3/decomp/tools/cwmatch.py), [datacheck.py](../rush2049-mat3/decomp/tools/datacheck.py), [verify_link.py](../rush2049-mat3/decomp/tools/verify_link.py). Their PPC masks and GC five-byte baseline require replacement, not reuse as DC acceptance policy.

## Ranking after the pilot

| Priority | Work | Reason / boundary |
|---|---|---|
| 1 | Resource IDs/state, lists/search, small integer helpers, portable math | Strong 315-slot bridge; compact behavior; good compiler discriminators. GC libc provenance is still checked. |
| 2 | Drivetrain/physics, tracks/paths, cameras, collisions, game state | Rich MAT3 names/types/call graph; expand only after DC field layouts and FP policy are verified. |
| 3 | UI/menu logic, timers, save/game-record policies | Useful semantics but platform wrappers, text/resources, region and retrace differences need separation. |
| 4 | Rendering, audio, input, filesystem/memory-card implementations | PowerVR/Kamui2/AICA/Maple/GDFS differ from GX/PAD/NGC backends; use interface contracts and DC SDK clues, not GC bodies. |
| Infrastructure gate | Startup/linker/memory initialization | Must map early to support exact linking, but do not spend the first gameplay matching effort porting GC startup or SDK libraries. |

## Measurements, checkpoints, and decisions

Keep four counters, not one flattering percentage: `I/7`, `E/7`, exact local-link `Llocal/7`, and strict emitted/linked byte coverage. O raw historical-object equality remains unavailable. Pilot byte denominators are the **frozen DC span union**, with code and owned literals/static data separately itemized; not 1,364 PPC bytes or 715,968 MAT3 bytes. Unmapped slots remain visible. Whole-project function totals are unknown until a DC inventory exists. For the full image, report strict byte coverage against 1,123,492 file bytes and a separately defined gameplay subset; unknown regions remain in the full denominator. Shared pools, inlines and aliases count once. Original-byte/assembly fallback blobs may permit a baseline image but earn zero recovered-C coverage.

Record engineer minutes to identification, E recovery and first strict local link; candidate compiles/configurations; exact new nonoverlapping code/data bytes per hour; held-out success; semantic/ABI failures; mapping reversals; rework after integration; and toolchain setup time separately. Compare two size/complexity-matched pilot tasks with a short (e.g. 20-minute) DC-only identification window before revealing MAT3, or use a separate analyst's DC-only time estimate. Then unblind and record additional time. Treat this tiny comparison as directional evidence, not statistical proof; include amortized setup cost and failed attempts.

- **Checkpoint 0:** target/source/report hashes and the 315-slot bridge frozen; no new match claim.
- **Checkpoint 1:** analyzer/toolchain available, seven DC mappings/boundaries reviewed, ABI probes documented. Unresolved mappings stay pending; no large C port.
- **Checkpoint 2:** frozen configuration tested on held-out cases. Expand only with at least five credible I/E results and at least three nontrivial strict local links spanning pointer/integer and FP behavior, with a held-out arithmetic/global case passing. Tiny `interp`/getter success alone does not qualify.
- **Checkpoint 3:** one coherent TU plus its owned data integrates without regression. Continue only if reuse demonstrably reduces median I-to-E time and improves strict linked bytes per hour (a proposed useful target is roughly 2x the matched-complexity DC-only estimate) without rising mapping reversals or integration failures. If it helps naming but not exact emission, retain it as a semantic reference and fix the compiler/link environment before scaling.

**First actions when execution is requested:** (1) make the period SH-4 compiler/assembler/linker plus analyzer available and hash them; (2) in parallel, annotate the verified resource table and locate the seven pilot candidates from DC callers/constants/layouts; (3) freeze ABI and the four training/three held-out split, then run only that bounded pilot. No need to finish the three MAT3 residual functions first.

**Current blockers/decisions:** SH toolchain/analyzer not located in the checked environment; exact compiler/version/ABI/link behavior unresolved; DC function boundaries/global layout and original object provenance unavailable. The user must choose/provide an existing toolchain location or authorize a later provisioning step before compilation. These do not block the completed plan. Do not guess an exact compiler from banners, claim PPC byte transfer, or count correspondence as matching C.

Plan evidence: [summary](mat3-to-dreamcast-evidence/summary.json), [pilot selections](mat3-to-dreamcast-evidence/pilot-selection.json), [resource agreement](mat3-to-dreamcast-evidence/resource-agreement.json), [tool inventory](mat3-to-dreamcast-evidence/tool-inventory.json), [reference hashes](mat3-to-dreamcast-evidence/reference-file-hashes.json), [evidence manifest](mat3-to-dreamcast-evidence/evidence-manifest.json). No original binaries or source-tree copies are included in this evidence folder.
<!-- END MAT3 TO DREAMCAST PLAN -->
