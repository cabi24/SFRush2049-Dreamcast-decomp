"""Post-canonical-build stability check (read-only).

1) summary counters in build/report.json
2) per-unit records for the 21 dirty units: canonical build/report.json
   vs the dirty per-run reports saved in runs/<unit>/dirty/report.json
   (ignoring built_at/seconds)
3) evidence/strict-integration-receipt.json: current diff vs HEAD must equal
   the audited dirty-receipt diff saved as receipt-diff.txt (no timestamps
   exist in the receipt, so byte-stability is expected)
4) git status inventory of the dirty tree
"""
import json
import subprocess

REPO = r"C:\rush2049-dreamcast"
LANE = REPO + r"\matching-evidence\2026-10-18\cabin-lane1"

report = json.load(open(REPO + r"\build\report.json", "rb"))
units = {u["name"]: u for u in report["units"]}

# 1. summary
s = report["summary"]
checks = [
    ("map_sha256", report["map_sha256"], "5aebc1783c96742de045188d696fa8cc1756241b4a6e94acf336a0b66b9208cf"),
    ("target_sha256", report["target_sha256"], "336c97aa409e4b38582c43077381b1c99d6a64bbec630e3be7a9829adda8e688"),
    ("game exact B", s["game_code"]["exact_bytes"], 62498),
    ("game total B", s["game_code"]["total_bytes"], 569664),
    ("image exact B", s["image"]["exact_bytes"], 131829),
    ("image total B", s["image"]["total_bytes"], 1123492),
    ("functions exact", s["functions"]["exact"], 417),
    ("functions total", s["functions"]["total"], 4717),
    ("units mapped", s["units"]["mapped"], 157),
    ("units exact", s["units"]["exact"], 26),
    ("units partial", s["units"]["partial"], 86),
    ("units with_source", s["units"]["with_source"], 112),
]
ok = True
for name, got, want in checks:
    mark = "OK " if got == want else "FAIL"
    if got != want:
        ok = False
    print(f"[{mark}] {name}: {got} (want {want})")
print("1. summary:", "PASS" if ok else "FAIL")
print()

# 2. per-unit cross-check
IGNORE = {"built_at", "seconds", "generated"}
DIRTY_UNITS = [
    "HAL_text", "Init", "TB_text", "blit", "controls", "dc_misc_8C06C08C",
    "fmath", "game", "model", "options", "optionscr", "pakerr", "pane",
    "streaks", "stree", "stunt", "unk_8C065F2C", "visuals", "zoid",
    "zoidpoly", "dc_unk_8C069E34",
]
bad = 0
for n in DIRTY_UNITS:
    dirty = json.load(open(f"{LANE}\\runs\\{n}\\dirty\\report.json", "rb"))
    canon = units.get(n)
    if canon is None:
        print(f"[FAIL] {n}: missing from canonical report")
        bad += 1
        continue
    dk = {k: v for k, v in dirty.items() if k not in IGNORE}
    ck = {k: v for k, v in canon.items() if k not in IGNORE}
    if dk == ck:
        print(f"[OK ] {n}: canonical == dirty per-run record")
    else:
        bad += 1
        print(f"[FAIL] {n}: differs")
        for k in sorted(set(dk) | set(ck)):
            if dk.get(k) != ck.get(k):
                print(f"       key {k}: dirty={dk.get(k)!r} canon={ck.get(k)!r}")
print(f"2. per-unit: {len(DIRTY_UNITS) - bad}/{len(DIRTY_UNITS)} identical")
print()

# 3. receipt vs audited diff
cur = subprocess.run(
    ["git", "-C", REPO, "diff", "--", "evidence/strict-integration-receipt.json"],
    capture_output=True, text=True, encoding="utf-8", errors="replace")
saved = open(LANE + r"\receipt-diff.txt", "rb").read().decode("utf-8", errors="replace")

def body(d):
    return [l for l in d.splitlines()
            if l.startswith(("@@", "+", "-"))
            and not l.startswith("---") and not l.startswith("+++")]

cb, sb = body(cur.stdout), body(saved)
if cb == sb:
    print(f"3. receipt: PASS (diff vs HEAD identical to audited diff; {len(cb)} body lines)")
else:
    print(f"3. receipt: FAIL (saved {len(sb)} lines, current {len(cb)} lines)")
    import difflib
    for l in list(difflib.unified_diff(sb, cb, "saved", "current", lineterm=""))[:60]:
        if l.startswith(("+", "-")) and not l.startswith(("+++", "---")):
            print("   ", l)
print()

# 4. git status inventory
st = subprocess.run(["git", "-C", REPO, "status", "--porcelain"],
                    capture_output=True, text=True, encoding="utf-8", errors="replace")
lines = [l for l in st.stdout.splitlines() if l.strip()]
by_state = {}
for l in lines:
    state = l[:2]
    by_state.setdefault(state, []).append(l[3:])
print(f"4. git status: {len(lines)} entries")
for state in sorted(by_state):
    files = by_state[state]
    print(f"   {state}: {len(files)}")
    for f in files:
        print(f"      {f}")
