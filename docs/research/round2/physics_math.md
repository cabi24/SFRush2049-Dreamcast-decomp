# Round 2: physics / math outcomes

Historical bounded experiments, not an instruction to apply unmatched source.
Accepted gains are separately documented in ../../round2-matching.md.
Private diagnostics and raw build artifacts are intentionally omitted.

## 1. sym — negative

Unit: `drivsym`; target: `8C0235FE`. Reported accepted delta: 0 emitted bytes / 0 functions.

- sym-negative-first-abs

**Residual:** Target 8C02367E negative-first branch/neg delay slot motivated inverted abs ternary. Function remains displaced -4; fixed-address differences 679 to 680; no strict gain.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 2. forces2 — negative

Unit: `drivsym`; target: `8C02429E`. Reported accepted delta: 0 emitted bytes / 0 functions.

- forces2-explicit-branches

**Residual:** Native N64 func_800E1C30 uses explicit rear-thrust branches. Converted ternaries to equivalent branches; differences 672 to 680. Target frame20 vs baseline12 and prefix drift remain.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 3. torques — negative

Unit: `drivsym`; target: `8C024598`. Reported accepted delta: 0 emitted bytes / 0 functions.

- torques-explicit-vector-add
- torques-array-order
- torques-counted-loop

**Residual:** Native N64 func_800E15A0 corroborates two live vector temporaries and explicit vector add. Explicit sums increased body1360 to1508; array declaration swap neutral; target loop backedge motivated local count bound, but it regressed antispin/shared pool and was rejected. Three-probe cap reached; first target discrepancy8C0245A0 saved r9 vs r10, frame36 vs32.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 4. InterpQuats — negative

Unit: `fmath`; target: `8C026E20`. Reported accepted delta: 0 emitted bytes / 0 functions.

- interp-delta-lifetime

**Residual:** Target saves one fewer integer register and homes a different component pointer. Widening existing delta local lifetime is neutral (660 fixed-address differing bytes). No new concrete shaping evidence after this probe; NormalVector capped prefix still displaces function16 bytes.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 5. velocities — partial

Unit: `drivsym`; target: `8C0249AC`. Reported accepted delta: 0 emitted bytes / 0 functions.

- velocities-explicit-vector-add
- velocities-comma-vector-add

**Residual:** N64 func_800E114C corroborates explicit V+temp/W+temp form; explicit addition reproduces target early r11/r13 pointer allocation, but further destination-pointer scheduling at8C0249DA differs. Comma vector additions do not fix it. No fixed-address credit; baseline body668, candidates676/672 vs mapped662.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 6. positions — negative

Unit: `drivsym`; target: `8C024C5C`. Reported accepted delta: 0 emitted bytes / 0 functions.

- positions-array-order
- positions-comma-copy

**Residual:** Two existing vector array declarations swapped based on target stack layout: differences1081 to1082. N64 func_800D0424 previous-position comma-copy variant neutral. Target frame68 vs32 remains unexplained; no unused padding introduced.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 7. MovingSurfaceForce — negative

Unit: `drivsym`; target: `8C023A30`. Reported accepted delta: 0 emitted bytes / 0 functions.

- moving-surface-early-return

**Residual:** Native N64 func_800E1F80 early-return structure replaces first outer else. Fixed-address differences624 to625; frame56 vs target52 still first difference8C023A44. Earlier two probes plus this third exhaust budget. Reverted.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 8. checkok — partial

Unit: `drivsym`; target: `8C0250E6`. Reported accepted delta: 0 emitted bytes / 0 functions.

- checkok-rounded-literals
- checkok-target-literals

**Residual:** Target literal inspection proves1.2,.6,.15,.4,.2,-.1,.707 require target-rounding spellings. Initial variant also adjusted .9, but target0x3F666666 proves .9 already correct; second corrected that. No strict bytes gained (1066 diff); branch/pool ordering at8C02511A and later loop remains. Literal correctness only, not whole-function match.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 9. forces — partial

Unit: `drivsym`; target: `8C023DE2`. Reported accepted delta: 0 emitted bytes / 0 functions.

- forces-nonvolatile

**Residual:** Parent approved narrow header experiment. Removing top-level parameter volatile reproduces seven-instruction target save/call/reload/tail-call shape (14 bytes versus baseline22), but linked6 bytes early; relative branch distances differ. No credit. Research patch includes declaration and definition.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

## 10. _ReadShifter — partial

Unit: `controls`; target: `8C01E44C`. Reported accepted delta: 0 emitted bytes / 0 functions.

- shifter-zero-branch
- shifter-shared-return
- shifter-zero-else

**Residual:** Target8C01E4D2 requires true-zero branch into speed selection; explicit goto and structured zero/else each reduce5 differing bytes to2 and preserve all baseline spans. Residual8C01E4D4 compiler RTS versus target BRA to shared return. Shared-return-label variant regresses existing exact controls spans and rejected. Three-probe cap reached, all restored.

**Resume only with:** Resolve target-specific pointer/stack and shared-pool context with new donor or target dataflow evidence; do not repeat exhausted shapes.

