"""Small, dependency-free static SH-4 LE decoder for the Dreamcast pilot.

Not an emulator or a validated replacement for an established disassembler.
Unknown encodings are retained as .word and stop conservative CFG traversal.
FPU register fields are displayed as FR fields unless fpscr is supplied; PR/SZ
affect register interpretation. PC-relative operands assume a non-delay slot.
References consulted (opcode facts, implementation written for this pilot):
  https://raw.githubusercontent.com/qemu/qemu/master/disas/sh4.c
  https://raw.githubusercontent.com/torvalds/linux/master/arch/sh/kernel/cpu/sh4/fpu.c
"""
from __future__ import annotations

import argparse
import json
import struct
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Iterator

DEFAULT_BASE = 0x8C010000


def sx(value: int, bits: int) -> int:
    return (value ^ (1 << (bits - 1))) - (1 << (bits - 1))


@dataclass(frozen=True)
class Instruction:
    pc: int
    word: int
    mnemonic: str
    operands: str = ""
    kind: str = "normal"
    target: int | None = None
    delay_slot: bool = False
    conditional: bool = False
    literal_address: int | None = None
    literal_value: int | None = None
    literal_size: int = 0
    reads: tuple[str, ...] = ()
    writes: tuple[str, ...] = ()
    indirect_register: str | None = None
    immediate: int | None = None
    note: str = ""

    @property
    def known(self) -> bool:
        return self.kind != "unknown"

    @property
    def text(self) -> str:
        return self.mnemonic + (" " + self.operands if self.operands else "")

    def to_dict(self) -> dict:
        return {**asdict(self), "text": self.text, "known": self.known}


