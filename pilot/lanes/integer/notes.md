# Integer pilot mapping — frozen static pass

Target SHA-256: `336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688`.
Base: `0x8C010000`. All offsets/ranges are exclusive at the end.

| Pilot | VA / file range | Code bytes | Static conclusion |
|---|---|---:|---|
| `bsearch` training | `0x8C09BC04`; `0x8BC04–0x8BC7A` | 118 | Bsearch API counterpart, but different library/source algorithm from MAT3 |
| `InsertLLE` training | `0x8C03A1C8`; `0x2A1C8–0x2A28E` | 198 | All mode branches, field offsets and writes agree with MAT3 |
| `interp` training | `0x8C0231D2`; `0x131D2–0x131E6` | 20 | Exact arithmetic expression identified; original DC return type remains unresolved |
| `enginetorque` held out | `0x8C0230B0`; `0x130B0–0x131D2` | 290 | Strong identification from clamps, signed16 curve indexing, inline interpolation and scale; no held-out tuning |

These four nonoverlapping code spans total **626 bytes**. This is an identification denominator, with **zero recovered-C/exact emission/local-link bytes claimed**. Each CFG traverses its whole code span with no unknown instruction or truncation. The decoder is custom and still needs independent analyzer/compiler corroboration. `mapping.json` contains hashes, source pins, scores, references, ABI/layout observations, drift and alternatives. Per-function `.cfg.json` and `.disasm.txt` are the reproducible traces.

## Shared literals and return widths

`interp` calls the signed32 division helper at `0x8C09B680` using numerator `R1`, denominator `R0`, quotient `R0`. The helper has signed `DIV0S` setup and 32 `DIV1` steps; it saves/restores `R2/R3`. Its pointer at file `0x13228–0x1322C` is shared by torque and `interp`, and must be owned once by the integrator. Torque additionally uses signed16 constants `1150`, `1149` at `0x1321C–0x13220` and helper/table pointers through `0x1323C`. The instruction decoder's linear view of these pools is not code evidence.

The standalone `interp` sequence is `b-a`, low32 multiply by `rem`, signed divide by `total`, then add `a` in the RTS delay slot. It does **not** sign-extend the result to16 bits. No out-of-line direct call or static pointer reference was found. The same expression is inlined three times in torque, where intermediates are narrowed through signed16 stores/loads. A candidate may use `a + rem*(b-a)/total` for arithmetic in its defined C range, but retain two validation outputs: raw32 result and signed16 projection. The MAT3 signed-short declaration alone cannot settle the original DC source declaration.

Torque also returns a full integer register without narrowing; its `engine` caller explicitly `EXTS.W R0,R0` at `0x8C023210` and `0x8C023288`. This establishes the actual low16 caller contract for torque. Its exact final FP order is scale-table value times tune scale, then converted signedshort interpolation result times that product, then `FTRC`; FPSCR mode remains to be established.

## Binary-search source drift

The DC routine uses one-based unsigned bounds:

```c
lower = 1;
upper = nmemb;
while (lower >= 1 && lower <= upper) {
    mid = lower + ((upper - lower) >> 1);
    current = base + (mid - 1) * size;
    result = compar(key, current);
    if (result < 0) upper = mid - 1;
    if (result > 0) lower = mid + 1;
    if (result == 0) return current;
}
return 0;
```

This pseudocode is a traced source candidate, not compiled or equivalent-C acceptance. Pointer arithmetic must be32-bit for the target contract. The fifth argument comparator is loaded from entry `SP+0` (`SP+40` after its prologue) and called indirectly. The DC body lacks MAT3's `size==0` return, end-limit calculation and final fallback comparator; comparator call order/duplicate selection can differ. In particular, a zero-size, nonempty search can return base in DC where MAT3 immediately returns null. That is concrete library/source drift, not a compiler flag problem.

Independent callers support the API name: object lookup at `0x8C03D424` uses a72-byte element and model loop/name key, while `FindWorld` at `0x8C065DEC` uses20-byte records and a comparator comparing names at+4. These match MAT3 `mbox.c::MBOX_FindObject_WORK` and `world.c::FindWorld` contracts. A full search for another custom MAT3-like implementation was not performed; no alternative was found in the bounded checks.

## List layout and controls

DC `InsertLLE` uses list flags at byte0/1, count at+4, head+8, tail+12, node next+0/prev+4. It preserves `R14`, makes no calls, and needs no literal pool. All handle/direct and singly/doubly linked paths match the MAT3 body, including continuing traversal after a singly-linked replacement rather than inventing an early break. Adjacent `InitLL` at `0x8C03A1B8` falls through to `ClearLL` at `0x8C03A1BE`, and the inverse `RemoveLLE` at `0x8C03A28E` corroborates TU order and layout. `RemoveLLE` is a strong negative control because it decrements rather than increments the count.

Identification scores are prioritization annotations, not probabilities. The report records unresolved runner-up searches without inventing a numeric score/lead. Root review should distinguish an accepted semantic identity from a historical original-source identity, particularly for `bsearch` and `interp` return width.

## Suggested focused checks before compilation

- `interp`: signed inputs/remainders and exact truncating division; separate raw32 and signed16 outputs. Avoid source UB, zero divisors and `INT_MIN/-1` until their target behavior is specified.
- `InsertLLE`: all four flag combinations, null insertion, head/middle/tail insertion, complete next/prev graph and count. Run mutation controls on node offsets and count update.
- `bsearch`: empty/found/missing/duplicate/zero-size cases and comparator trace. The MAT3 body should deliberately fail the DC zero-size trace as a drift control.
- Held-out torque: keep identification facts frozen; no compiler configuration tuning or source-shape tuning was performed here.

To regenerate only lane reports: run `python lanes/integer/build_evidence.py` from this pilot workspace. It reads the pinned target and three MAT3 C files, imports the shared static decoder, and writes only this lane. The exploratory `minimal_decode.py`, `search.py`, `list_search.py`, and `xref.py` are bounded static search helpers, not matching tools or an authoritative disassembler. No game/toolchain executables were run and no MAT3/original file was edited.
