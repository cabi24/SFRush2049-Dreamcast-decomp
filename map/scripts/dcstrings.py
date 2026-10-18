"""DC string literals: printable NUL-terminated runs (len>=3) -> raw/dc_strings.json {addr: text}"""
import json
from common import *
out = {}
i = 0
P = set(range(32, 127)) | {9, 10, 13}
while i < SIZE:
    if DATA[i] in P and (i == 0 or DATA[i - 1] == 0 or i % 4 == 0):
        j = i
        while j < SIZE and DATA[j] in P: j += 1
        if j < SIZE and DATA[j] == 0 and j - i >= 3:
            out[BASE + i] = DATA[i:j].decode('latin-1')
            i = j + 1; continue
        i = j if j > i else i + 1
    else:
        i += 1
json.dump({h(k): v for k, v in out.items()}, open(os.path.join(RAW, 'dc_strings.json'), 'w'), indent=0)
print(len(out))