def decode(word: int, pc: int, blob: bytes | None = None,
           base: int = DEFAULT_BASE, fpscr: int | None = None) -> Instruction:
    """Decode one 16-bit instruction; pc and metadata addresses are virtual.

    literal_value is signed for MOV.W and unsigned for MOV.L. A missing or
    out-of-range blob gives literal_address but no literal_value. A value does
    not establish that a PC-relative access lies in executable code.
    """
    if not 0 <= word <= 0xFFFF:
        raise ValueError("word must be a 16-bit unsigned integer")
    n, m, lo, top = (word >> 8) & 15, (word >> 4) & 15, word & 15, word >> 12
    rn, rm = f"r{n}", f"r{m}"

    def ins(mn: str, ops: str = "", **kw) -> Instruction:
        return Instruction(pc, word, mn, ops, **kw)

    def rr(mn: str, dest_reads: bool = False, writes: bool = True) -> Instruction:
        return ins(mn, f"{rm},{rn}", reads=(rm, rn) if dest_reads else (rm,),
                   writes=(rn,) if writes else ())

    def lit(mn: str, address: int, size: int, dest: str, value_is_address=False):
        value = None
        if value_is_address:
            value = address
        elif blob is not None and 0 <= address - base <= len(blob) - size:
            value = int.from_bytes(blob[address-base:address-base+size], "little",
                                   signed=(size == 2))
        return ins(mn, f"@(0x{address:08X},pc),{dest}", literal_address=address,
                   literal_value=value, literal_size=size, writes=(dest,),
                   note="PC-relative calculation assumes this is not a delay-slot instruction")

    exact = {
        0x0008: "clrt", 0x0009: "nop", 0x0018: "sett", 0x0019: "div0u",
        0x0028: "clrmac", 0x0048: "clrs", 0x0058: "sets", 0x001B: "sleep",
        0xFBFD: "frchg", 0xF3FD: "fschg",
    }
    if word == 0x000B:
        return ins("rts", kind="return", delay_slot=True, reads=("pr",))
    if word == 0x002B:
        return ins("rte", kind="return", delay_slot=True, reads=("spc", "ssr"))
    if word == 0x0038:
        return ins("ldtlb")
    if word in exact:
        return ins(exact[word])

    if top == 0:
        if word & 0xF0FF in (0x0003, 0x0023):
            call = (word & 0xFF) == 3
            return ins("bsrf" if call else "braf", rn,
                       kind="call" if call else "branch", delay_slot=True,
                       reads=(rn,), writes=("pr",) if call else (),
                       indirect_register=rn, note="target = pc + 4 + register")
        if word & 0xF0FF == 0x0029:
            return ins("movt", rn, reads=("t",), writes=(rn,))
        caches = {0x83: "pref", 0x93: "ocbi", 0xA3: "ocbp", 0xB3: "ocbwb"}
        if word & 0xFF in caches:
            return ins(caches[word & 0xFF], "@" + rn, reads=(rn,))
        if word & 0xF0FF == 0x00C3:
            return ins("movca.l", f"r0,@{rn}", reads=("r0", rn))
        sysreg = {0x02: "sr", 0x12: "gbr", 0x22: "vbr", 0x32: "ssr",
                  0x42: "spc", 0x3A: "sgr", 0xFA: "dbr"}
        special = {0x0A: "mach", 0x1A: "macl", 0x2A: "pr", 0x5A: "fpul", 0x6A: "fpscr"}
        if word & 0xFF in sysreg:
            reg = sysreg[word & 0xFF]
            return ins("stc", f"{reg},{rn}", reads=(reg,), writes=(rn,))
        if word & 0xFF in special:
            reg = special[word & 0xFF]
            return ins("sts", f"{reg},{rn}", reads=(reg,), writes=(rn,))
        if lo == 2 and m >= 8:
            reg = f"r{m-8}_bank"
            return ins("stc", f"{reg},{rn}", reads=(reg,), writes=(rn,))
        if lo in (4, 5, 6):
            return ins("mov." + "bwl"[lo-4], f"{rm},@(r0,{rn})", reads=(rm, "r0", rn))
        if lo in (12, 13, 14):
            return ins("mov." + "bwl"[lo-12], f"@(r0,{rm}),{rn}", reads=("r0", rm), writes=(rn,))
        if lo == 7:
            return ins("mul.l", f"{rm},{rn}", reads=(rm, rn), writes=("macl",))
        if lo == 15:
            return ins("mac.l", f"@{rm}+,@{rn}+", reads=(rm, rn, "mach", "macl"),
                       writes=(rm, rn, "mach", "macl"))
    elif top == 1:
        return ins("mov.l", f"{rm},@({lo*4},{rn})", reads=(rm, rn))
    elif top == 2:
        if lo in (0, 1, 2, 4, 5, 6):
            size = lo & 3
            return ins("mov." + "bwl"[size], f"{rm},@{'-' if lo >= 4 else ''}{rn}",
                       reads=(rm, rn), writes=(rn,) if lo >= 4 else ())
        names = {7: "div0s", 8: "tst", 9: "and", 10: "xor", 11: "or", 12: "cmp/str", 13: "xtrct"}
        if lo in names:
            test = lo in (7, 8, 12)
            return ins(names[lo], f"{rm},{rn}", reads=(rm, rn), writes=("t",) if test else (rn,))
        if lo in (14, 15):
            return ins("mulu.w" if lo == 14 else "muls.w", f"{rm},{rn}", reads=(rm, rn), writes=("macl",))
    elif top == 3:
        names = {0: "cmp/eq", 2: "cmp/hs", 3: "cmp/ge", 4: "div1", 5: "dmulu.l",
                 6: "cmp/hi", 7: "cmp/gt", 8: "sub", 10: "subc", 11: "subv",
                 12: "add", 13: "dmuls.l", 14: "addc", 15: "addv"}
        if lo in names:
            wr = ("t",) if lo in (0, 2, 3, 6, 7) else (("mach", "macl") if lo in (5, 13) else (rn,))
            if lo in (4, 10, 11, 14, 15):
                wr += ("t",)
            return ins(names[lo], f"{rm},{rn}", reads=(rm, rn), writes=wr)
    elif top == 4:
        low = word & 0xFF
        if low in (0x0B, 0x2B):
            call = low == 0x0B
            return ins("jsr" if call else "jmp", "@" + rn, kind="call" if call else "branch",
                       delay_slot=True, reads=(rn,), writes=("pr",) if call else (), indirect_register=rn)
        unary = {0x00: "shll", 0x01: "shlr", 0x04: "rotl", 0x05: "rotr",
                 0x08: "shll2", 0x09: "shlr2", 0x10: "dt", 0x11: "cmp/pz",
                 0x15: "cmp/pl", 0x18: "shll8", 0x19: "shlr8", 0x20: "shal",
                 0x21: "shar", 0x24: "rotcl", 0x25: "rotcr", 0x28: "shll16", 0x29: "shlr16"}
        if low in unary:
            test = low in (0x11, 0x15)
            change_t = low in (0, 1, 4, 5, 0x10, 0x11, 0x15, 0x20, 0x21, 0x24, 0x25)
            return ins(unary[low], rn, reads=(rn,), writes=((rn,) if not test else ()) + (("t",) if change_t else ()))
        if low == 0x1B:
            return ins("tas.b", "@" + rn, reads=(rn,), writes=("t",))
        sysreg = {0x0E: "sr", 0x1E: "gbr", 0x2E: "vbr", 0x3E: "ssr", 0x4E: "spc", 0x3A: "sgr", 0xFA: "dbr"}
        special = {0x0A: "mach", 0x1A: "macl", 0x2A: "pr", 0x5A: "fpul", 0x6A: "fpscr"}
        if low in sysreg or low in special:
            reg = (sysreg if low in sysreg else special)[low]
            return ins("ldc" if low in sysreg else "lds", f"{rn},{reg}", reads=(rn,), writes=(reg,))
        loadsys = {0x07: "sr", 0x17: "gbr", 0x27: "vbr", 0x37: "ssr", 0x47: "spc", 0x36: "sgr", 0xF6: "dbr"}
        loadsp = {0x06: "mach", 0x16: "macl", 0x26: "pr", 0x56: "fpul", 0x66: "fpscr"}
        storesys = {0x03: "sr", 0x13: "gbr", 0x23: "vbr", 0x33: "ssr", 0x43: "spc", 0x32: "sgr", 0xF2: "dbr"}
        storesp = {0x02: "mach", 0x12: "macl", 0x22: "pr", 0x52: "fpul", 0x62: "fpscr"}
        if low in loadsys or low in loadsp:
            reg = (loadsys if low in loadsys else loadsp)[low]
            return ins("ldc.l" if low in loadsys else "lds.l", f"@{rn}+,{reg}", reads=(rn,), writes=(rn, reg))
        if low in storesys or low in storesp:
            reg = (storesys if low in storesys else storesp)[low]
            return ins("stc.l" if low in storesys else "sts.l", f"{reg},@-{rn}", reads=(rn, reg), writes=(rn,))
        if m >= 8 and lo in (3, 7, 14):
            reg = f"r{m-8}_bank"
            if lo == 3:
                return ins("stc.l", f"{reg},@-{rn}", reads=(reg, rn), writes=(rn,))
            if lo == 7:
                return ins("ldc.l", f"@{rn}+,{reg}", reads=(rn,), writes=(rn, reg))
            return ins("ldc", f"{rn},{reg}", reads=(rn,), writes=(reg,))
        if lo in (12, 13):
            return rr("shad" if lo == 12 else "shld", True)
        if lo == 15:
            return ins("mac.w", f"@{rm}+,@{rn}+", reads=(rm, rn, "mach", "macl"), writes=(rm, rn, "mach", "macl"))
    elif top == 5:
        return ins("mov.l", f"@({lo*4},{rm}),{rn}", reads=(rm,), writes=(rn,))
    elif top == 6:
        if lo in (0, 1, 2, 4, 5, 6):
            return ins("mov." + "bwl"[lo & 3], f"@{rm}{'+' if lo >= 4 else ''},{rn}",
                       reads=(rm,), writes=(rn, rm) if lo >= 4 and m != n else (rn,))
        if lo == 3:
            return rr("mov")
        names = {7: "not", 8: "swap.b", 9: "swap.w", 10: "negc", 11: "neg",
                 12: "extu.b", 13: "extu.w", 14: "exts.b", 15: "exts.w"}
        if lo in names:
            return ins(names[lo], f"{rm},{rn}", reads=(rm,), writes=(rn, "t") if lo == 10 else (rn,))
    elif top == 7:
        imm = sx(word & 255, 8)
        return ins("add", f"#{imm},{rn}", immediate=imm, reads=(rn,), writes=(rn,))
    elif top == 8:
        hi = (word >> 8) & 15
        if hi in (0, 1):
            return ins("mov." + "bw"[hi], f"r0,@({lo*(hi+1)},{rm})", reads=("r0", rm))
        if hi in (4, 5):
            return ins("mov." + "bw"[hi-4], f"@({lo*(hi-3)},{rm}),r0", reads=(rm,), writes=("r0",))
        if hi == 8:
            imm = sx(word & 255, 8)
            return ins("cmp/eq", f"#{imm},r0", immediate=imm, reads=("r0",), writes=("t",))
        if hi in (9, 11, 13, 15):
            target = (pc + 4 + sx(word & 255, 8) * 2) & 0xFFFFFFFF
            return ins({9: "bt", 11: "bf", 13: "bt/s", 15: "bf/s"}[hi], f"0x{target:08X}",
                       kind="branch", target=target, conditional=True, delay_slot=hi >= 13, reads=("t",))
    elif top == 9:
        return lit("mov.w", (pc + 4 + (word & 255)*2) & 0xFFFFFFFF, 2, rn)
    elif top in (10, 11):
        target = (pc + 4 + sx(word & 4095, 12)*2) & 0xFFFFFFFF
        return ins("bra" if top == 10 else "bsr", f"0x{target:08X}", target=target,
                   kind="branch" if top == 10 else "call", delay_slot=True, writes=("pr",) if top == 11 else ())
    elif top == 12:
        sub, imm = (word >> 8) & 15, word & 255
        if sub in (0, 1, 2):
            return ins("mov." + "bwl"[sub], f"r0,@({imm*(1 << sub)},gbr)", reads=("r0", "gbr"))
        if sub == 3:
            return ins("trapa", f"#{imm}", kind="trap", immediate=imm)
        if sub in (4, 5, 6):
            return ins("mov." + "bwl"[sub-4], f"@({imm*(1 << (sub-4))},gbr),r0", reads=("gbr",), writes=("r0",))
        if sub == 7:
            return lit("mova", ((pc+4) & ~3) + imm*4, 0, "r0", True)
        if sub >= 8:
            mn = ("tst", "and", "xor", "or")[sub & 3] + (".b" if sub >= 12 else "")
            return ins(mn, f"#{imm}," + ("@(r0,gbr)" if sub >= 12 else "r0"), immediate=imm,
                       reads=("r0", "gbr") if sub >= 12 else ("r0",),
                       writes=("t",) if sub & 3 == 0 else (() if sub >= 12 else ("r0",)))
    elif top == 13:
        return lit("mov.l", (((pc+4) & ~3) + (word & 255)*4) & 0xFFFFFFFF, 4, rn)
    elif top == 14:
        imm = sx(word & 255, 8)
        return ins("mov", f"#{imm},{rn}", immediate=imm, writes=(rn,))
    elif top == 15:
        note = "FR fields shown; effective FPU register width/bank depends on FPSCR.PR/SZ/FR" if fpscr is None else ""
        pr = bool(fpscr is not None and fpscr & (1 << 19))
        sz = bool(fpscr is not None and fpscr & (1 << 20))
        fn, fm = f"fr{n}", f"fr{m}"
        def f_reg(field, double):
            return (f"xd{field & ~1}" if field & 1 else f"dr{field}") if double else f"fr{field}"
        if lo <= 5:
            if pr and (n & 1 or m & 1):
                return ins(".word", f"0x{word:04X}", kind="unknown", note="odd register field invalid in double arithmetic mode")
            a, b = f_reg(m, pr), f_reg(n, pr)
            return ins(("fadd", "fsub", "fmul", "fdiv", "fcmp/eq", "fcmp/gt")[lo], f"{a},{b}",
                       reads=(a, b), writes=(b,) if lo < 4 else ("t",), note=note)
        if lo in (6, 8, 9):
            fn = f_reg(n, sz)
            src = f"@(r0,{rm})" if lo == 6 else f"@{rm}{'+' if lo == 9 else ''}"
            return ins("fmov", f"{src},{fn}", reads=(rm, "r0") if lo == 6 else (rm,),
                       writes=(fn, rm) if lo == 9 else (fn,), note=note)
        if lo in (7, 10, 11):
            fm = f_reg(m, sz)
            dest = f"@(r0,{rn})" if lo == 7 else f"@{'-' if lo == 11 else ''}{rn}"
            return ins("fmov", f"{fm},{dest}", reads=(fm, rn, "r0") if lo == 7 else (fm, rn),
                       writes=(rn,) if lo == 11 else (), note=note)
        if lo == 12:
            return ins("fmov", f"{f_reg(m,sz)},{f_reg(n,sz)}", reads=(f_reg(m,sz),), writes=(f_reg(n,sz),), note=note)
        if lo == 14:
            return ins("fmac", f"fr0,{fm},{fn}", reads=("fr0", fm, fn), writes=(fn,), note=note)
        if lo == 13:
            if m in (0, 1, 2, 3):
                names = ("fsts", "flds", "float", "ftrc")
                reg = f_reg(n, pr and m in (2, 3))
                if pr and m in (2, 3) and n & 1:
                    return ins(".word", f"0x{word:04X}", kind="unknown", note="odd double register field")
                to_fpul = m in (1, 3)
                return ins(names[m], f"{reg},fpul" if to_fpul else f"fpul,{reg}",
                           reads=(reg,) if to_fpul else ("fpul",), writes=("fpul",) if to_fpul else (reg,), note=note)
            if m in (4, 5, 6, 7, 8, 9):
                names = {4: "fneg", 5: "fabs", 6: "fsqrt", 7: "fsrra", 8: "fldi0", 9: "fldi1"}
                reg = f_reg(n, pr and m in (4, 5, 6))
                return ins(names[m], reg, reads=(reg,) if m < 8 else (), writes=(reg,), note=note)
            if m in (10, 11) and not n & 1:
                reg = f"dr{n}"
                return ins("fcnvsd" if m == 10 else "fcnvds", f"fpul,{reg}" if m == 10 else f"{reg},fpul",
                           reads=("fpul",) if m == 10 else (reg,), writes=(reg,) if m == 10 else ("fpul",))
            if m == 14:
                src, dst = f"fv{(n & 3)*4}", f"fv{(n >> 2)*4}"
                return ins("fipr", f"{src},{dst}", reads=(src, dst), writes=(f"fr{(n >> 2)*4+3}",), note=note)
            if m == 15 and n & 3 == 1:
                dst = f"fv{(n >> 2)*4}"
                return ins("ftrv", f"xmtrx,{dst}", reads=("xmtrx", dst), writes=(dst,), note=note)
            if m == 15 and not n & 1:
                return ins("fsca", f"fpul,dr{n}", reads=("fpul",), writes=(f"fr{n}", f"fr{n+1}"), note=note)
    return ins(".word", f"0x{word:04X}", kind="unknown", note="unimplemented or invalid SH-4 encoding")


