# Rush 2049 Dreamcast unit map (`1ST_READ.BIN`)

Unit map ("splits") of the retail v1.001 executable: which translation units occupy which address ranges, in
link order, with every decoded function's extent and, where it can be justified, its MAT3 (GameCube) name.
Generated 2026-10-18. Static analysis only: nothing here was compiled or executed.

| File | Content |
|---|---|
| `units.json` | 154 contiguous, non-overlapping units covering `0x8C010000..0x8C1224A4` (schema from the task: name, source, kind, confidence, text, functions, pools, data, bss, notes) |
| `functions.csv` | 4,726 functions: unit, name, start, end (exclusive, delay slot included), size, mat3_name, confidence, evidence |
| `scripts/` | the pipeline (Python, reads the image and MAT3 read-only); `unit_spec.py` holds the reviewed unit boundaries |
| `raw/` | intermediate data (Ghidra export, function walk, atoms, MAT3 inventory, matcher output, reports) |
| `..\tools\ghidra_scripts\MapExport.java`, `ApplyUnitMap.java` | Ghidra export / label scripts (new files; `DumpRanges.java` untouched) |

The three anchor units come out exactly as given: `timer` starts at 0x8C0100A4 and its last function ends at
0x8C01020E (the unit's text runs on to 0x8C010218 because CountdownTimeOut's literal pool follows); `drivetra`
is 0x8C022CA4..0x8C0232F8 (code ends 0x8C0232E4, pool to 0x8C0232F8); `ll` starts at 0x8C03A1B8, code ends
0x8C03A35E, text ends 0x8C03A360 after the `0000` pad. `build.py` checks all three on every run.
`units.seed.json` (written by the dc_* build tooling, not by this map) agrees: identical function extents and pools
for the three units (here timer.c's last pool also covers the 0x8C01020E alignment pad, as drivetra's pools do in
the seed); its calibrated timer.c D/B symbol layout is copied into `units.json`. `tools/dc_common.load_map` loads
`units.json` without warnings (154 units, 4,726 functions).

## Coverage

| Kind | Units | Bytes | Share |
|---|---:|---:|---:|
| game, with a MAT3 source file | 102 | 529,956 | 47.2% |
| game, DC-only or unidentified file | 17 | 39,708 | 3.5% |
| sdk (Audio64, Shinobi, Kamui2, GDFS, CRI middleware; coarse) | 24 | 318,482 | 28.3% |
| library (Hitachi SHC runtime / C library) | 2 | 12,432 | 1.1% |
| startup (entry stub, section table, SDK startup) | 3 | 872 | 0.1% |
| data (C and D sections, SDK data blocks) | 6 | 222,042 | 19.8% |
| **total** | **154** | **1,123,492** | 100% |

Unit confidence: game units with a MAT3 source 71 high / 25 medium / 6 low; the 17 DC-only game units, 23 of
24 SDK units and one library unit are low.

Functions: 4,726 decoded entries (1,564 in game units, 86 library, 15 startup, 3,061 SDK). 1,183 game functions carry a
MAT3 name: **905 high, 256 medium, 22 low**; 381 game functions and all SDK functions are `func_8C0xxxxx`
(SDK names are not claimed apart from `__divls/__modls/__divws/__modws/bsearch`). Inside game units 499,054 bytes
are instructions, 68,546 literal pools / switch tables, 2,064 pads or unexplained (listed below).

MAT3 GameSource files placed: 102 of 124 (non-NGC). Not placed: asincos, audiodebug (merged into audio.c on DC),
audiosfxps2, audiosongps2, audiosysps2, blit_pc, bonus, boot_pc, bsearch (SDK libc on DC), eggs, fake, fakegdfs,
libgraph, matmemorycard, menuFont, menuFunc, modf, os, stats, tan, texture, timingbar. Some may hide in the
DC-only units (blit_pc in `dc_blit_8C06B2EC`, fake/eggs in `unk_8C0260F0`), unproven.

## Layout found

