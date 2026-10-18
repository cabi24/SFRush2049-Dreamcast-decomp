<!-- BEGIN RUSH2049 RETAIL OVERVIEW: static inspection, 2026-10-18 -->
# Rush 2049 Dreamcast retail v1.001 overview

**Retail's gameplay executable is byte-for-byte identical to the August 1 prototype.** A direct comparison of all 1,123,492 bytes found zero differences, and both SHA-256 hashes are:

`336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`

This makes the August 1 executable a verified reference for this target. It does not make the entire discs identical: retail changes boot/volume metadata and layout, omits two unused sound banks, and differs in a small portion of one audio stream.

## Target and immutable inputs

Inspected exact directory: `San Francisco RUSH 2049 v1.001`. Original files: [GDI](San%20Francisco%20RUSH%202049%20v1.001/San%20Francisco%20RUSH%202049%20v1.001%20%282000%29%28Midway%29%28NTSC%29%28US%29%5B%21%5D.gdi), [track01](San%20Francisco%20RUSH%202049%20v1.001/track01.bin), [track02](San%20Francisco%20RUSH%202049%20v1.001/track02.raw), [track03](San%20Francisco%20RUSH%202049%20v1.001/track03.bin), [track04](San%20Francisco%20RUSH%202049%20v1.001/track04.raw), [track05](San%20Francisco%20RUSH%202049%20v1.001/track05.bin).

All original inputs were read-only. Extracted executable/IP and decoded inspection samples remain in separate task scratch; the adjacent evidence folder contains JSON and small Python helpers, no game binaries. No game execution, install, matching wave, source publication, or unrelated repository change occurred. The earlier [betafindings.md](betafindings.md) remains the historical prototype catalogue; this overview adds the retail inspection.

| Identity | Observed value |
|---|---|
| IP product / version / title | `T-9707N / V1.001 / RUSH 2049` |
| IP date / region / peripherals | `20000802 / U / 0799A10` |
| IP maker / company / media | `SEGA ENTERPRISES / MIDWAY / AE19 GD-ROM1/1` |
| Boot executable | `1ST_READ.BIN`, 1,123,492 bytes (`0x1124A4`), LBA 548451 |
| Executable ISO9660 date | `2000-08-01 17:08:20 -08:00` |
| Volume / high-density PVD date | `SFR20491 / 2000073117165700à` (July 31, 17:16:57, -08:00) |
| IP.BIN SHA-256 (32,768 reconstructed bytes) | `8d59535d398ac8b5e922046cffae3fca9b35b88d5f594fcba952e0e94c9b21bc` |

The low-density PVD is one second earlier. IP and filesystem dates are recorded metadata, not independently verified compiler times. Host file dates are 1996-12-25 placeholders and are not build evidence.

## Disc and asset inventory

Five-track GDI GD-ROM dump; data tracks are 2352-byte Mode 1 sectors, with 2048-byte payloads. Both ISO9660 sessions were read. All five tracks were hashed (1,187,720,016 raw bytes).

| Track | Start LBA | Kind | Raw bytes |
|---|---:|---|---:|
| 1 | 0 | Mode 1 data | 1,427,664 |
| 2 | 757 | Audio | 1,237,152 |
| 3 | 45000 | Mode 1 data | 30,185,568 |
| 4 | 57984 | Audio | 1,060,752 |
| 5 | 58585 | Mode 1 data | 1,153,808,880 |

There are **4 low-density and 789 high-density files** (793 total; 1,005,277,275 allocated bytes). Of the high-density files, 350 are in the root and 439 belong to the browser tree, including duplicated templates.

The root contains 314 `.LZS`, 18 audio `.STR`, 7 `.KAT` sound banks, 2 `.SFD` movie streams, 2 `.ROM` resources, 4 `.BIN`, one `.DRV`, one `.PVR`, and one warning-audio `.DA`. `.ROM` here is an asset extension, not a cartridge image. `2_DP.BIN` is PlanetWeb; `AUDIO64.DRV` starts with ARM instructions and belongs to the audio subsystem.