def disassemble(blob: bytes, start: int = 0, end: int | None = None,
                base: int = DEFAULT_BASE, fpscr: int | None = None) -> Iterator[Instruction]:
    """Linear sweep; start/end are file offsets. This does not discover code."""
    if start & 1:
        raise ValueError("start offset must be even")
    end = len(blob) if end is None else min(end, len(blob))
    for off in range(start, end - 1, 2):
        yield decode(struct.unpack_from("<H", blob, off)[0], base+off, blob, base, fpscr)


def literal_xrefs(blob: bytes, start: int = 0, end: int | None = None,
                  base: int = DEFAULT_BASE) -> list[dict]:
    """Linear candidates, including false positives in data. Filter using CFG."""
    return [i.to_dict() for i in disassemble(blob, start, end, base) if i.literal_address is not None]


def direct_calls(blob: bytes, start: int = 0, end: int | None = None,
                 base: int = DEFAULT_BASE) -> list[dict]:
    return [i.to_dict() for i in disassemble(blob, start, end, base) if i.kind == "call" and i.target is not None]


def walk_cfg(blob: bytes, entry: int, base: int = DEFAULT_BASE,
             max_instructions: int = 4096, lower: int | None = None,
             upper: int | None = None) -> dict:
    """Conservative intraprocedural reachability, entry/lower/upper are VAs.

    Follows direct branch targets and call fallthroughs, not called functions or
    indirect jumps. Delayed transfers include the slot before transfer edges.
    Edges are conceptual transfer edges from the branch after slot execution;
    slot instruction nodes are marked in delay_slots. Unknown stops a path.
    Does not infer function extent from a final RTS or linearly consume pools.
    """
    low = base if lower is None else max(base, lower)
    high = base + len(blob) if upper is None else min(base + len(blob), upper)
    pending, seen, instructions, edges, slots, warnings = [entry], set(), {}, [], set(), []
    def allowed(pc):
        return pc & 1 == 0 and low <= pc <= high - 2
    while pending and len(instructions) < max_instructions:
        pc = pending.pop()
        if pc in seen or not allowed(pc):
            continue
        seen.add(pc)
        i = decode(struct.unpack_from("<H", blob, pc-base)[0], pc, blob, base)
        instructions[pc] = i
        if not i.known:
            warnings.append(f"unknown instruction stops path at 0x{pc:08X}")
            continue
        if i.delay_slot:
            spc = pc + 2
            if allowed(spc):
                slot = decode(struct.unpack_from("<H", blob, spc-base)[0], spc, blob, base)
                instructions[spc] = slot
                slots.add(spc)
                if not slot.known:
                    warnings.append(f"unknown delay-slot instruction stops path at 0x{spc:08X}")
                    continue
                if slot.kind in ("branch", "call", "return", "trap"):
                    warnings.append(f"control transfer in delay slot at 0x{spc:08X}; invalid/unsupported path")
                    continue
                if slot.literal_address is not None:
                    warnings.append(f"PC-relative instruction in delay slot at 0x{spc:08X}; operand requires branch context")
            else:
                warnings.append(f"delay slot outside bounds at 0x{spc:08X}")
        dests = []
        fall = pc + (4 if i.delay_slot else 2)
        if i.kind == "branch":
            if i.target is not None:
                dests.append((i.target, "taken"))
            else:
                warnings.append(f"indirect branch unresolved at 0x{pc:08X}")
            if i.conditional:
                dests.append((fall, "fallthrough"))
        elif i.kind == "call":
            edges.append({"from": pc, "to": i.target, "kind": "call", "indirect_register": i.indirect_register})
            dests.append((fall, "return_fallthrough"))
        elif i.kind not in ("return", "trap"):
            dests.append((fall, "fallthrough"))
        for dst, kind in dests:
            edges.append({"from": pc, "to": dst, "kind": kind})
            if allowed(dst) and dst not in seen:
                pending.append(dst)
    return {"entry": entry, "base": base, "bounds": [low, high],
            "instructions": [instructions[a].to_dict() for a in sorted(instructions)],
            "edges": edges, "delay_slots": sorted(slots), "warnings": warnings,
            "truncated": bool(pending), "note": "Static candidates only; indirect jumps, jump tables and function boundaries require review."}