* **Code region 1, 0x8C010000..0x8C0A77B2** (P section of the game and the first libraries), link order:
  startup stub; `timer.c`; DC glue (`unk_8C010218`); `unzip.c`; then the GameSource files in case-insensitive
  alphabetical order `atan.c .. zoidwindow.c` (front-end screens audioscr, controlscr, gameoverscr,
  highscorescr, optionscr, pausescr, videoscr sit inside this run); a DC platform group (Audio64 wrappers,
  `paksysdc.c`, Kamui texture/renderer back end, GBR-based helpers); a second alphabetical screens group
  `carscr.c .. warningscr.c`; a third alphabetical group of 2049-era gameplay files `hudextra, powerups,
  shots, streaks, weapons, worlddam`; then SHC runtime/libc, Audio64, more runtime.
  Anomalies against strict alphabetical order: `timer.c` and `unzip.c` come first, and `controlsys.c` precedes
  `controlscr.c` (its content is unambiguous; the C-section order agrees, so the DC file name may differ).
* **C section 0x8C0A77B4..0x8C0B8C20**: starts with the startup section table (words 0x8C0B8C20, 0x8C0D2480,
  0x8C1224C0, 0x8C1A9014, ...) and build date strings, then game constants (to 0x8C0B3734), then library constants.
* **D section 0x8C0B8C20..0x8C0D2480**: game initialized data first (timer.c at 0x8C0B8C24, as calibrated;
  `gWaveFilenames` pointer table at 0x8C0C44C0), library data from about 0x8C0D231C.
* **Code region 2, 0x8C0D2480..0x8C1224A4**: SDK startup (entry target), CRI Sofdec/MW/ADX, GDFS, Shinobi,
  Kamui2, with SDK data blocks at 0x8C1151DC..0x8C11A4A0 and 0x8C11BB0E..0x8C121F60 and small code blocks after them.
* **B section 0x8C1224C0..0x8C1A9014** (not file-backed; from the section table and reference ownership).
  Only timer.c's retrace counter is recorded per unit.

Independent check of the unit order: for each game unit, the median of the C/D/B addresses that only that unit
references follows the unit order for 61 of 65 units (C), 76 of 84 (D) and 71 of 83 (B) as a longest increasing
subsequence; the outliers are units whose "exclusive" references are globals defined elsewhere.

## Method

1. **Ghidra export** (`MapExport.java`, read-only): functions, instruction ranges, references, defined data.
2. **Function extents** (`walk.py`, `functions.py`, `postsweep.py`): recursive descent over SH-4 control flow
   from seeds = Ghidra entries, computed-call targets, `bsr` targets, `bra` tail calls whose delay slot pops the
   frame and whose target has a prologue, in-image code pointers that decode cleanly, and gap scanning of
   uncovered stretches (SHC prologue words `mov.l rN,@-r15`, `sts.l pr,@-r15`, `fmov.s frN,@-r15`, `add #-n,r15`).
   Delay slots are included in extents; walks stop at other entries (InitLL falls through into ClearLL);
   `mova`/`mov.w`/`mov.l` PC-relative targets are literal pools; switch tables (`mova; mov.w @(r0,rX); braf`)
   are sized from the `cmp/hs`/`cmp/hi` bound. Overlapping candidates are pruned in favour of the stronger
   evidence (call/bsr > Ghidra/tail-call > code pointer/gap scan; prologue wins ties).
3. **Atoms** (`atoms.py`): consecutive functions that cannot be split between units because one references a
   literal pool placed after/before the other (SHC pools are per translation unit), a `bsr`/tail-call joins
   them, one falls through into the next, or the next function is not 4-aligned (unit sections start
   4-aligned). 890 atoms in the whole image.
4. **MAT3 inventory** (`mat3funcs.py`, `mat3strings.py`): per file, every function in DWARF order (DWARF order
   equals source line order for all 145 CUs, and includes GC-deadstripped functions), GC size, callee names,
   string literals and float literals from the decomp sources.
