<!-- BEGIN RUSH2049 PROTOTYPE FINDINGS: generated from static inspection, 2026-10-18 -->
# Rush 2049 Dreamcast prototype findings

Inspection date: 2026-10-18. Root: `C:\rush2049-dreamcast`.

**The February and April builds contain useful resource-name tables and gameplay diagnostics. None of the four inspected builds yielded recoverable source-level debug metadata.** Start with April for readable resource labels, retain February as an earlier reference, and use August 1 as the broad later-build comparison.

**Retail status:** The main/retail Dreamcast game has not been inspected. No newly arriving retail files were processed for this document. This is an inspection catalogue, not the start of a decompilation.

## Scope and preservation

All four build directories were inspected read-only. Coverage included the low- and high-density ISO9660 filesystems in each GDI, 1,507 listed files, hashes and debug-marker sweeps of all 20 raw tracks (4,763,632,608 bytes), and successful decoding of all 425 `.LZS` files in the August builds. The four outer `.7z` archives were listed; every extracted track CRC matched its archive member.

Original builds and archives were preserved. No game executable was run, no software was installed, and no binary was uploaded. Executable/IP extraction was confined to a separate task scratch directory. The adjacent evidence folder contains generated JSON/CSV/text and a small inspection helper, **no raw tracks or game binaries**.

## Exact build directories and original files

| Build | Exact directory | Original file links |
|---|---|---|
| February 17 | `San Francisco Rush 2049 (Feb 17, 2000 Dreamcast Prototype)` | [GDI](San%20Francisco%20Rush%202049%20%28Feb%2017%2C%202000%20Dreamcast%20Prototype%29/RUSH%202049.GDI), [track03 / boot area](San%20Francisco%20Rush%202049%20%28Feb%2017%2C%202000%20Dreamcast%20Prototype%29/track03.bin), [track05 / game executable extent](San%20Francisco%20Rush%202049%20%28Feb%2017%2C%202000%20Dreamcast%20Prototype%29/track05.bin), [archive](San%20Francisco%20Rush%202049%20%28Feb%2017%2C%202000%20Dreamcast%20Prototype%29.7z), [dump metadata](San%20Francisco%20Rush%202049%20%28Feb%2017%2C%202000%20Dreamcast%20Prototype%29/Disc%20Information.txt) |
| April 10 | `San Francisco Rush 2049 (Apr 10, 2000 prototype)` | [GDI](San%20Francisco%20Rush%202049%20%28Apr%2010%2C%202000%20prototype%29/disc.gdi), [track03 / boot area](San%20Francisco%20Rush%202049%20%28Apr%2010%2C%202000%20prototype%29/track03.bin), [track05 / game executable extent](San%20Francisco%20Rush%202049%20%28Apr%2010%2C%202000%20prototype%29/track05.bin), [archive](San%20Francisco%20Rush%202049%20%28Apr%2010%2C%202000%20prototype%29.7z), [IP.BIN](San%20Francisco%20Rush%202049%20%28Apr%2010%2C%202000%20prototype%29/ip.bin), [dump metadata](San%20Francisco%20Rush%202049%20%28Apr%2010%2C%202000%20prototype%29/info.txt) |
| August 1 | `San Francisco Rush 2049 (Aug 1, 2000 prototype)` | [GDI](San%20Francisco%20Rush%202049%20%28Aug%201%2C%202000%20prototype%29/disc.gdi), [track03 / boot area](San%20Francisco%20Rush%202049%20%28Aug%201%2C%202000%20prototype%29/track03.bin), [track05 / game executable extent](San%20Francisco%20Rush%202049%20%28Aug%201%2C%202000%20prototype%29/track05.bin), [archive](San%20Francisco%20Rush%202049%20%28Aug%201%2C%202000%20prototype%29.7z), [IP.BIN](San%20Francisco%20Rush%202049%20%28Aug%201%2C%202000%20prototype%29/ip.bin), [dump metadata](San%20Francisco%20Rush%202049%20%28Aug%201%2C%202000%20prototype%29/info.txt) |
| August 7 | `San Francisco Rush 2049 (Aug 7, 2000 prototype)` | [GDI](San%20Francisco%20Rush%202049%20%28Aug%207%2C%202000%20prototype%29/RUSH%202049%20v1.000%202000-08-07%20E%20White-label%20Bananabreak/disc.gdi), [track03 / boot area](San%20Francisco%20Rush%202049%20%28Aug%207%2C%202000%20prototype%29/RUSH%202049%20v1.000%202000-08-07%20E%20White-label%20Bananabreak/track03.bin), [track05 / game executable extent](San%20Francisco%20Rush%202049%20%28Aug%207%2C%202000%20prototype%29/RUSH%202049%20v1.000%202000-08-07%20E%20White-label%20Bananabreak/track05.bin), [archive](San%20Francisco%20Rush%202049%20%28Aug%207%2C%202000%20prototype%29.7z), [dump metadata](San%20Francisco%20Rush%202049%20%28Aug%207%2C%202000%20prototype%29/RUSH%202049%20v1.000%202000-08-07%20E%20White-label%20Bananabreak/disc.txt) |