def linear_indirect_xrefs(blob: bytes, start: int = 0, end: int | None = None,
                          base: int = DEFAULT_BASE) -> list[dict]:
    """Candidate indirect transfers resolved from a short straight-line trace.

    Tracks literal/constant MOV, register MOV and ADD immediate only. Invalidates
    all state on control transfers/unknowns; never propagates across a call or
    delay slot. Like literal_xrefs, a linear sweep can decode data as code.
    Evidence returned is a candidate, requiring a reviewed CFG/code range.
    """
    values, evidence, results, skip_pc = {}, {}, [], None
    for i in disassemble(blob, start, end, base):
        if i.pc == skip_pc:
            skip_pc = None
            continue
        if i.indirect_register:
            reg = i.indirect_register
            if reg in values:
                value = values[reg]
                target = (i.pc + 4 + value) & 0xFFFFFFFF if i.mnemonic in ("braf", "bsrf") else value
                results.append({"pc": i.pc, "target": target, "instruction": i.text,
                                "register": reg, "evidence_pcs": evidence[reg],
                                "confidence": "straight-line constant candidate; code reachability not established"})
        if i.kind != "normal":
            values.clear()
            evidence.clear()
            if i.delay_slot:
                skip_pc = i.pc + 2
            continue
        old_values, old_evidence = values.copy(), evidence.copy()
        for reg in i.writes:
            values.pop(reg, None)
            evidence.pop(reg, None)
        n, m = (i.word >> 8) & 15, (i.word >> 4) & 15
        rn, rm = f"r{n}", f"r{m}"
        if i.literal_value is not None and i.writes:
            reg = i.writes[0]
            values[reg], evidence[reg] = i.literal_value & 0xFFFFFFFF, [i.pc]
        elif i.mnemonic == "mov" and i.immediate is not None:
            values[rn], evidence[rn] = i.immediate & 0xFFFFFFFF, [i.pc]
        elif i.word & 0xF00F == 0x6003 and rm in old_values:
            values[rn], evidence[rn] = old_values[rm], old_evidence[rm] + [i.pc]
        elif i.mnemonic == "add" and i.immediate is not None and rn in old_values:
            values[rn], evidence[rn] = (old_values[rn] + i.immediate) & 0xFFFFFFFF, old_evidence[rn] + [i.pc]
    return results


