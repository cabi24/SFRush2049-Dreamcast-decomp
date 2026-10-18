# Dreamcast Rush 2049 bounded pilot

The first static pilot is complete. **Five strong MAT3 semantic identities, one API counterpart with confirmed source drift, and one unlocated slot. Zero accepted equivalent-C functions, SH-4 objects or exact local links.** See [results and blocker](PILOT-RESULTS.md) and [machine-readable progress](analysis/progress.json).

The target is US v1.001, 1,123,492 bytes, raw little-endian SH-4 at `0x8C010000`, SHA256 `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`. Its executable equals the Aug 1 prototype. The fixed split remains four training functions and three held-out functions; the configuration trial cap remains 16, with zero used.

## Files and boundaries

- [Manifest](analysis/manifest.json): all seven slots, exact span hashes, conservative identities and one-time shared literal groups.
- [Integer evidence](lanes/integer/notes.md), [resource/FP evidence](lanes/resource_float/notes.md): instructions, callers, layouts, alternatives and drift.
- [Tool inventory](lanes/toolchain/research.md): no usable SH-4 compiler/assembler/linker found in the bounded search; recognized provisioning paths and exact WSL denial.
- [Static decoder](lanes/decoder/README.md): dependency-free partial analyzer, not a complete emulator or authoritative disassembler.
- [Training C scaffolds](src/training/README.md): explicit hypotheses; not source promotions.
- `tools/compare.py`: strict unmasked raw/ELF32-LE-SH ET_EXEC comparison. ET_REL is rejected for linked-match acceptance. `tools/object_info.py` reports typed ELF relocations without applying or masking them.
- `tools/probe.py`: provenance/hash-pinned compiler gate, a cap on distinct training configurations, isolated output/logs, and external frozen-hash enforcement for held-out validation. It does not acquire tools, mini-link objects or auto-promote matches. Linker configuration/maps and full ABI probes remain future work.

The extracted executable stays in task scratch. No binary/track archive is included here. `config/target.json` points to that extraction, and tools verify its target hash. All disc media and MAT3 were read-only. The 31 pinned MAT3 references were rehashed after the mapping pass. Original-object equality is unavailable from the flat target.

## Reproduce the current checks

From this directory with existing Python 3.12:

```powershell
python -B tools/validate.py
python -B tools/consolidate.py
python -B tools/probe.py interp --dry-run
```

The last command deliberately returns 3 while the toolchain is unavailable. `validate.py` expects that initial blocked state; after provisioning, review it and the frozen evidence before using it as a new acceptance suite. It performs no target compilation. Validation contains 14 synthetic verifier/object controls and 16 decoder controls, plus target/span/reference rechecks. The target self-comparison is labeled a control and earns zero source bytes.

For a future generated linked artifact, `compare.py` can compare `analysis/identity-code-spans.json`; its pass alone is insufficient. Also verify the shared dependencies in `analysis/dependency-spans.json`, generated source/tool provenance, ABI/semantics, padding, section placement and linker map. There is currently no generated SH artifact to compare.

Do not run wide flag searches, expand the function set, import GC backends, or count original/assembly fallback bytes as C. Do not run a full integration build or watcher as part of this pilot. The next dependency is a reviewed SH-4 compiler/assembler/linker, followed by compiler/ABI calibration on the four training slots. See [the original plan](../mat3-to-dreamcast-plan.md) for stop and expansion gates.
