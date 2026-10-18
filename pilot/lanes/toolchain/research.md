# Bounded SH-4 toolchain investigation

The remaining local search located Microsoft LLVM 19.1.5 outside PATH, but **no usable SH-4 compiler, assembler, linker, or stock SH-4 disassembler is established**. This is a scoped result, not a declaration that tools are absent from every disk/archive. [Machine-readable inventory and hashes](inventory.json); [reproducible read-only inventory script](collect_inventory.py).

## Available tools and limits

- `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\Llvm\x64\bin\clang.exe` reports 19.1.5. Its Authenticode signature is valid and names Microsoft. `--print-targets` and `llvm-objdump --version` list ARM/AArch64 and x86 targets only. They cannot compile or disassemble this SH-4 target. Clang may support host-only semantic fixtures, provided those are kept separate from DC ABI/emission claims. `lld.exe` was located and hashed, not used to link a target.
- MAT3's previously catalogued GC/Wii/X360 and ProDG NGC bundles are architecture-specific reference tools, not located SH tools. Their provenance/hashes remain in the [prior inventory](/C:/rush2049-dreamcast/mat3-to-dreamcast-evidence/tool-inventory.json).
- PATH, named common SDK/tool roots, Downloads' top level, user `.local`/`.cache`/Programs/LocalAI, Git's tool directory, and Visual Studio tool filenames were checked. No Katana/Hitachi/Renesas/Ghidra folder was found in these checked locations. The `recomp-v1.0.tar.gz` archive was only listed at its gzip layer; its 1.1 GB inner tar was not searched or extracted. No claim is made about its contents.
- **Denied action:** `C:\Windows\system32\wsl.exe --list --verbose` (read-only distribution inventory) returned exit 1, `Access is denied. Error code: Wsl/E_ACCESSDENIED`. No distro or tools inside it were enumerated, no retry or system setting change was attempted. This is a command/runtime denial; no automatic approval-review rejection occurred.

## Compiler family evidence: unresolved

The pinned Dreamcast executable is raw SH-4. Shinobi/Kamui/Audio64 version banners identify linked SDK/runtime components; they do not identify the producer of gameplay objects. Library code, handwritten startup, and gameplay may have different origins. No compiler-vendor conclusion is justified by a normal SH prologue or the R4-R7 argument convention.

A bounded printable compiler-marker scan of the pinned retail image found only `Gcc<7` at file `0x556EF`: five incidental printable bytes without a vendor/version banner. This is rejected as compiler evidence. No genuine compiler marker was recovered by this scan; absence from a stripped flat binary does not identify a family.

Primary vintage source establishes an important control: **GCC 2.95.2 supports Hitachi-compatible ABI mode**. Its SH target configuration includes `-m4`, `-m4-single`, `-m4-single-only`, `-ml`, and `-mhitachi`; the latter also changes MAC-register preservation. The same source defines 32-bit pointers and conditional double alignment. Thus a Hitachi ABI fingerprint alone cannot distinguish Hitachi SHC from GCC. Inspect `CPP_SPEC`, `TARGET_SWITCHES`, `CONDITIONAL_REGISTER_USAGE`, layout and argument rules in the [GCC 2.95.2 target source](https://raw.githubusercontent.com/gcc-mirror/gcc/releases/gcc-2.95.2/gcc/config/sh/sh.h). Its incomplete manual must not be used to reject source-supported SH-4 options.

The [official GCC release history](https://gcc.gnu.org/gcc-2.95/) dates 2.95.2 to October 27, 1999 and 2.95.3 to March 16, 2001. The former is temporally plausible for an August 2000 binary; the latter is a later analysis/control candidate unless a dated vendor fork is demonstrated. Neither is evidence this game used GCC.

The [Renesas SH compiler option guide](https://www.renesas.com/en/document/apn/sh-compiler-application-note-2-compiler-use-guide-option-guide) is **version 9, September 2007**, not documentation proving a 2000 release. It demonstrates option-sensitive inlining, constant loading, FP policy, and MAC preservation. Consult the actual acquired vintage package documentation before choosing SHC commands/options.

Recommended discriminators after mappings freeze: integer loop and indirect-call lowering (`bsearch`), pointer/data-store ordering (`InsertLLE`), FP scheduling/precision and literal sharing (`interp`, `BodyVector`), helper-call bodies/names when reliably recovered, register saves, stack cleanup, aggregate/varargs behavior, and the exact linker/pool consequences. Compare several nontrivial bodies; do not attribute a family from one epilogue or FP call convention. A mixed SDK/gameplay compiler remains possible. No training configuration has been run, and no held-out behavior was used to tune a configuration.

## Exact next dependencies and recognized sources

1. **Best path:** provide the path to a lawfully available period Katana/Hitachi SHC package already held by the user, including compiler, assembler, linker, headers, runtime libraries and version/help documentation. Inventory/hashes/help come first. No unofficial proprietary compiler mirror was sought or downloaded.
2. **Open-source calibration/control path:** provision a separate local build of GNU GCC 2.95.2 plus a compatible historical SH Binutils release, from official GNU sources. The [official GNU 2.95.2 archive](https://ftp.gnu.org/gnu/gcc/gcc-2.95.2/) is an exact recognized acquisition source. This requires an approved download/build environment and dependency plan; no acquisition or compilation was attempted. Compiler target/configuration and libc/ABI choices still require evidence; choosing `sh-elf` by convenience does not identify the original object format. A modern SH Binutils build can independently validate decoding/relocations while remaining separate from vintage reproduction.
3. **Analyzer path:** a verified official [NSA Ghidra release](https://github.com/NationalSecurityAgency/ghidra/releases) and the JDK required by that release would provide a reviewable stock analyzer. Download/extract/install and execution were not performed. Verify the selected release's actual SuperH processor language/ABI rather than assuming any release contains the needed module. This is an analysis dependency, not a replacement compiler.
4. If existing WSL tools are expected, obtain a read-only WSL inventory through an allowed execution context before installing duplicates. The denied command above leaves its contents unknown.

When a GNU objdump with SH support becomes available, first run `objdump -i` to verify registered targets. Then use a raw-binary input, explicit little endianness, SH-4 machine, and `--adjust-vma=0x8c010000` for the pinned executable. Exact machine spelling is taken from that tool's target list. The [primary objdump manual](https://sourceware.org/binutils/docs/binutils/objdump.html) documents these selectors and relocation reporting. Disassembling all raw bytes also decodes pools/data; boundary manifests must still distinguish them.

## Current pilot status

This lane has made **zero compiler invocations, zero emitted SH objects, zero target links, and zero exact-match claims**. Root-owned correspondence and comparison-harness work can continue independently. Stop before flag searches until a usable target compiler/assembler/linker and frozen span/ABI manifest exist.