5. **Global matcher** (`match.py`): anchors seed a BinDiff-style propagation (callee-sequence alignment from
   matched pairs, caller votes, same-file neighbours, rare strings), each candidate scored by strings, float
   constants (DC literals are truncated toward zero, compared with relative tolerance 2e-6), size ratio
   (DC ~0.75 x GC) and call-count agreement; accepted greedily with a uniqueness margin.
6. **Unit assignment**: atoms labelled by majority of matches (`assign.py`), unlabeled stretches ranked against
   every MAT3 file with an order-preserving DP alignment (`segdp.py`, `stretch_report.py`, `tryfile.py`),
   then reviewed by hand into `unit_spec.py` (one line of evidence per unit), checked against the alphabetical
   link order and hard boundaries (a `0000`/`0009` pad between code and the next function's code).
7. **Naming** (`build.py`): within each sourced unit, a DP alignment of DC functions to the MAT3 file's function
   list; then a call-graph cross-check over all names; then output.
8. **Ghidra labels** (`ApplyUnitMap.java`): creates missing functions at mapped entries (clearing analyzer data
   first), renames every mapped function, comments each with unit/confidence/MAT3 name, puts a `unit_<name>`
   label and plate comment on every unit start, and removes stale user-defined functions from earlier runs.
   Result: 4,723 of 4,726 map functions present and named in the project (3 SDK code-pointer entries could not
   be created); one analyzer function (0x8C111B26) is not in the map.

## Evidence rules

* A **unit boundary** is only placed at an atom boundary. A `0000` pad followed directly by code is a hard
  boundary (13 cases in code region 1: 0x8C01B6F0, 0x8C03A360, 0x8C05145C, 0x8C05B56C in the game part and
  nine in the libraries from 0x8C09BC7C on; most units end on a 4-aligned literal pool, so a pad is rare);
  every other boundary is "possible" and was chosen by content (MAT3 file evidence on both sides) and order.
* **Unit confidence**: high = the MAT3 file wins the DP by a wide margin and several functions carry strings /
  float constants / call-graph agreement (or an anchor); medium = clear DP winner plus the alphabetical slot,
  but few distinctive functions; low = slot-only or no MAT3 counterpart (all DC-only units).
* **Function name confidence** (each needs at least two independent pieces of evidence):
  * high: anchor, or global match (call-graph propagation) confirmed by a DP pair score >= 6 (size + strings /
    floats / callee names), or a DP medium upgraded by >= 2 call-graph confirmations not used to make it;
  * medium: DP score >= 3, or a global match with a weaker DP score, or a low name with >= 1 call-graph confirmation;
  * low: position in the file order (DP pair, or a run of <= 3 unmatched pairs bounded by accepted pairs) plus a
    plausible size ratio (0.45..2.2 of 0.75 x GC size), no call-graph contradiction checked beyond the count.
  The `evidence` column records which: `global-match(...)`, `dp=`, `order-run`, `callgraph=+confirm/-contradict`,
  and how the entry point was found (`entry=ghidra/call/bsr/tailbra/codeptr/gapscan/prologue-sweep/anchor`).
* Unnamed functions are `func_<addr>`. A missing name means "no counterpart justified", not "absent from MAT3".

## Uncertain boundaries and guesses

* **DC platform group 0x8C069E34..0x8C072820** (~35 KB, 10 units `dc_*`): split at atom boundaries where the role
  changes (sound layer, Audio64 init, blit, loaders, title/VMU helpers, `paksysdc.c`, Kamui textures, ZOID back
  end, GBR helpers). File count and names are guesses; only `paksysdc.c` is a MAT3 file.
* **`startup` 0x8C010000..0x8C0100A4**: the stub, the "Lib Handle" block and two small functions (one is the
  interrupt-mask helper used by InitializeTimer) may be more than one object.
* **`unk_8C010218` 0x8C010218..0x8C010320**: five atoms (stub, VideoSetOffset-like Kamui call, the main-like
  function called by the SDK startup, an LZS-style decoder) - one to five DC files.
* **Single-atom slot units** with weak content evidence: `HAL_Support` (1 fn), `HAL_time`, `mb_wad`, `stdlib`
  (two functions; probably a random-number file), `text`, `zoidlight`, `bstree`, `circuit`, `warningscr`,
  `cheatscr` (DP margin 27.3 vs 23.9 for pausescr.c).
* **Attachment of small edge atoms to a neighbour**: gameoverscr.c's last two functions (0x8C02B114, 0x8C02B174),
  zip.c's 4-byte 0x8C0686F0, the three DC-only functions `unk_8C065F2C` (kept separate from world.c/zip.c; the D
  section shows they own data between waves.c and zip.c), `unk_8C055F38`, `unk_8C0951C0`, `unk_8C03A360`,
  `unk_8C0260F0` (empty stubs + one function; eggs.c/fake.c slot), `unk_8C07D90C` (boot/pak-check screen; the
  matcher's updateTitleScreen proposal was rejected because titlescr.c's body is at 0x8C08CF50), `unk_8C0898D8`.
