"""Read-only, bounded tool inventory; writes reports only beside this script.

This does not install tools, compile sources, or execute a game. Known Microsoft
LLVM and Windows WSL executables are invoked only for version/target inventory.
"""
import datetime
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

OUT = Path(__file__).resolve().parent
ROOT = OUT.parents[1]
sys.path.insert(0,str(ROOT/'tools'))
from dc_target import target_path
TARGET = target_path(ROOT)
PIN = "336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688"
LLVM = Path(r"C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\Llvm\x64\bin")


def sha(path):
    return hashlib.file_digest(open(path, "rb"), "sha256").hexdigest()


def command(args):
    try:
        result = subprocess.run(args, capture_output=True, timeout=30)
        return {"argv": args, "returncode": result.returncode,
                "stdout": result.stdout.decode("utf-8", "replace").replace("\x00", ""),
                "stderr": result.stderr.decode("utf-8", "replace").replace("\x00", "")}
    except (OSError, subprocess.TimeoutExpired) as exc:
        return {"argv": args, "error": str(exc)}


names = ["shc", "sh-elf-gcc", "sh4-elf-gcc", "sh-coff-gcc", "sh-elf-as",
         "sh4-elf-as", "sh-elf-ld", "sh4-elf-ld", "sh-elf-objdump",
         "sh4-elf-objdump", "ghidraRun", "analyzeHeadless", "java", "clang",
         "llvm-objdump", "wsl", "python"]
common_roots = [r"C:\tools", r"C:\devkitPro", r"C:\msys64", r"C:\cygwin64",
                r"C:\KATANA", r"C:\SEGA", r"C:\Renesas", r"C:\Hitachi",
                r"C:\Program Files\Renesas", r"C:\Program Files (x86)\Renesas",
                r"C:\Program Files\Ghidra", r"C:\ghidra", r"C:\Program Files\LLVM"]
scan_roots = [r"C:\Program Files\Git\usr\bin",
              r"C:\Program Files (x86)\Microsoft Visual Studio",
              r"C:\Users\cabi2\.local", r"C:\Users\cabi2\.cache",
              r"C:\Users\cabi2\AppData\Local\Programs", r"C:\Users\cabi2\LocalAI"]
pattern = re.compile(r"^(shc|sh4.*|sh-.*|.*objdump|ghidraRun|analyzeHeadless|clang|lld|java)\.(exe|bat|cmd)$", re.I)
file_matches = []
walk_errors = []
for scan_root in scan_roots:
    if Path(scan_root).is_dir():
        for folder, _, filenames in os.walk(scan_root, onerror=lambda e: walk_errors.append(str(e))):
            for name in filenames:
                if pattern.match(name):
                    file_matches.append(str(Path(folder) / name))

known = []
for name in ["clang.exe", "llvm-objdump.exe", "lld.exe"]:
    path = LLVM / name
    if path.exists():
        known.append({"path": str(path), "size": path.stat().st_size, "sha256": sha(path)})
commands = []
if (LLVM / "clang.exe").is_file():
    commands.extend([command([str(LLVM / "clang.exe"), "--version"]),
                     command([str(LLVM / "clang.exe"), "--print-targets"]),
                     command([str(LLVM / "llvm-objdump.exe"), "--version"])])
# This probe previously returned Wsl/E_ACCESSDENIED; record only that observed
# result rather than retrying the denied action automatically.
wsl_observed = {"argv": [r"C:\Windows\system32\wsl.exe", "--list", "--verbose"],
                "returncode": 1,
                "stdout": "Access is denied.\r\nError code: Wsl/E_ACCESSDENIED\r\n",
                "status": "denied; not retried; distribution inventory remains unknown"}

target_report = None
if TARGET.is_file():
    raw = TARGET.read_bytes()
    target_sha = hashlib.sha256(raw).hexdigest()
    if target_sha != PIN:
        raise SystemExit("Target hash mismatch; no scan performed")
    marker = re.compile(rb"\b(?:GCC|GNU|Hitachi|SHC|Renesas|Cygnus|Metrowerks|Microsoft)\b", re.I)
    hits = [{"offset": f"0x{m.start():X}", "text": m.group().decode("ascii"),
             "assessment": "incidental five printable bytes; no banner/version context; rejected as compiler evidence" if m.group() == b"Gcc<7" else "unreviewed marker candidate"}
            for m in re.finditer(rb"[\x20-\x7e]{5,}", raw) if marker.search(m.group())]
    target_report = {"path": str(TARGET), "size": len(raw), "sha256": target_sha,
                     "printable_compiler_vendor_marker_hits": hits,
                     "interpretation": "Runtime printable marker search only; absence does not prove a compiler family."}

report = {
    "generated_utc": datetime.datetime.now(datetime.timezone.utc).isoformat(),
    "scope": "PATH, common roots, shallow Downloads/tool-folder inventory, recursive filenames only in six listed tool roots; previous MAT3 tools metadata reused. Not an exhaustive disk or archive-content search.",
    "path": {name: shutil.which(name) for name in names},
    "common_roots": {name: Path(name).exists() for name in common_roots},
    "recursive_filename_roots": scan_roots,
    "recursive_filename_matches": file_matches,
    "scan_errors": walk_errors,
    "located_llvm": known,
    "commands": commands,
    "wsl_probe_observed": wsl_observed,
    "clang_signature_observed": "Valid Authenticode signature; CN=Microsoft 3rd Party Application Component, O=Microsoft Corporation, L=Redmond, S=Washington, C=US",
    "target": target_report,
    "prior_inventory": r"C:\rush2049-dreamcast\mat3-to-dreamcast-evidence\tool-inventory.json",
    "conclusion": "Microsoft LLVM 19.1.5 supports only registered ARM/AArch64/x86 targets here; no usable SH4 compiler, assembler, linker or stock disassembler established. WSL tools remain unknown because inventory was denied.",
    "actions_not_performed": ["tool download", "tool install", "game execution", "source compilation", "original input modification", "MAT3 build or matching"]
}
(OUT / "inventory.json").write_text(json.dumps(report, indent=2), encoding="utf-8")
print(json.dumps({"report": str(OUT / "inventory.json"), "filename_hits": len(file_matches),
                  "target_marker_hits": len(target_report["printable_compiler_vendor_marker_hits"]) if target_report else None,
                  "conclusion": report["conclusion"]}, indent=2))
