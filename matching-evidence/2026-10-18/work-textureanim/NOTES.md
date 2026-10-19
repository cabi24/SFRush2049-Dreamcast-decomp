# Texture animation matching trial, 2026-10-18

RESULT: parked after three evidence-backed source shapes; no improvement retained.

Baseline and restored source SHA256: c57cc6f2e815534e0397c0c13df28a4ec25c8e8fe81b4a1485873133b9f90f67.
Baseline private native fixed-address link retains TextureAnimation exact (420-byte range, 418 explicitly emitted credited bytes), InitTextureAnimation differs in three bytes. Exact delta: zero functions, zero credited bytes.

Residual is only register selection at the initial plist->name test:
- 8C05B30A: ours 62E2 mov.l @r14,r2; target 63E2 mov.l @r14,r3.
- 8C05B310: ours 2228 tst r2,r2; target 2338 tst r3,r3.
All other bytes of InitTextureAnimation match. Full decoded baseline listing is baseline/listing.txt; the existing pilot/tools/sh4.py decoder was used, with no masks and no dependency installation.

Three independent shapes (each based on baseline):
1. MAT3 direct plist->ptr assignment without ttDef: initializer shrinks 2 bytes; 150 differing initializer bytes and subsequent function moves -2. Rejected.
2. void *ttDef to agree with proved destination ptr type: identical linked baseline output, same 3 differing bytes. Rejected.
3. Cache tested name in explicit char * local and pass same pointer into lookup: initializer shrinks 6 bytes, 185 differing initializer bytes; following function moves -6. Rejected. Target reloads name for lookup, so caching is disproved by the output.

Each variant has source.c, result.json, pp compiler input, raw object and native abs/map/json link packet in its own directory. No shared build outputs, report, map, header, externs, objdiff, receipt or handoff changed. Native SHC5.1r11 and frozen flags via dc_build.build_unit; LNK6.0E through existing shc_link.py. run.py overrides only output directories and calls build_unit directly (no split/main/report/objdiff writes).

Reproduce baseline: python -B matching-evidence/2026-10-18/work-textureanim/run.py baseline
Generate baseline listing: python -B matching-evidence/2026-10-18/work-textureanim/list.py

Source restored byte-for-byte from baseline/source.c after trial 3. Stop rule reached. No full build, game execution, install, download, commit or push.