All **314 LZS assets decoded successfully** to 56,650,694 bytes, and all compressed and decoded hashes agree with August 1. The validated decoder uses LSB-first flags: 1 = literal; 0 = two-byte backreference, length `(a & 15) + 2`, distance `((a & 240) << 4) | b`; a zero-distance token terminates the stream. [Decoder](retail-v1.001-evidence/lzs.py) and decoded hashes in [scan.json](retail-v1.001-evidence/scan.json).

## Comparison with the four prototypes

Counts use `(ISO session, file path)` as the key and compare complete file SHA-256 values; they exclude directories and disc-layout sectors.

| Prototype | Shared files | Identical hashes | Changed shared files | Prototype-only / retail-only | Main executable |
|---|---:|---:|---:|---:|---|
| February 17 | 10 | 0 | 10 | 173 / 783 | Different |
| April 10 | 175 | 90 | 85 | 225 / 618 | Different |
| August 1 | 793 | 792 | 1 | 2 / 0 | Identical |
| August 7 | 129 | 121 | 8 | 0 / 664 | Different |

Against August 1, retail adds no files and omits `UNUSED1.KAT` (1,065,344 bytes) and `UNUSED2.KAT` (1,121,408 bytes). The only changed shared payload is `STARSKY.STR` (62,588,928 bytes): 846 byte differences within offsets 15,370,367–15,371,216. The reason is not established; this alone does not distinguish intentional audio editing from mastering/dump differences. [Exact comparison](retail-v1.001-evidence/prototype_comparison.json); [audio difference ranges](retail-v1.001-evidence/starsky_difference.json).

## Architecture, load address, and initial anchors

