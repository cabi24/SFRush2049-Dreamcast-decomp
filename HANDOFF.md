# HANDOFF â€” Rush 2049 Dreamcast matching decompilation

Updated 2026-10-18. Read this first, then `BUILDING.md`, `map/README.md`, `include/README.md`,
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
- Map: `map/units.json`, 154 contiguous units, 4,726 functions, 102 of 124 MAT3 GameSource files placed
  (`map/README.md` lists the 22 unplaced and the five weakest boundaries). Ghidra project `ghidra-project/RushDC.gpr`
  carries the same names (`tools/ghidra.ps1`).
- Build: `python tools/dc_split.py && python tools/dc_build.py` (strict, unmasked, at real addresses). Current:

| Unit | Exact text/literals | Initialized data | Exact functions |
|---|---:|---:|---:|
| HAL_Support | 32 | 0 | 1/1 |
| HAL_text | 40 | 15 | 0/26 |
| HAL_time | 234 | 1 | 4/4 |
| Init | 342 | 12 | 3/7 |
| TB_Vector | 132 | 0 | 2/2 |
| TB_text | 492 | 329 | 10/21 |
| asincos | 64 | 16 | 0/3 |
| atan | 286 | 0 | 3/4 |
| audio | 580 | 4,857 | 13/30 |
| audiomgr | 0 | 381 | 0/57 |
| audioscr | 0 | 935 | 0/12 |
| audiosfx | 58 | 0 | 0/30 |
| blit | 1,038 | 4 | 8/10 |
| bstree | 220 | 0 | 6/6 |
| camera | 108 | 1,642 | 1/23 |
| carload | 1,024 | 0 | 1/1 |
| carscr | 0 | 9,760 | 0/22 |
| cheatscr | 1,482 | 52 | 4/5 |
| checkpoint | 118 | 24 | 3/8 |
| circuit | 144 | 376 | 1/5 |
| circuitscr | 0 | 949 | 0/19 |
| collision | 280 | 0 | 2/7 |
| controls | 1,190 | 104 | 5/11 |
| controlscr | 0 | 4,999 | 0/11 |
| controlsys | 438 | 122 | 5/11 |
| countdown | 770 | 69 | 2/2 |
| dc_audio64_init_8C06ADD4 | 1,244 | 317 | 15/16 |
| dc_blit_8C06B2EC | 1,252 | 5 | 10/12 |
| dc_kamui_tex_8C06E5B4 | 0 | 0 | 0/11 |
| dc_misc_8C06B80C | 0 | 24 | 0/13 |
| dc_misc_8C06C08C | 436 | 24 | 4/31 |
| dc_sound_8C069F78 | 0 | 28 | 0/23 |
| dc_unk_8C069E34 | 0 | 0 | 0/1 |
| dc_zoid_8C06ED90 | 108 | 0 | 2/31 |
| demo | 842 | 45 | 7/7 |
| drivetra | 794 | 0 | 6/10 |
| drivsym | 244 | 44 | 1/17 |
| drones | 46 | 0 | 2/4 |
| fmath | 28 | 72 | 1/31 |
| front | 1,880 | 1,102 | 5/5 |
| game | 0 | 163 | 0/14 |
| gameoverscr | 122 | 7 | 1/16 |
| gamerecords | 192 | 0 | 2/25 |
| gfx | 576 | 88 | 8/10 |
| headlights | 308 | 172 | 1/4 |
| highscorescr | 0 | 1,177 | 0/20 |
| hud | 1,264 | 4,855 | 5/33 |
| hudextra | 0 | 1,788 | 0/13 |
| licensescr | 526 | 143 | 2/2 |
| ll | 422 | 0 | 4/4 |
| maxpath | 496 | 57 | 4/25 |
| mb_wad | 182 | 0 | 3/4 |
| mbox | 530 | 523 | 6/13 |
| mdrive | 0 | 378 | 0/20 |
| memsys | 0 | 1 | 0/30 |
| model | 118 | 1 | 2/3 |
| modescr | 250 | 986 | 2/8 |
| options | 756 | 0 | 1/4 |
| optionscr | 166 | 2,092 | 1/11 |
| pakerr | 760 | 52 | 6/19 |
| pakscr | 236 | 43 | 2/17 |
| paksysdc | 0 | 102 | 0/38 |
| pane | 1,082 | 480 | 3/5 |
| password | 884 | 305 | 5/6 |
| pausescr | 6 | 445 | 1/12 |
| playerscr | 0 | 6,038 | 0/30 |
| powerups | 1,580 | 8 | 8/8 |
| rekon | 770 | 0 | 4/4 |
| resurrect | 0 | 328 | 0/19 |
| road | 18 | 0 | 0/3 |
| save | 0 | 35 | 0/62 |
| selectcarload | 244 | 4 | 2/7 |
| shots | 0 | 280 | 0/16 |
| skids | 2,186 | 20 | 6/8 |
| sky | 458 | 7 | 3/3 |
| smoke | 92 | 154 | 2/6 |
| startscr | 0 | 1,270 | 0/9 |
| state | 290 | 12 | 5/6 |
| statll | 130 | 0 | 2/3 |
| stdlib | 64 | 4 | 2/2 |
| streaks | 1,116 | 16 | 4/5 |
| stree | 58 | 72 | 1/25 |
| stunt | 1,016 | 0 | 6/13 |
| targetpaths | 302 | 4 | 2/12 |
| targets | 0 | 525 | 0/32 |
| text | 450 | 7 | 3/3 |
| textureanim | 418 | 0 | 1/2 |
| timer | 0 | 16 | 0/9 |
| titlescr | 264 | 176 | 1/2 |
| tracks | 1,116 | 407 | 3/3 |
| trackscr | 0 | 2,968 | 0/29 |
| unk_8C01021C_video | 78 | 0 | 3/3 |
| unk_8C01026C_entry | 56 | 0 | 1/1 |
| unk_8C0102A4_lz | 0 | 0 | 0/1 |
| unk_8C03A360 | 46 | 5 | 0/2 |
| unk_8C055F38 | 56 | 0 | 0/1 |
| unk_8C065F2C | 258 | 12,223 | 1/3 |
| unzip | 26 | 362 | 1/11 |
| videoscr | 0 | 939 | 0/13 |
| visuals | 44 | 600 | 0/60 |
| warningscr | 0 | 24 | 0/2 |
| waves | 1,850 | 630 | 20/22 |
| weapons | 600 | 1,252 | 3/5 |
| world | 378 | 0 | 5/6 |
| worlddam | 500 | 80 | 2/3 |
| zip | 6,838 | 475 | 19/25 |
| zoid | 440 | 0 | 7/21 |
| zoidblit | 464 | 0 | 7/7 |
| zoidlight | 44 | 0 | 2/2 |
| zoidpoly | 848 | 0 | 7/8 |
| zoidtexture | 280 | 0 | 7/7 |
| zoidwindow | 2,666 | 224 | 15/15 |

