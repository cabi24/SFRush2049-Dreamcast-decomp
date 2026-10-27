# HANDOFF â€” Rush 2049 Dreamcast matching decompilation

## Verified InitWaves match — 2026-10-27

InitWaves now adds **460 emitted code bytes and one exact function**. Verified totals:
**79,014 / 569,664 game code/literal bytes (13.8703%)**,
**148,345 / 1,123,492 image bytes (13.2039%)**, **519 / 1,555 game functions**.
All earlier accepted spans are preserved. Independent replay, a deliberately incorrect
control, and a full 114-unit integration build passed strict target comparison.

This verification used the explicit pinned Linux/wibo toolchain under separate review,
not the native-only wrappers at this commit. The new evidence is
`evidence/linux-round1-integration-20261027.json`; its tool hashes describe that
verification environment. `evidence/strict-integration-receipt.json` remains the
historical native baseline and predates this source change. No claim of full raw
Windows/Linux artifact equivalence or whole-game execution is made.
See `docs/initwaves-match-20261027.md` for scope and reproduction requirements.


Updated 2026-10-26. Read this first, then `BUILDING.md`, `map/README.md`, `include/README.md`,
`pilot/lanes/toolchain/calibration.md` (compiler facts) and `tools/README.md` (tool provenance).
`CODEX.md`, `overview.md` and `mat3-to-dreamcast-plan.md` are the earlier planning record.

## State

- Target: US v1.001 `1ST_READ.BIN` (SHA-256 `336c97aaâ€¦`), raw little-endian SH-4 at `0x8C010000`,
  1,123,492 bytes; local copy `pilot/orig/1ST_READ.BIN` (git-ignored; `retail-v1.001-evidence/verify_retail.py` re-extracts it).
- Compiler: Hitachi SHC 5.x (5.0r32â€“5.1r13 byte-identical on everything tested). **Frozen flags**
  `-comment=nonest -cpu=sh4 -division=cpu -fpu=single -endian=little -macsave=0 -sjis -string=const -speed -noinline`
  (`-inline=16` is an equivalent alternative so far). Never add decomp.me's `-aggressive=2 -extra=asm=1800 -pic=0`.
  Linker: H-series `LNK.EXE` 6.0E. Details and all compiler quirks in `pilot/lanes/toolchain/calibration.md`
  and `calib/drivetra/notes.md` (float literals truncate; `short / short-typed` â†’ `__divws`; products written left to right; `-speed` loop shape; per-block address loads).
- Map: `map/units.json`, 156 contiguous units, 4,726 functions, 102 of 124 MAT3 GameSource files placed
  (`map/README.md` lists the 22 unplaced and the five weakest boundaries). Ghidra project `ghidra-project/RushDC.gpr`
  carries the same names (`tools/ghidra.ps1`).
- Build: `python tools/dc_split.py && python tools/dc_build.py` (strict, unmasked, at real addresses). Current:

| Unit | Exact text/literals | Initialized data | Exact functions |
|---|---:|---:|---:|
| asincos | 64 | 16 | 0/3 |
| atan | 368 | 0 | 4/4 |
| audio | 580 | 4,857 | 13/30 |
| audiomgr | 0 | 381 | 0/57 |
| audioscr | 0 | 935 | 0/12 |
| audiosfx | 590 | 0 | 8/30 |
| blit | 1,322 | 4 | 10/10 |
| bstree | 220 | 0 | 6/6 |
| camera | 108 | 1,642 | 1/23 |
| carload | 1,024 | 0 | 1/1 |
| carscr | 0 | 9,760 | 0/22 |
| cheatscr | 1,482 | 52 | 4/5 |
| checkpoint | 118 | 24 | 3/8 |
| circuit | 144 | 376 | 1/5 |
| circuitscr | 0 | 949 | 0/19 |
| collision | 280 | 0 | 2/7 |
| controls | 1,742 | 104 | 7/11 |
| controlscr | 0 | 4,999 | 0/11 |
| controlsys | 438 | 122 | 5/11 |
| countdown | 770 | 69 | 2/2 |
| dc_audio64_init_8C06ADD4 | 1,244 | 317 | 15/16 |
| dc_blit_8C06B2EC | 1,308 | 5 | 12/12 |
| dc_kamui_tex_8C06E5B4 | 0 | 0 | 0/11 |
| dc_misc_8C06B80C | 0 | 24 | 0/13 |
| dc_misc_8C06C08C | 1,302 | 0 | 12/14 |
| dc_periph_8C06C664 | 948 | 24 | 13/14 |
| dc_sound_8C069F78 | 0 | 28 | 0/23 |
| dc_status_8C06CA98 | 0 | 0 | 0/3 |
| dc_unk_8C069E34 | 88 | 0 | 0/1 |
| dc_zoid_8C06ED90 | 108 | 0 | 2/31 |
| demo | 842 | 45 | 7/7 |
| drivetra | 794 | 0 | 6/10 |
| drivsym | 244 | 44 | 1/17 |
| drones | 46 | 0 | 2/4 |
| fmath | 2,508 | 72 | 25/31 |
| front | 1,880 | 1,102 | 5/5 |
| game | 528 | 163 | 4/14 |
| gameoverscr | 122 | 7 | 1/16 |
| gamerecords | 3,704 | 0 | 16/25 |
| gfx | 576 | 88 | 8/10 |
| HAL_Support | 32 | 0 | 1/1 |
| HAL_text | 208 | 15 | 1/26 |
| HAL_time | 234 | 1 | 4/4 |
| headlights | 308 | 172 | 1/4 |
| highscorescr | 0 | 1,177 | 0/20 |
| hud | 1,264 | 4,855 | 5/33 |
| hudextra | 0 | 1,788 | 0/13 |
| Init | 712 | 12 | 5/7 |
| licensescr | 526 | 143 | 2/2 |
| ll | 422 | 0 | 4/4 |
| maxpath | 496 | 57 | 4/25 |
| mb_wad | 252 | 0 | 4/4 |
| mbox | 956 | 523 | 8/13 |
| mdrive | 0 | 378 | 0/20 |
| memsys | 0 | 1 | 0/30 |
| model | 1,520 | 1 | 3/3 |
| modescr | 250 | 986 | 2/8 |
| options | 1,136 | 0 | 3/4 |
| optionscr | 1,118 | 2,092 | 3/11 |
| pakerr | 2,904 | 52 | 16/19 |
| pakscr | 236 | 43 | 2/17 |
| paksysdc | 0 | 102 | 0/38 |
| pane | 1,320 | 480 | 5/5 |
| password | 884 | 305 | 5/6 |
| pausescr | 6 | 445 | 1/12 |
| playerscr | 0 | 6,038 | 0/30 |
| powerups | 1,580 | 8 | 8/8 |
| rekon | 770 | 0 | 4/4 |
| resurrect | 0 | 328 | 0/19 |
| road | 18 | 0 | 0/3 |
| save | 8,250 | 35 | 45/62 |
| selectcarload | 244 | 4 | 2/7 |
| shots | 0 | 280 | 0/16 |
| skids | 2,186 | 20 | 6/8 |
| sky | 458 | 7 | 3/3 |
| smoke | 244 | 154 | 3/6 |
| startscr | 0 | 1,270 | 0/9 |
| state | 290 | 12 | 5/6 |
| statll | 130 | 0 | 2/3 |
| stdlib | 64 | 4 | 2/2 |
| streaks | 1,168 | 16 | 4/5 |
| stree | 616 | 72 | 5/25 |
| stunt | 1,168 | 0 | 7/13 |
| targetpaths | 302 | 4 | 2/12 |
| targets | 0 | 525 | 0/32 |
| TB_text | 1,322 | 329 | 18/21 |
| TB_Vector | 132 | 0 | 2/2 |
| text | 450 | 7 | 3/3 |
| textureanim | 418 | 0 | 1/2 |
| timer | 0 | 16 | 0/9 |
| titlescr | 264 | 176 | 1/2 |
| tracks | 1,116 | 407 | 3/3 |
| trackscr | 0 | 2,968 | 0/29 |
| unk_8C01021C_video | 78 | 0 | 3/3 |
| unk_8C01026C_entry | 56 | 0 | 1/1 |
| unk_8C0102A4_lz | 0 | 0 | 0/1 |
| unk_8C03A360 | 110 | 5 | 1/2 |
| unk_8C055F38 | 56 | 0 | 0/1 |
| unk_8C065F2C | 604 | 12,223 | 2/3 |
| unzip | 26 | 362 | 1/11 |
| videoscr | 0 | 939 | 0/13 |
| visuals | 326 | 600 | 1/60 |
| warningscr | 0 | 24 | 0/2 |
| waves | 1,850 | 630 | 20/22 |
| weapons | 600 | 1,252 | 3/5 |
| world | 378 | 0 | 5/6 |
| worlddam | 500 | 80 | 2/3 |
| zip | 6,838 | 475 | 19/25 |
| zoid | 1,316 | 0 | 20/21 |
| zoidblit | 464 | 0 | 7/7 |
| zoidlight | 44 | 0 | 2/2 |
| zoidpoly | 896 | 0 | 8/8 |
| zoidtexture | 280 | 0 | 7/7 |
| zoidwindow | 2,666 | 224 | 15/15 |

Game text/literal credit: **78,554 /569,664 (13.7895%)**, plus **69,331 initialized data bytes**.
Whole-image credit: **147,885 /1,123,492 (13.1630%)**, **518 exact source functions**, 114 source units.
The 2026-10-20 round adds 11,762 text/literal bytes and 59 exact functions over `cac721e`: MAT3
bodies ported in map order took save from 0 to 45 of 62 exact functions and gamerecords from 2 to 16.
Shots, the third unit of that round, is not started. The loop is faster: persistent compiler slots
cut one compile from about 2 s to 0.8 s and a full strict build plus audit to under a minute, and
`tools/fncmp.py` gives register-accurate side-by-side listings. Each unit is committed separately
with its exact byte and function delta; `calib/lessons.md` records codegen lessons and every parked
function's first differing instruction. Published at
https://github.com/cabi24/SFRush2049-Dreamcast-decomp (branch `master`).