* **Library/SDK partition**: `shc_runtime_libc_1/2` and `audio64` boundaries (0x8C09C06C, 0x8C0A55AC) are atom
  boundaries chosen by string families; region 2 SDK units are runs of atoms tagged by their first diagnostic
  string (names such as `sdk_sdk_fatal_8C0E9FC0` describe only the first tagged atom); individual library objects
  are not resolved.
* **Data**: C/D game-vs-library splits are at the first library-referenced word (could be early by the size of the
  last game object); no per-unit data split (only timer.c's calibrated D range is listed).
* **Function discovery**: 2,064 bytes inside game units are pads or unexplained: the "Lib Handle" text, a 192-byte
  zero block at 0x8C071B5C, two 32-byte FPU-constant blocks at 0x8C072628/0x8C072664 and an absolute
  jump table at 0x8C071F78 (all in `dc_*`), small stretches in audioscr (0x8C017778..0x8C0177D2), hud
  (0x8C03613C..0x8C036174), pausescr (0x8C0475EE..0x8C0476CC), carscr (0x8C072ADE..0x8C072B2C), playerscr
  (0x8C07FC06), libc_1 (0x8C09BA28..0x8C09BAB4) - mostly unreached code (dead branches or code only reached
  through computed jumps) or pool data whose referencing load was not decoded.

## Reproduce

```
cd C:\rush2049-dreamcast\map\scripts
# Ghidra export (read-only), ~1 min. NOTE: raw/ghidra_*.tsv in this folder were exported BEFORE the labels
# were applied; re-exporting now would seed discovery with the map's own functions.
..\..\tools\ghidra.ps1 -Headless C:\rush2049-dreamcast\ghidra-project RushDC -process 1ST_READ.BIN -noanalysis -readOnly `
   -scriptPath C:\rush2049-dreamcast\tools\ghidra_scripts -postScript MapExport.java C:\rush2049-dreamcast\map\raw
python functions.py; python postsweep.py; python dcstrings.py; python dcfeatures.py; python atoms.py
python mat3strings.py; python mat3funcs.py; python match.py; python build.py
# inspection helpers: show.py LO HI | tryfile.py LO HI [file.c ...] [-v] | stretch_report.py | assign.py
# labels (writes the Ghidra project):
..\..\tools\ghidra.ps1 -Headless C:\rush2049-dreamcast\ghidra-project RushDC -process 1ST_READ.BIN -noanalysis `
   -scriptPath C:\rush2049-dreamcast\tools\ghidra_scripts -postScript ApplyUnitMap.java `
   C:\rush2049-dreamcast\map\functions.csv C:\rush2049-dreamcast\map\raw\units.tsv
```

Python must run without `-I` (capstone is in the user site). `build.py` also writes `raw/units.tsv` (start, end,
name, source, kind, confidence) for `ApplyUnitMap.java`, asserts contiguity and full coverage, and re-checks the
three anchor units. `functions.py` takes about 2 minutes (iterative discovery, 30 rounds).
