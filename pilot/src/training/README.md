# Provisional training source

These four scaffolds make the static hypotheses reviewable. None is accepted equivalent C, emitted SH-4 code, or a byte match. The vintage compiler family, ABI and headers are unresolved. C11 assertions and standard integer headers are analysis conveniences; adapt them only after reviewing the acquired toolchain. Host compilation cannot settle target behavior.

- `bsearch.c` follows the DC library algorithm, which differs from MAT3.
- `InsertLLE.c` preserves the observed list offsets and branches; requires 32-bit pointers.
- `interp.c` covers defined signed32 arithmetic only. Raw32 versus signed16 source return type remains open.
- `BodyVector.c` deliberately leaves DC_FMAC unresolved. FPSCR/rounding, exceptional values and code generation require target evidence; source reloads preserve aliasing.

See `../../analysis/source-provenance.json` and the lane reports. No held-out C bodies were produced or tuned.