Use **raw, little-endian SH-4, 32-bit**, nominal load base **`0x8C010000`**, file-backed range **`0x8C010000`–`0x8C1224A4` exclusive**. Evidence includes the Dreamcast GDI/IP boot format, readable SH-4 startup instructions and little-endian in-range pointer tables. At file `0x1A`, instruction `D004` loads the literal at `0x2C`; `402B` at `0x1C` jumps through R0 to **`0x8C0D2480`** (file `0xC2480`). The standard load convention is documented in [IP.BIN and 1ST_READ.BIN](https://mc.pp.se/dc/ip.bin.html).

| Anchor | File offset | Candidate loaded address | Interpretation |
|---|---|---|---|
| Initial stub | `0x0` | `0x8C010000` | Code plus nearby literal pool |
| Startup jump target | `0xC2480` | `0x8C0D2480` | First concrete runtime code target |
| `texte.lzs` string | `0xA09B4` | `0x8C0B09B4` | Resource-loading cross-reference seed |
| `debugfonttex.lzs` string | `0xA0A18` | `0x8C0B0A18` | Font/resource cross-reference seed |
| Filename pointer table | `0xB44C0` | `0x8C0C44C0` | 315 unique entries; all resolve to disc files |
| Table end / zero word | `0xB49AC` | `0x8C0C49AC` | Observed table terminator |
| `kmBeginScene` literal | `0x9AA48` | `0x8C0AAA48` | Rendering-library diagnostic, not function entry |
| `a64FileRead - gdFsRead failed` | `0xA3890` | `0x8C0B3890` | File/audio SDK diagnostic |

Organization is a statically linked raw image with mixed code, literal pools, resource strings/pointers, and SDK/CRI data. `Lib Handle Start/End` metadata appears beside the entry stub; resource/audio strings cluster around `0x94F80`–`0xA8400`, with SDK banners near `0x1052D9` onward. These are anchors, **not formal `.text`/`.rodata`/`.data` section boundaries**. BSS, stack, heap, main entry, function extents, and runtime copy/overlay destinations remain to be traced. Do not linear-disassemble pointer/string regions. [Anchor map](retail-v1.001-evidence/layout_anchors.json); [all filename pointers](retail-v1.001-evidence/resource_filename_table.json).

## Debug information and toolchain clues

No valid ELF/COFF objects, symbol-table entries, debug type records, source-line records, maps, or object companions were recovered; no DWARF/STABS/PDB markers survived validation. Full allocated-file and raw-track sweeps plus decoded-LZS scans found one invalid ELF-looking byte sequence in `WINGEY.STR`, also present in August 1. These are bounded static checks, not exhaustive recovery of arbitrary undocumented/deleted data.

The gameplay binary retains named SDK diagnostics but no readable early `kWave...` name table. Its identical PlanetWeb binary contains 13 source-filename literals and the assertion format `assertion %s failed in %s, at line %ld!` at browser offset `0x107CA8`. These belong to browser/support code, not recovered Rush gameplay source. [Browser evidence](retail-v1.001-evidence/browser_literals.json); [diagnostic candidates](retail-v1.001-evidence/diagnostics.json).

Runtime fingerprints are Shinobi **1.68**, Audio64 **10.0.f.2**, Kamui2 **16,3,4,0**, GDFS **1.09**, and syStart **1.17**; exact strings/offsets are in [summary.json](retail-v1.001-evidence/summary.json). These identify linked libraries, not the compiler. Compiler vendor/version, optimization flags, linker details, and a matching toolchain remain unestablished. No SH disassembler/compiler was located on the checked PATH; this pass used existing Python and did not install anything.

`DEBUG`, `PATH EDITOR`, and `FRAME SCALE` remain in decoded `TEXTE.LZS` at `0x22E7`, `0x22ED`, and `0x2D59`. There is no retail `PATHEDIT.*` file. Menu reachability and editor functionality are untested.

## What the earlier work can contribute

April's 160 and February's 155 verified resource-label pairs provide semantic naming seeds. Filename-stem comparison finds **142 April and 112 February candidates** in the retail table. These are hypotheses about labels, not recovered enum declarations or proven structure types. Indices changed: April `kWaveDebugFontTex` is index 6; retail `debugfonttex.lzs` is index 8. [April crosswalk candidates](retail-v1.001-evidence/Apr_10_2000_prototype_resource_candidates.json); [February candidates](retail-v1.001-evidence/Feb_17_2000_Dreamcast_Prototype_resource_candidates.json).

Existing N64/MAT3 knowledge can help name gameplay systems, resource semantics, vehicle/track/path records, constants, and algorithms. Reuse existing format notes/parsers after checking field order, widths, units, alignment, and endianness against these bytes. N64 MIPS and GameCube PowerPC code, ABIs, compiler behavior, and platform libraries differ from Dreamcast SH-4; machine-code matches, addresses, and C structure layouts cannot be assumed. No unrelated N64/MAT3 repository was inspected or modified for this pass.

## Small ordered setup plan

1. **Pin this exact target.** Keep the original GDI/tracks read-only; use the recorded hashes and a separate extraction/project directory. [verify_retail.py](retail-v1.001-evidence/verify_retail.py) can verify/extract the main executable and boot area without running them.
2. **Create the SH-4 analysis project.** Import the pinned executable as raw LE SH-4 at `0x8C010000`; label the entry stub, startup target, and filename table above. Trace startup to establish BSS/global initialization, stack/heap setup, and the game entry before applying broad function labels.
3. **Start with the resource pipeline.** Follow references to the 315-entry table and `texte.lzs`/`debugfonttex.lzs` to locate loading/decompression/dispatch code. Use the validated LZS decoder and tiny text/font assets as the first format fixtures. Transfer April names as checked candidates with per-build indices.
4. **Partition SDK and gameplay evidence.** Use known diagnostics and library fingerprints to isolate audio/GDFS/rendering code; keep PlanetWeb separate. Then choose one small gameplay routine linked to a named asset as the first decompilation target, after its boundaries/callers are verified. Identify the original compiler from code-generation evidence before planning matching builds.

**Recommended next action:** import the pinned retail executable into a separate SH-4 analysis project and map the startup target plus filename table. No matching wave is necessary to establish this foundation.

## Portable reports and reproducibility

From this directory, `python .\retail-v1.001-evidence\verify_retail.py` verifies the executable/IP hashes and 315 pointers read-only. To extract into separate scratch, add `--out "$env:TEMP\rush2049-retail-v1001"`. The helper only selects the exact catalogued retail directory. Its syntax was checked; it has not been run as a separate project/setup action.

Additional reports: [inventory with dates/LBAs](retail-v1.001-evidence/inventory.json), [file and track hashes / decompression results](retail-v1.001-evidence/scan.json), [track layout](retail-v1.001-evidence/tracks.json), [evidence SHA-256 manifest](retail-v1.001-evidence/evidence-manifest.json). Embedded original absolute paths are provenance; document links and the verification helper are relative to this root.
<!-- END RUSH2049 RETAIL OVERVIEW -->
