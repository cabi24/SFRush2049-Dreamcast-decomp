# calib — compiler calibration workspace (2026-10-17/18)

Disposable experiment area for the pilot's toolchain calibration. Canonical conclusions are in
`../pilot/lanes/toolchain/calibration.md`; the frozen configuration is `../pilot/config/toolchain.json`.

- `run_calib.py` — compile pilot slots with `../tools/shc_cc.py` and compare against the target spans
  (instruction-exact, literal-pool displacement masked). `-v all` sweeps every package; `--src DIR`
  picks a source set; extra SHC flags after `--`; `--no-default` drops decomp.me's scratch extras.
  Appends one row per compile to `trials.jsonl`; writes `out/<label>/<version>/<slot>.txt` listings
  with a side-by-side diff on mismatch, plus the `.elf` and raw SYSROF `.obj`.
- `src/` first drafts (MAT3 bodies); `src2/` rejected ternary InsertLLE; `src3/` **final source set**
  (interp, bsearch, InsertLLE, BodyVector, `ll_tu.c` whole ll.c unit, enginetorque shape 1,
  UpdateFrameTimer shape 0); `src4/`, `src5/` the other held-out shapes (all recorded in `trials.jsonl`).
- `link/` — `shc_link.py` outputs: `ll_tu.{abs,map,json}` and `BodyVector.{abs,map,json}`, both
  `all_exact: true` at their target addresses.
- `_target_spans.txt`, `_ll_region.txt` capstone dumps; `_ghidra_spans.txt` Ghidra dump of the held-out
  spans and their neighbours (complete FPU decoding).

Frozen recipe for a slot:

```
python ..\tools\shc_cc.py -v shc-v5.1r11 src3\InsertLLE.c -o out\x.elf --no-default-flags -- -speed -noinline
python run_calib.py -v shc-v5.1r11 -s ll_tu --src src3 --label check --no-default -- -speed -noinline
python ..\tools\shc_link.py --obj out\check\shc-v5.1r11\ll_tu.obj --start P=8C03A1B8 --name ll_tu
```

Unit-level files (2026-10-18, continued): `sh4dis.py` (full SH-4 word decoder), `unitcmp.py` (per-function
compare: `python unitcmp.py out/<label>/<ver>/unit.elf sym=TARGETVA:SIZE ...`), `src3/timer_tu.c`
(timer.c unit), `src3/drivetra_part.c` (enginetorque+interp+engine), `src6`–`src12` the declaration,
volatile and inliner variants, `_ghidra_units.txt` / `_ghidra_helpers.txt` Ghidra listings of the two
regions and the division helpers, `link/timer_tu.*` the data-section-exact timer link.
