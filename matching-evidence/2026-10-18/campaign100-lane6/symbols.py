import sys,os,json
sys.path.insert(0,os.path.abspath('tools'));import dc_common as C
_,s=C.read_elf('build/campaign100-lane6/zip/baseline/obj/zip.elf')
print(json.dumps([v for v in s if v['name']],indent=2))
