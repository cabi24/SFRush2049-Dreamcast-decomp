import sys, pathlib, json
ROOT = pathlib.Path(r'C:\rush2049-dreamcast')
sys.path.insert(0, str(ROOT / 'tools'))
import dc_build as B
C = B.C
label = sys.argv[1] if len(sys.argv) > 1 else 'baseline'
out = pathlib.Path(__file__).parent / label
out.mkdir(parents=True, exist_ok=True)
symbols = C.read_symbol_file(C.SYMBOLS_TXT)
externs = C.read_symbol_file(C.EXTERNS_TXT)
unit = next(u for u in C.load_map()['units'] if u['name'] == 'password')
flat = B.expand_includes(str(ROOT / 'src/password.c'))
if label == 'loop-update':
    flat = flat.replace('for (b = 0; b < numBits; b++) {\n        gData', 'for (b = 0; b < numBits; b++, gNumBits++) {\n        gData')
    flat = flat.replace('        data >>= 1;\n        gNumBits++;', '        data >>= 1;')
elif label == 'body-counter':
    flat = flat.replace('for (b = 0; b < numBits; b++) {\n        gData', 'for (b = 0; b < numBits;) {\n        gData')
    flat = flat.replace('        data >>= 1;\n        gNumBits++;', '        b++;\n        data >>= 1;\n        gNumBits++;')
elif label == 'split-index':
    flat = flat.replace('    int b; /* r0 */', '    int b; /* r0 */\n    unsigned int index;\n    unsigned int bit;')
    flat = flat.replace('        gData[gNumBits >> 3] |= (data & 1) << (gNumBits & 7);', '        bit = gNumBits & 7;\n        index = gNumBits >> 3;\n        gData[index] |= (data & 1) << bit;')
private_source = out / 'password.c'
private_source.write_text(flat + '\n', encoding='latin-1')
original_compile = B.compile_unit
B.compile_unit = lambda name, src: original_compile(name, str(private_source))
C.BUILD = str(out)
C.OBJ_DIR = str(out / 'obj')
C.LINK_DIR = str(out / 'link')
pathlib.Path(C.OBJ_DIR).mkdir(exist_ok=True)
pathlib.Path(C.LINK_DIR).mkdir(exist_ok=True)
result = B.build_unit(unit, symbols, externs)
(out / 'result.json').write_text(json.dumps(result, indent=2))
print(json.dumps({k: result.get(k) for k in ['status','exact_text_bytes','exact_data_bytes','functions_exact','functions','notes']}, indent=2))
