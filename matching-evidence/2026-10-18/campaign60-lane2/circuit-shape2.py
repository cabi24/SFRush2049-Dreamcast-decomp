from pathlib import Path
p=Path('src/circuit.c');s=Path('matching-evidence/2026-10-18/campaign60-lane2/circuit.original.c').read_text().replace('    return rand_r(&gCircuitSeed) % (max + 1u);','    unsigned int divisor = max + 1u;\n    unsigned int value = rand_r(&gCircuitSeed);\n    return value % divisor;');p.write_text(s)