August 7 has an additional nested directory: `RUSH 2049 v1.000 2000-08-07 E White-label Bananabreak`. The links above include that directory.

## Disc, executable, and resource formats

- Each build is a five-track GDI GD-ROM image: data tracks 1/3/5 use 2352-byte Mode 1 sectors; tracks 2/4 are audio. Both low- and high-density ISO9660 sessions were enumerated. Track 3 starts at GDI LBA 45000.
- All four boot filenames are `1ST_READ.BIN`. They are headerless little-endian SH-4 programs, not ELF or COFF containers. The standard load address is `0x8C010000`; a file offset plus that base gives a candidate loaded address for ordinary resident data. Validate runtime copies/overlays before treating this as a universal mapping.
- `.ROM` names refer to proprietary game resources/data, not N64 cartridge images. Early builds use many uncompressed `.ROM` assets; August builds use a custom LZSS-style `.LZS` encoding. Decoding all 314 August 1 and 111 August 7 files completed with valid backreferences and explicit end tokens.
- `.KAT` files are audio-bank resources; `.STR` files are audio streams; `.SFD` files have MPEG program-stream signatures. Audio64 `.DRV` files begin with ARM code. `MANATEE*.DRV` uses an `SDRV` header. `2_DP.BIN` is the PlanetWeb browser, not a symbol-rich copy of Rush.
- August 1 also includes browser `.PSD` artwork and `.OLD` files with GIF signatures. Those are not source/object/debug companions.

