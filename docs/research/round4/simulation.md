# Round 4 simulation: 25 source-probed targets

The only accepted round-four gain is a natural pointer-induction dependency in PlayGame, separately on master. It restores downstream AbortGame: 170 emitted function bytes plus 88 pool bytes, one function. PlayGame remains nonexact.

SetClutch's absolute-RPM expression yields an identical 88-byte body displaced by 180 bytes. The archived patch earns zero credit and is not accepted. Cylinder/radius helper probes exposed genuine copied-input/delta-vector stack differences; no artificial arrays or frame padding were introduced.

Accepted master results: [round-four matching note](../../round4-matching.md). Associated probe records can share whole-TU builds; they are not a unique compiler invocation count.

| Unit | Function | Target | Outcome | Associated records | Accepted bytes/functions |
|---|---|---|---|---:|---:|
| circuit | SetCircuitCode | `8C01D18C` | bounded-negative-restored | 1 | 0/0 |
| collision | setFBCollisionForce | `8C01D9A4` | bounded-negative-restored | 1 | 0/0 |
| collision | setMiniFBCollisionForce | `8C01E0B8` | bounded-negative-restored | 1 | 0/0 |
| targets | _AnimateCoin | `8C05A020` | bounded-negative-restored | 1 | 0/0 |
| targets | _AnimateRotGeneric | `8C05A422` | bounded-negative-restored | 1 | 0/0 |
| drones | _AssignDrones | `8C02558A` | bounded-negative-restored | 1 | 0/0 |
| game | PlayGame | `8C028932` | accepted-dependency-gain | 1 | 258/1 |
| game | _CalcPolePositions | `8C028EB4` | bounded-negative-restored | 1 | 0/0 |
| mdrive | Init_MDrive | `8C03DF34` | bounded-negative-restored | 1 | 0/0 |
| mdrive | SetClutch | `8C03E18C` | research-moved-exact-restored | 1 | 0/0 |
| mdrive | _UpdatePlayerModel | `8C03E56C` | bounded-negative-restored | 1 | 0/0 |
| mdrive | _UpdateDroneModels | `8C03E76A` | bounded-negative-restored | 1 | 0/0 |
| mdrive | modelstop | `8C03EE5E` | bounded-negative-restored | 1 | 0/0 |
| mdrive | End_MDrive | `8C03FAD8` | bounded-negative-restored | 1 | 0/0 |
| targetpaths | _PlacePathTargetPos | `8C056186` | bounded-negative-restored | 1 | 0/0 |
| targetpaths | _PlaceGhostRacePath | `8C056548` | bounded-negative-restored | 1 | 0/0 |
| targetpaths | _AnimatePathTarget | `8C056AC0` | bounded-negative-restored | 1 | 0/0 |
| targetpaths | _PlayMoveSound | `8C057414` | bounded-negative-restored | 1 | 0/0 |
| targets | _InitBBoxModelStruct | `8C057D2C` | bounded-negative-restored | 1 | 0/0 |
| targets | TargetPathAvoid | `8C057E16` | bounded-negative-restored | 1 | 0/0 |
| targets | SetTargetDef | `8C057F7C` | bounded-negative-restored | 1 | 0/0 |
| targets | _TargetCollCylinder | `8C058A1C` | bounded-negative-restored | 3 | 0/0 |
| targets | _TargetCollMineCylinder | `8C058B00` | bounded-negative-restored | 3 | 0/0 |
| targets | _TargetCollRadius | `8C058BCA` | bounded-negative-restored | 3 | 0/0 |
| targets | _StartCone | `8C059D48` | bounded-negative-restored | 1 | 0/0 |
