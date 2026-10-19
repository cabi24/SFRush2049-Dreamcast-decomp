from pathlib import Path
p=Path('src/circuit.c');s=p.read_text().replace('    return rand_r(&gCircuitSeed) % (max + 1u);','    unsigned int value = rand_r(&gCircuitSeed);\n    return value % (max + 1u);');p.write_text(s)