Game text/literal credit: **49,730 /569,664 (8.7297%)**, plus **69,331 initialized data bytes**.
Whole-image credit: **119,061 /1,123,492 (10.5974%)**, **328 exact source functions**, 112 source units.
The sixteenth batch adds 428 text/literal bytes, 397
initialized data bytes and 4 exact functions. The audited batch is locally committed
through `b16ca0045d04b385e0fae4a2c8d5c930e791e01c`. No remote push was performed.

Working tree (not yet committed): the dc_audio64_init match below adds five exact functions
and 320 text bytes; `python -B tools/audit_report.py` currently reports 62,498 game text bytes,
131,829 whole-image bytes (11.7339%) and 417 exact functions. It folds in the earlier uncommitted
campaign packets in this tree, so audit before committing as a batch.

`matching-evidence/2026-10-18/integration-sixteenth.json` records current source/header/compiler-input,
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
   record non-obvious codegen lessons in `calib/drivetra/notes.md`-style notes or the calibration report.
4. Commit per unit. Never count masked or fuzzy matches; alignment padding is never counted.

## Rules

- `C:\rush2049-mat3` is read-only reference. Never commit game media, `pilot/orig`, downloaded tools, `build/`, `map/raw/`.
- `tools/shc_cc.py` is native-only. On a policy denial, stop that action and report the exact command/reason. Use normal reviewed approval; never change security settings or use a WSL/wibo fallback.
- From the Claude desktop app, Ghidra must run through `tools/ghidra.ps1` (settings outside AppData).