`evidence/strict-integration-receipt.json` (regenerated by `python -B tools/audit_report.py`) records current source/header/compiler-input,
tool/output hashes and disjoint earned address spans. `python -B tools/audit_report.py` validates
saved outputs without compiling. No linker fill, gaps, alignment, explicit record padding or BSS
earns image credit. Full fixed-address equality remains required; no comparison masks are used.
Compiler, linker and flags remain frozen. Only units whose compiler input changes need a fresh
native link; byte-identical inputs may retain the already-verified native artifacts.

The audio/stree generated epilogues are part of their source functions. Current total/game
function denominators are 4,717/1,555. The two original gameover tail atoms and their
external pool belong to gamerecords; identities stay conservative pending mapping review.
Asincos selected03 retains16 C bytes and64 independently bounded external literal bytes;
no fourth variant was run. The original `_func_8C05145C` alias of InitSTree remains for world.
Four uncertain front zdepth high-halfword bytes remain conservatively excluded from credit.

Target-backed camera, full surface and audio fields preserve model0x808/car0x3CC strides.
Player profiles use the proved0x40 stride and data handle at+0x3C for camera and circuit.
The unsupported+0x48 profile field was removed. CarParts uses the proved0x60 stride.
The external SetNewPathIndex entry is split from SetDroneResPathIndex with unchanged byte union;
that split adds one source function and earns no extra bytes. Videoscr removes one unproven
empty stub from the heuristic denominator; its four RTS/NOP bytes remain an unsourced gap.
World and circuit retain their original function-address aliases after semantic map renames.
Carscr merges seven internal fragments into its real updateCarScreen stack frame (-7 heuristic
functions); playerscr owns its proved two-function button prefix from recordscr, with count unchanged.
ZIP uses the normal allocator at0x8C040256; the distinct high-end allocator is0x8C040380.
Queue SDK status tests have no failure action and are omitted; their earlier private88 pool bytes
are uncredited. Three uncertain primitive-color RGB bytes also remain uncredited.
HUDextra owns the callback-proven trailing animation entry0x8C0951C0; all thirteen original
function identities and the complete byte union are retained. Startup uses three pool-closed
C compilation groups, preserving all six original identities; the unproved empty218 entry
remains source-null and uncredited. Grouping names and semantic roles are reconstructions,
not recovered debug symbols. HUD's private blit pointer is qualified without changing shared headers.
HAL text uses target-backed externs for23 real globals and allocates no fabricated BSS prefix.
Twenty-seven empty diagnostic conditions were removed from HAL text and audio manager;
the two actual audio validation helpers remain external, without empty replacement bodies.
The unproved update_reverb atom is an unsourced four-byte gap and is removed from the heuristic
function denominator, earning no bytes. Original anonymous identities retain explicit linker aliases
for reconstructed semantic names. Zoid supplies seven baseline C bodies and leaves24 targets
unsourced; its admitted native output was reused without a new Zoid compiler launch.
Private work uses disjoint unit ownership; later private packets remain excluded until audited
integration. MAT3 stays read-only. Validation uses separate fixed-address translation-unit links;
no full game image build or game execution was performed.

## Resume

```
cd C:\rush2049-dreamcast
python tools\dc_split.py
python tools\dc_build.py            # or: python tools\dc_build.py --unit NAME -v
tools\objdiff\objdiff.exe           # open C:\rush2049-dreamcast\objdiff.json
```
Headless disassembly of any range: `.\tools\ghidra.ps1 -Headless ghidra-project RushDC -process 1ST_READ.BIN -noanalysis -scriptPath tools\ghidra_scripts -postScript DumpRanges.java out.txt 8c0xxxxx-8c0yyyyy`.

## Per-unit workflow

1. Pick a unit from `map/units.json` (kind `game`, with a MAT3 source); read its functions in Ghidra
   or a DumpRanges listing and the MAT3 source in `C:\rush2049-mat3\decomp\src\GameSource\src`.
2. Write `src/<unit>.c` using `include/dc_types.h` (absolute include path; extend the header with
   evidence comments as offsets are proven) and MAT3 bodies; add externals to `src/externs.txt`.
3. `python tools\dc_build.py --unit <unit> -v`, iterate on source shape only (flags are frozen);
   `python -B tools\fncmp.py <unit> <function>` shows the linked and target code side by side,
   register-accurate, with the literal each PC-relative load reads and the first differing word
   marked. Record non-obvious codegen lessons in `calib/drivetra/notes.md`-style notes or the
   calibration report. Several workers may run `--unit` builds at once (different units);
   `build/report.json` is merged under a lock.
4. Commit per unit. Never count masked or fuzzy matches; alignment padding is never counted.

## Rules

- `C:\rush2049-mat3` is read-only reference. Never commit game media, `pilot/orig`, downloaded tools, `build/`, `map/raw/`.
- `tools/shc_cc.py` is native-only. On a policy denial, stop that action and report the exact command/reason. Use normal reviewed approval; never change security settings or use a WSL/wibo fallback.
- From the Claude desktop app, Ghidra must run through `tools/ghidra.ps1` (settings outside AppData).
