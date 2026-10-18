"""Word-by-word SH-4 disassembly: capstone for what it knows, a small table for the FPU/system
instructions capstone 5 misses, '??' otherwise. Every SH instruction is one 16-bit word, so this
never desynchronises on data."""
import capstone

_md = capstone.Cs(capstone.CS_ARCH_SH, capstone.CS_MODE_SH4 | capstone.CS_MODE_LITTLE_ENDIAN)

_F_NM = {0x0: 'fadd fr{m},fr{n}', 0x1: 'fsub fr{m},fr{n}', 0x2: 'fmul fr{m},fr{n}', 0x3: 'fdiv fr{m},fr{n}',
         0x4: 'fcmp/eq fr{m},fr{n}', 0x5: 'fcmp/gt fr{m},fr{n}', 0x6: 'fmov.s @(r0,r{m}),fr{n}',
         0x7: 'fmov.s fr{m},@(r0,r{n})', 0x8: 'fmov.s @r{m},fr{n}', 0x9: 'fmov.s @r{m}+,fr{n}',
         0xA: 'fmov.s fr{m},@r{n}', 0xB: 'fmov.s fr{m},@-r{n}', 0xC: 'fmov fr{m},fr{n}', 0xE: 'fmac fr0,fr{m},fr{n}'}
_F_ND = {0x0: 'fsts fpul,fr{n}', 0x1: 'flds fr{n},fpul', 0x2: 'float fpul,fr{n}', 0x3: 'ftrc fr{n},fpul',
         0x4: 'fneg fr{n}', 0x5: 'fabs fr{n}', 0x6: 'fsqrt fr{n}', 0x8: 'fldi0 fr{n}', 0x9: 'fldi1 fr{n}',
         0xA: 'fcnvsd fpul,dr{n}', 0xB: 'fcnvds dr{n},fpul', 0xE: 'fipr fv{m},fv{n}'}


def _fallback(w):
    hi, n, m, lo = w >> 12, (w >> 8) & 15, (w >> 4) & 15, w & 15
    if hi == 0xF:
        if lo == 0xD and m in _F_ND:
            return _F_ND[m].format(n=n, m=m)
        if lo in _F_NM:
            return _F_NM[lo].format(n=n, m=m)
        if w == 0xF3FD:
            return 'fschg'
        if w == 0xFBFD:
            return 'frchg'
    if hi == 0x4 and lo == 0xA and m == 5:
        return f'lds r{n},fpul'
    if hi == 0x4 and lo == 0xA and m == 6:
        return f'lds r{n},fpscr'
    if hi == 0x0 and lo == 0xA and m == 5:
        return f'sts fpul,r{n}'
    if hi == 0x0 and lo == 0xA and m == 6:
        return f'sts fpscr,r{n}'
    if hi == 0x4 and lo == 0x6 and m == 5:
        return f'lds.l @r{n}+,fpul'
    if hi == 0x4 and lo == 0x6 and m == 6:
        return f'lds.l @r{n}+,fpscr'
    if hi == 0x4 and lo == 0x2 and m == 5:
        return f'sts.l fpul,@-r{n}'
    if hi == 0x4 and lo == 0x2 and m == 6:
        return f'sts.l fpscr,@-r{n}'
    return None


def disasm_words(code, addr):
    """Yield (address, word_bytes, text) for every 16-bit word."""
    for i in range(0, len(code) - 1, 2):
        wb = code[i:i + 2]
        w = wb[0] | (wb[1] << 8)
        text = None
        for ins in _md.disasm(wb, addr + i):
            text = f'{ins.mnemonic} {ins.op_str}'.strip()
            break
        if text is None:
            text = _fallback(w)
        yield addr + i, wb, text if text is not None else '??'


def is_pcrel_load(w):
    """mov.w @(disp,pc) 0x9nnn, mov.l @(disp,pc) 0xDnnn, mova 0xC7nn."""
    return (w >> 12) in (0x9, 0xD) or (w >> 8) == 0xC7
