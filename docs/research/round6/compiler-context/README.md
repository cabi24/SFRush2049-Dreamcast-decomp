# Bounded compiler-context control

**Finding:** with frozen SHC5.1r11 flags and default LNK6.0E, linking a real nearby callee object did not relax the external call in this control. The separate-object caller is byte-identical to a DEFINE-only mini-link at the same callee address. Direct-call selection occurred during same-TU compilation.

| Arrangement | Callee address | Caller call form | Caller extent |
|---|---|---|---|
| Both real definitions in oneTU |8C010016|Direct PC-relative BSR|22 instruction bytes|
| Caller and real callee in separate objects |8C01001C|Register-indirect JSR|24 instruction bytes+4-byte address literal|
| Same caller object, DEFINE callee=8C01001C |8C01001C|Register-indirect JSR|Same28 bytes, exactly identical|

The external call has a near18-byte PC-relative displacement from call-PC+4. Caller uses the callee result and original argument; `-noinline` was unchanged. Sources are tiny ordinary C with no padding, forced branches, volatile state or game edits. The synthetic callee is real code in the first two arrangements and deliberately only a symbol definition in the third. No synthetic functions count toward the500-target round.

## Reproduction

Sources:caller.c,callee.c,both.c. `commands.json` records exact flags and LNK subcommands with private path placeholders. `result.json` records tool/input/output hashes and summarized call forms, not raw disassembly. The same unchanged shc_link wrapper was used for each link; it returns1 because unrelated synthetic bytes do not match the retail image. All three native links themselves returned0, emitted ABS output, and had no undefined/duplicate-unit diagnostics.

An initial invalid control through shc_cc.py exposed its fixed dos_src.c module name: LNK deleted the second dos_src unit and left the callee undefined. That result was discarded, not interpreted as call behavior. The valid control launches the same verified SHC binaries with identical flags and distinct source filenames; no production wrapper, flag or linker option was changed.

## Scope

This rules out automatic near-call relaxation by this default LNK path for this small control. It does not prove behavior of every linker mode or source construct. It supports treating same-TU completeness as a real compiler-context requirement rather than expecting a larger default multi-object link to recreate a compiler-selected direct call.

Production source hashes, audited tools, map pin and common header remain unchanged. Work stopped after the requested three controls. Binaries, full maps, native logs and raw instruction dumps remain private outside this publishable directory.
