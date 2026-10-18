# Focused SH-4 decoder

`../../tools/sh4.py` is a new pure-Python static-analysis helper written for this bounded pilot. It has no dependencies outside Python's standard library and executes no target code. It is not a complete emulator, established disassembler replacement, function-boundary proof, or compiler fingerprint.

## API

```python
from sh4 import decode, disassemble, walk_cfg, literal_xrefs, direct_calls, linear_indirect_xrefs
i = decode(word, pc, blob=None, base=0x8C010000, fpscr=None)
print(i.text, i.kind, i.target, i.delay_slot)
print(i.literal_address, i.literal_value, i.reads, i.writes)
record = i.to_dict()
```

`decode` receives a **16-bit integer instruction word** and **virtual PC**. `blob` starts at `base`; real-target byte order is little-endian. `disassemble`, `literal_xrefs`, `direct_calls`, and `linear_indirect_xrefs` take **file offsets** `start`/`end`. `walk_cfg` takes **virtual addresses** `entry`, `lower`, and `upper`; `upper` is exclusive. Its instruction limit defaults to 4096. The CLI's `--start`/`--end` are file offsets and `--cfg` is a virtual entry address.

Implemented common integer moves/arithmetic/compares/shifts, special-register transfers, direct/indirect branches and calls, returns, traps, PC-relative literal access, and common SH-4 FPU scalar/vector operations. Mnemonics use source,destination order. Register read/write metadata is useful for candidate tracing and is **not an exhaustive architectural effects model** (SR flag details, memory effects, and exceptions are not modeled).

`Instruction.kind` is `normal`, `branch`, `call`, `return`, `trap`, or `unknown`. Unknown words stay `.word 0xNNNN`, `known=False`. CFG traversal stops on unknown instructions and unknown delay slots. Delayed transfers include their slot instruction before conceptual branch edges. Calls return to `pc+4`; ordinary non-delayed conditional branches fall through to `pc+2`. The CFG does not enter called functions, resolve jump tables, or follow indirect branches. A leaf RTS does not prove a function's total extent or ownership of adjacent literal pools.

PC-relative operands use the normal instruction PC. If such an instruction is in a branch delay slot, its operand needs branch context; CFG reports a warning. FPU fields are displayed as FR fields with an explicit FPSCR warning unless `fpscr` is supplied. Optional PR/SZ interpretation handles common double-register aliases; full FPSCR validity checks and bank tracking are outside scope. Privileged-instruction variant legality is not exhaustively checked. Default FPU names alone do not establish the runtime FPSCR state.

Linear sweeps can decode pool/data words as plausible instructions. `literal_xrefs` and `direct_calls` return **candidates**, not reachable-code claims. `linear_indirect_xrefs` tracks MOV literal/immediate/register and ADD immediate in one straight-line region, invalidating state on control transfers and unknowns; it skips delay slots and never propagates values across a call. Its results also require CFG review. It is deliberately small and conservative.

## Reproduce

From the pilot root, use an available Python 3.12 interpreter:

```powershell
python tools/sh4.py --selftest
python tools/sh4.py orig/1ST_READ.BIN --start 0 --count 16
python tools/sh4.py orig/1ST_READ.BIN --cfg 0x8C010000 --count 64
```

The focused tests cover startup PC-relative alignment, signed branch displacements, delayed/non-delayed conditional branches, FPU N/M and vector register fields, unknown preservation, MOV.W sign extension, CFG delay slots, and literal-backed indirect candidate resolution. See `selftest.json` for the exact run result. Real startup output is retained in `startup.json` and `startup-cfg.json`. In particular:

- `D004` at file `0x1A` / VA `0x8C01001A` loads the 32-bit literal at file `0x2C`, value `0x8C0D2480`.
- `402B` at file `0x1C` is `jmp @r0`, followed by a `nop` delay slot at `0x1E`.
- The CFG from entry `0x8C010000` stops at that unresolved indirect jump; the separate linear trace reports the literal-backed target as a candidate.

Opcode facts and delay/register-field checks were cross-checked against primary implementation sources: [QEMU SH disassembler table](https://raw.githubusercontent.com/qemu/qemu/master/disas/sh4.c) and [Linux SH-4 FPU exception handler](https://raw.githubusercontent.com/torvalds/linux/master/arch/sh/kernel/cpu/sh4/fpu.c). No source package, executable, or external dependency was installed.