def selftest() -> dict:
    checks = []
    def check(label, condition):
        if not condition:
            raise AssertionError(label)
        checks.append(label)
    data = bytearray(0x30)
    struct.pack_into("<I", data, 0x2C, 0x8C0D2480)
    i = decode(0xD004, DEFAULT_BASE+0x1A, data)
    check("startup MOV.L D004 aligns pc+4 before displacement", i.literal_address == DEFAULT_BASE+0x2C and i.literal_value == 0x8C0D2480 and i.writes == ("r0",))
    j = decode(0x402B, DEFAULT_BASE+0x1C)
    check("startup JMP @R0 has delay slot", j.text == "jmp @r0" and j.delay_slot and j.indirect_register == "r0")
    check("negative BRA displacement sign extension", decode(0xAFFE, 0x1000).target == 0x1000)
    check("negative BT displacement sign extension", decode(0x89FE, 0x1000).target == 0x1000)
    check("BT non-delayed versus BT/S delayed", not decode(0x8900, 0).delay_slot and decode(0x8D00, 0).delay_slot)
    check("FADD source M destination N fields", decode(0xF230, 0).text == "fadd fr3,fr2")
    check("FMOV memory source uses integer M destination float N", decode(0xF238, 0).text == "fmov @r3,fr2")
    check("FMOV store source float M destination integer N", decode(0xF23A, 0).text == "fmov fr3,@r2")
    check("FMAC includes fr0, source M, destination N", decode(0xF23E, 0).text == "fmac fr0,fr3,fr2")
    check("FTRV vector destination field", decode(0xF5FD, 0).text == "ftrv xmtrx,fv4")
    check("FIPR vector fields", decode(0xF6ED, 0).text == "fipr fv8,fv4")
    check("SH4 invalid encoding retained", not decode(0xFFFF, 0).known)
    check("MOV.W sign extension", decode(0x9000, 0x1000, b"\0\0\0\0\xff\xff", 0x1000).literal_value == -1)
    test_blob = struct.pack("<6H", 0x8901, 0x0009, 0x000B, 0x0009, 0xFFFF, 0xFFFF)
    cfg = walk_cfg(test_blob, 0x1000, base=0x1000)
    check("CFG follows both conditional paths and includes RTS delay slot", cfg["delay_slots"] == [0x1006] and any(e["to"] == 0x1006 for e in cfg["edges"]))
    trace = struct.pack("<4H", 0xD001, 0x402B, 0x0009, 0x0009) + struct.pack("<I", 0x12345678)
    check("literal-backed indirect candidate resolves before slot", linear_indirect_xrefs(trace, base=0x1000)[0]["target"] == 0x12345678)
    unknown_slot = struct.pack("<4H", 0xA001, 0xFFFF, 0x0009, 0x0009)
    check("unknown delay slot stops conservative CFG path", len(walk_cfg(unknown_slot, 0x1000, base=0x1000)["instructions"]) == 2)
    return {"passed": len(checks), "checks": checks}