The load convention is described by Marcus Comstedt in [IP.BIN and 1ST_READ.BIN](https://mc.pp.se/dc/ip.bin.html). This reference supports the format/load convention; build-specific findings below come from the local bytes.

## Executable identity and filesystem timestamps

Timestamps below are the main executable's ISO9660 directory-record timestamps, including the encoded UTC offset. They agree with the folder labels. Host filesystem modification dates reflect later dump/copy operations and should not be used as compile dates. These recorded timestamps are evidence, not independently verified compiler times.

| Build | `1ST_READ.BIN` bytes | ISO9660 executable timestamp | LBA | High-density files |
|---|---:|---|---:|---:|
| February 17 | 591,856 | `2000-02-17T17:29:50-08:00` | 548711 | 179 |
| April 10 | 858,148 | `2000-04-10T15:20:28-08:00` | 548580 | 396 |
| August 1 | 1,123,492 | `2000-08-01T17:08:20-08:00` | 548451 | 791 |
| August 7 | 1,131,300 | `2000-08-07T16:40:00-08:00` | 548447 | 125 |

Each low-density filesystem contains four files. Total listed file counts, including both sessions, are February 183, April 400, August 1 795, and August 7 129. August 7 has substantially fewer assets; that observation alone does not establish its retail/demo status.

Full executable SHA-256 hashes:

- **February 17:** `fa3535c44bdfd6b8268dd009c2f02e0125906f0d2b335640d9d63efd5be5dc0d`
- **April 10:** `f1f414602df378069199931f54093480bc43e09c4168fd612cd9bdef12e11776`
- **August 1:** `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`
- **August 7:** `fd713902764734adb6bb3f2234560ef197108bfe7dbc2ff1ad112647e446d263`

## IP.BIN metadata and volume dates

`IP.BIN` here means the reconstructed 32,768-byte boot area at LBA 45000 (track 3). For April and August 1 it exactly matched the supplied `ip.bin` companion. February and August 7 have no separately supplied `ip.bin` in their original directories.

| Build | IP product / version | IP date | Regions / peripherals | Volume | PVD timestamp as stored |
|---|---|---|---|---|---|
| February 17 | `T9708N / V1.000` | `20000218` | `JUE / 0799010` | `RUSH1` | `20000221154827-32` |
| April 10 | `T0000M / V0.800` | `20000403` | `U / 0799810` | `RUSH31` | `2000032015001200\xe0` |
| August 1 | `T9707N / V0.900` | `20000630` | `U / 0799A10` | `US` | `2000073117165700\xe0` |
| August 7 | `9D-0800-50 / V1.000` | `20000807` | `E / 079BA10` | `2049PAL1` | `20000807184121-32` |

PVD = ISO9660 primary volume descriptor. April's apparent volume creation time is March 20, 2000 at 15:00:12 with offset -08:00. August 1's high-density PVD records July 31 at 17:16:57 (-08:00); its low-density PVD is one second earlier. February and August 7 store literal trailing `-32` bytes in this field instead of the normal two hundredths digits plus a signed timezone byte; the first 14 digits read February 21 at 15:48:27 and August 7 at 18:41:21, respectively. Preserve the raw values rather than normalizing malformed authoring metadata into a precise timestamp.

IP dates are stale relative to some executable dates, so an IP date is not a reliable way to date the gameplay binary. February/August 7 title is `RUSH 2049`; August 1 is `SAN FRANCISCO RUSH 2049`. April uses a sample-disk warning title and `T0000M / V0.800`.

IP.BIN SHA-256 hashes:

- **February 17:** `f6022f342833088f4713459a413c8d677c29f25d7ebc09d809ff65d944582f7a`
- **April 10:** `6657c9806ba46f21a0f4fbb4d7b62a540cacc8aba4fcbd053eb8bf28e0059974`
- **August 1:** `ed30d037b9c8a680f39d3533cce745a6e3930a9c4da1bc4585116e79bb77c2db`
- **August 7:** `b8a55231c3743304a0652c24c7d1a626b368e400072dd74729e763e635287bec`

## Actual debug metadata versus useful strings

| Recovered formal metadata | February | April | August 1 | August 7 |
|---|---:|---:|---:|---:|
| Valid ELF/COFF debug-bearing files | 0 | 0 | 0 | 0 |
| Symbol-table entries | 0 | 0 | 0 | 0 |
| Debug type records | 0 | 0 | 0 | 0 |
| Source-line records | 0 | 0 | 0 | 0 |
| Map/linker listings or object/library companions | 0 | 0 | 0 | 0 |

No recoverable DWARF/STABS sections, PDBs, `.o`/`.obj` companions, maps, or unstripped gameplay executable were identified. Known format/section signatures and plausible COFF structures were checked in allocated files; raw tracks also received marker sweeps. All decoded LZS assets were scanned for debug-container/section markers. This is a bounded static inspection, not a proof that arbitrary opaque data could never encode an undocumented proprietary format or recoverable deleted material.

One `7F 45 4C 46` sequence occurs in August 1 `WINGEY.STR` at file offset `0x2956934` (43,346,228), raw `track05.bin` offset 1,093,040,580. Its identification bytes are `7f454c46004ae948de491f4ea6503651`: ELF class 0, data encoding `0x4A`, and version `0xE9` are invalid. This is an audio-data coincidence, not an ELF.

Short strings resembling filenames, such as `s2.h` inside SH-4 instruction bytes, were likewise not counted as source evidence. Runtime names below are labels/diagnostics, not formal function or type symbols.

## Verified resource-name tables

Offsets in this section are byte offsets in extracted `1ST_READ.BIN`. Pointers are 32-bit little-endian loaded addresses. Each name-table entry was paired with the same-index filename-table entry, and **all 155/160 named files exist on the corresponding disc**. The numeric `id` in the evidence is the zero-based observed table index; an original source enum declaration has not been recovered.

| Build | Name/filename pairs | Name pointer table | Filename pointer table | Portable evidence |
|---|---:|---|---|---|
| February 17 | 155 | `0x74FEC` | `0x75258` | [JSON](betafindings-evidence/Feb_17_2000_Dreamcast_Prototype/resource_pairs.json), [CSV](betafindings-evidence/Feb_17_2000_Dreamcast_Prototype/resource_pairs.csv) |
| April 10 | 160 | `0x82F64` | `0x831E4` | [JSON](betafindings-evidence/Apr_10_2000_prototype/resource_pairs.json), [CSV](betafindings-evidence/Apr_10_2000_prototype/resource_pairs.csv) |
| August 1 | 0 readable `kWave...` name pairs recovered | — | — | Filenames remain, but the early readable name tables were not found. |
| August 7 | 0 readable `kWave...` name pairs recovered | — | — | Filenames remain, but the early readable name tables were not found. |

Representative examples: `kWaveTextE -> texte.rom` and `kWaveTextS -> texts.rom` in both early builds; April ends with `kWavePathBackTrack6 -> track6pathback.rom`, February with `kWavePathBackObstacle1 -> obstacle1pathback.rom`. These mappings help recover resource numbering and identify callers of the resource loader.

## Gameplay and SDK diagnostics

Offsets below identify literal strings inside `1ST_READ.BIN`, **not function entry points**. Use code/data cross-references to identify callers before applying function names.

| Literal / diagnostic | February offset | April offset | August 1 offset | August 7 offset |
|---|---|---|---|---|
| `SelectBlit(%s) bad blit, blit:%p info:%p` | `0x60FF4` | `0x6CED4` | Not found | Not found |
| `AllocateMemoryAt: Ptr (%x) is not 64-byte aligned.` | `0x64C54` | `0x709BC` | Not found | Not found |
| `(buffer + size) > &gPaks[kMaxPaks] in PfsSetBlock` | `0x650F8` | `0x70FA4` | Not found | Not found |
| `Bad newWave in ReplaceWave` | `0x67970` | `0x73EF4` | Not found | Not found |
| `Katana Debug Adapter` | Not found | `0x68BE0` | `0x94F80` | `0x94DE0` |
| `amHeapAlloc - audio heap not initialized` | `0x6BB34` | `0x7A7D0` | `0xA6DBC` | `0xA6C28` |

`Katana Debug Adapter` is an SDK/device-identification string and does not establish an active remote debugger or a debug-enabled gameplay build. Audio routine names such as `amHeapAlloc`, `amHeapCheck`, and `amSoundFetchSample` mostly identify Sega audio-library code. `PfsSetBlock` is a potentially useful shared-code naming clue for N64 comparison, but its presence alone does not prove byte/function equivalence.

Additional filtered strings: [February 17](betafindings-evidence/Feb_17_2000_Dreamcast_Prototype/diagnostics.json), [April 10](betafindings-evidence/Apr_10_2000_prototype/diagnostics.json), [August 1](betafindings-evidence/Aug_1_2000_prototype/diagnostics.json), [August 7](betafindings-evidence/Aug_7_2000_prototype/diagnostics.json). These are search candidates and may include incidental printable bytes; the representative rows above were manually checked.

### Runtime-library fingerprints

| Build | Shinobi | Audio64 | Kamui2 |
|---|---|---|---|
| February | 1.43 | 9.0.b.3 | 16,3,2,4 |
| April | 1.65 | 10.0.f.1 | 16,3,4,0 |
| August 1 | 1.68 | 10.0.f.2 | 16,3,4,0 |
| August 7 | 1.68 | 10.0.f.2 | 16,3,4,0 |

Exact banner strings, offsets, and library build dates are in [summary.json](betafindings-evidence/summary.json). They are linked library fingerprints, **not an established compiler vendor/version**. No trustworthy GCC/Metrowerks/Hitachi/SN Systems compiler signature was recovered.

## Menus and path-editor evidence

All four English text resources contain `DEBUG`, `PATH EDITOR`, and `FRAME SCALE`. February and April also include an explicit `PATHEDIT.ROM` asset (96,468 and 184,732 bytes respectively); no `PATHEDIT.*` file was found in either August filesystem.

| Build / logical text resource | `DEBUG` | `PATH EDITOR` | `FRAME SCALE` |
|---|---|---|---|
| February `TEXTE.ROM` | `0x1A33` | `0x1A39` | `0x1FDC` |
| April `TEXTE.ROM` | `0x1A6F` | `0x1A75` | `0x2092` |
| August 1 decoded `TEXTE.LZS` | `0x22E7` | `0x22ED` | `0x2D59` |
| August 7 decoded `TEXTE.LZS` | `0x22D8` | `0x22DE` | `0x2D4A` |

August offsets refer to decompressed bytes, not the compressed disc file. `CHEATS`, `GRAVITY`, and `INVINCIBLE` labels also remain. **Menu reachability, controller sequences, editor functionality, and whether relevant code is enabled are untested.** Text and fonts alone are insufficient to establish a working developer menu.

## Browser-only source and assertion evidence

| Build | Verified source-filename literals in `2_DP.BIN` | Assertion evidence |
|---|---:|---|
| February | No `2_DP.BIN` | None identified for this browser |
| April | 2 | No explicit assertion format identified |
| August 1 | 13 | `assertion %s failed in %s, at line %ld!` at `0x107CA8` |
| August 7 | No `2_DP.BIN` | None identified for this browser |

April: `cursor.c` at `0xCEF14` and `../common/mpgaudio.c` at `0xD2BF0`.

August 1: `../common/melan.c`, `cursor.c`, `../common/mpgaudio.c`, `kgevt.c`, `kgactstr.c`, `kgfil.c`, `kgtrnstr.c`, `kgtrn.c`, `kgtrnweb.c`, `kgfsh.c`, `uthmn.c`, `utvct.c`, and `utbit.c`. Assertion expressions such as `flags != 0` and `size_of_payload != 0` occur nearby. Exact offsets: [August 1 browser literals](betafindings-evidence/Aug_1_2000_prototype/browser_source_literals.json); [April browser literals](betafindings-evidence/Apr_10_2000_prototype/browser_source_literals.json).

These are genuine diagnostic filename/expression strings for PlanetWeb and linked browser support code. They do not provide source contents, line-table records, or Rush gameplay source filenames. Their usefulness for the gameplay decompilation is limited.

## Evidence index and inspection/reproduction

- [summary.json](betafindings-evidence/summary.json): curated per-build metadata, executable/IP hashes, resource-table offsets, browser literals, and SDK banners.
- [inventory.json](betafindings-evidence/inventory.json): both-session ISO9660 listings with file paths, sizes, LBAs, flags, and raw directory dates.
- [scan_all.json](betafindings-evidence/scan_all.json): every listed file's SHA-256, raw-track SHA-256/CRC32, signatures, and filtered-string candidates. `markers` are raw hits requiring validation; the invalid audio ELF coincidence is retained rather than hidden.
- [lzs_all.json](betafindings-evidence/lzs_all.json): all 425 successful decompressions, compressed/decompressed sizes, decoded SHA-256, and marker/string candidates.
- [inspect_prototypes.py](betafindings-evidence/inspect_prototypes.py): small standard-library Python helper for re-reading only these four known prototypes, checking executable/IP hashes, and validating early resource tables. It does not run binaries. Optional extraction requires a separate output directory.
- [evidence-manifest.json](betafindings-evidence/evidence-manifest.json): SHA-256 and sizes of the portable evidence files.

Original paths embedded inside the copied JSON are provenance from this machine. The Markdown links and helper use relative paths, so the document and evidence folder can move with the game directories. Raw evidence may contain regex false positives; only findings explicitly validated in this document should be treated as established.

From this root, verify one known prototype without writing or running a game:

```powershell
python .\betafindings-evidence\inspect_prototypes.py --build apr10
```

Use `feb17`, `aug1`, or `aug7` for the others. To reproduce executable/IP extraction into a separate scratch directory, explicitly supply an output path outside the original game root:

```powershell
python .\betafindings-evidence\inspect_prototypes.py --build apr10 --out "$env:TEMP\rush2049-apr10-check"
```

The helper reads the GDI-listed data tracks, strips Mode 1 sector framing, reconstructs the recorded executable extent and boot area, compares their hashes with the captured evidence, and checks early name/filename pointers against the saved mapping. It never selects files by a broad root scan, so newly added retail directories are excluded. Full allocated-file/raw-track/LZS sweeps were completed in the original inspection; this helper is intentionally a focused reproducibility check, not a replacement for those broader sweeps.

## Priority for the forthcoming retail comparison

1. Inventory the retail files, identify the disc/container and boot executable, and record fresh SHA-256/IP metadata before importing code. **Retail has not been inspected yet.** Do not assume its region, revision, dates, compiler, or resource numbering matches a prototype.
2. Compare retail asset names and loading tables against April's 160 verified mappings and February's 155. Retain per-build IDs rather than copying indices blindly.
3. Use diagnostic-string cross-references and verified library banners to identify SDK/audio code, then compare gameplay functions and structures with August 1's later implementation. Keep browser findings separate from game findings.
4. Trace menu data to code before claiming a developer menu is reachable. Treat an emulator/runtime check as future separately authorized work; none occurred in this inspection.
5. Transfer names, constants, resource semantics, and algorithm hypotheses to N64/GameCube MAT3 only after independent checks. Dreamcast SH-4 is little-endian; N64 MIPS and GameCube PowerPC use different instruction sets, ABIs, endianness, libraries, and compiler behavior. These prototypes are semantic references, not drop-in function matches or source/type recovery.

No decompilation, N64/MAT3 matching work, integration build, or CI watcher was started for this documentation request.
<!-- END RUSH2049 PROTOTYPE FINDINGS -->
