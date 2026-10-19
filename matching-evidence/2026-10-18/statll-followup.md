# statll follow-up

Native SHC 5.1r11 / H-series linker, frozen flags, strict fixed-address comparison.
Baseline source emits 206 bytes at 0x8C05138C. GetStatLLE and FreeStatLLE are
exact; InitStatLL differs only at 0x8C051396..0x8C05139A:

| Address | Target word | Baseline word |
|---|---|---|
| 8C051396 | 5643 (mov.l @(12,r4),r6) | E000 (mov #0,r0) |
| 8C051398 | E000 (mov #0,r0) | 5643 (mov.l @(12,r4),r6) |

Three source-order trials were tested with `python tools/dc_build.py --unit statll`:
moving head initialization before the base reload (9 differing bytes), moving
the base reload before the dbl assignment (4 differing bytes, first offset8),
and moving head initialization between base reload and store assignment
(9 differing bytes). All retained206 emitted bytes. Restored the original source
after the three-trial bound. No new recovered bytes or functions are claimed.

The baseline `-v` build completed compilation, linking and report generation,
then failed its optional listing import with `ModuleNotFoundError: No module
named 'capstone'`. Subsequent comparisons omitted `-v`; no packages were installed.
The residual was inspected directly in target and linked SYSROF bytes.
