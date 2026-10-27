# Round 4 presentation: 25 source-probed targets

No accepted gain. Three retained research candidates were independently replayed on clean baseline translation units: _TextLen becomes moved-exact at 8C02FC48; removing _SetupFontData's unsupported empty character loop leaves 102 differing bytes; StartWeapon's fixed-base expression leaves 23 differing bytes at the proper 174-byte extent and correctly places AnimateWeapon. Each earns zero additional credit and preserves every prior exact function.

Some other exploratory same-TU trials retained earlier rejected context. Their differing-byte changes are not isolated causal gains. This caveat remains even though the three archived candidates were separately replayed cleanly. Missing camera/world-damage frames were not fabricated with stack padding. Original private ledgers and raw first-difference listings are intentionally omitted.

Accepted master results: [round-four matching note](../../round4-matching.md). Associated probe records can share whole-TU builds; they are not a unique compiler invocation count.

| Unit | Function | Target | Outcome | Associated records | Accepted bytes/functions |
|---|---|---|---|---:|---:|
| camera | CheckCameraView | `8C01908C` | bounded-negative-restored | 1 | 0/0 |
| camera | View3Cam | `8C0194EE` | bounded-negative-restored | 1 | 0/0 |
| camera | update_camera | `8C019D68` | bounded-negative-restored | 1 | 0/0 |
| camera | _UpdateDroneObjects | `8C01A34E` | bounded-negative-restored | 1 | 0/0 |
| HAL_text | HAL_DrawText | `8C02F658` | bounded-negative-restored | 1 | 0/0 |
| HAL_text | _TextLen | `8C02FC80` | bounded-negative-restored | 1 | 0/0 |
| camera | _UpdateRearCamera | `8C01A61E` | bounded-negative-restored | 1 | 0/0 |
| camera | circle_camera_around_car | `8C01A878` | bounded-negative-restored | 1 | 0/0 |
| camera | init_camera_on_track | `8C01A9C8` | bounded-negative-restored | 1 | 0/0 |
| camera | fix_camera_in_space | `8C01AB68` | bounded-negative-restored | 1 | 0/0 |
| camera | steady_move_cam | `8C01ACE8` | bounded-negative-restored | 1 | 0/0 |
| camera | maxpath_cam | `8C01B1C0` | bounded-negative-restored | 1 | 0/0 |
| camera | point_at_car | `8C01B406` | bounded-negative-restored | 1 | 0/0 |
| camera | LookInDir | `8C01B58E` | bounded-negative-restored | 1 | 0/0 |
| HAL_text | HAL_StartText | `8C02ED1C` | bounded-negative-restored | 1 | 0/0 |
| worlddam | newWorldDamage | `8C09AECC` | bounded-negative-restored | 1 | 0/0 |
| HAL_text | _SetupFontData | `8C02F090` | bounded-negative-restored | 1 | 0/0 |
| skids | _GetSkid | `8C04EAA8` | bounded-negative-restored | 1 | 0/0 |
| skids | AnimateSkids | `8C04F234` | bounded-negative-restored | 1 | 0/0 |
| smoke | StartSmoke | `8C04FA38` | bounded-negative-restored | 1 | 0/0 |
| smoke | UpdateSmoke | `8C050766` | bounded-negative-restored | 1 | 0/0 |
| waves | func_8C06588C | `8C06588C` | bounded-negative-restored | 1 | 0/0 |
| streaks | _AnimateStreakPolys | `8C098E76` | bounded-negative-restored | 3 | 0/0 |
| weapons | StartWeapon | `8C0995F4` | bounded-negative-restored | 1 | 0/0 |
| weapons | AnimateWeapon | `8C0996A2` | bounded-negative-restored | 1 | 0/0 |