def number(value):
    return int(value, 0)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("binary", nargs="?")
    parser.add_argument("--base", type=number, default=DEFAULT_BASE)
    parser.add_argument("--start", type=number, default=0, help="file offset, default 0")
    parser.add_argument("--end", type=number, help="exclusive file offset")
    parser.add_argument("--count", type=number, default=64, help="default linear instruction limit")
    parser.add_argument("--cfg", type=number, help="entry virtual address; JSON conservative CFG")
    parser.add_argument("--json", action="store_true")
    parser.add_argument("--selftest", action="store_true")
    args = parser.parse_args()
    if args.selftest:
        print(json.dumps(selftest(), indent=2))
        return
    if not args.binary:
        parser.error("binary or --selftest required")
    blob = Path(args.binary).read_bytes()
    if args.cfg is not None:
        print(json.dumps(walk_cfg(blob, args.cfg, args.base, args.count,
                                  args.base+args.start, args.base+args.end if args.end is not None else None), indent=2))
    else:
        end = args.end if args.end is not None else args.start+args.count*2
        items = list(disassemble(blob, args.start, end, args.base))
        if args.json:
            print(json.dumps([i.to_dict() for i in items], indent=2))
        else:
            for i in items:
                comment = ""
                if i.literal_value is not None:
                    comment += f" ; value={i.literal_value:#x}"
                if i.delay_slot:
                    comment += " ; delay slot follows"
                print(f"{i.pc:08X} {i.word:04X}  {i.text}{comment}")


if __name__ == "__main__":
    main()
