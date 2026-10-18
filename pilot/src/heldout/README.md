# Held-out slots

enginetorque, UpdateFrameTimer and GetWaveByIndex retain their fixed held-out roles. Static identification was allowed; no compiler options or C bodies were tuned against them. The first two have identified spans; the getter remains unlocated. Compilation requires an explicitly frozen configuration, its external SHA256 pin, and --validate-heldout. Do not substitute another getter to improve the result.
