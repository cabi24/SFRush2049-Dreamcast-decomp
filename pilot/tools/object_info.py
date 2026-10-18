"""Inspect generated ELF32 SH object sections and typed relocation records; do not mask/apply them."""
from pathlib import Path
import argparse,json,struct,hashlib

def inspect(data):
 if len(data)<52 or data[:6]!=b'\x7fELF\x01\x01':raise ValueError('not ELF32 little-endian; convert/link proprietary formats with reviewed vendor tools')
 typ,machine=struct.unpack_from('<HH',data,16)
 if machine!=42:raise ValueError('not SH machine 42')
 shoff=struct.unpack_from('<I',data,32)[0];entsize,count,namesidx=struct.unpack_from('<HHH',data,46)
 if not count or entsize<40 or shoff+count*entsize>len(data) or namesidx>=count:raise ValueError('invalid section table')
 raw=[struct.unpack_from('<10I',data,shoff+i*entsize) for i in range(count)]
 for s in raw:
  if s[1]!=8 and s[4]+s[5]>len(data):raise ValueError('truncated section')
 ns=raw[namesidx]
 if ns[1]!=3:raise ValueError('section names are not a string table')
 names=data[ns[4]:ns[4]+ns[5]];sections=[];relocs=[]
 for idx,s in enumerate(raw):
  no,kind,flags,addr,off,size,link,info,align,entity=s;end=names.find(b'\0',no)
  if no>=len(names) or end<0:raise ValueError('invalid section name')
  name=names[no:end].decode('ascii','replace')
  sections.append({'index':idx,'name':name,'type':kind,'flags':flags,'address':addr,'size':size,'alignment':align,'sha256':hashlib.sha256(data[off:off+size]).hexdigest() if kind!=8 else None})
  if kind in [4,9]:
   n=12 if kind==4 else 8
   if size%n or entity not in [0,n] or link>=count or info>=count:raise ValueError('malformed relocation section')
   for pos in range(off,off+size,n):
    r_offset,r_info=struct.unpack_from('<II',data,pos);relocs.append({'target_section':info,'offset':r_offset,'type':r_info&255,'symbol_index':r_info>>8,'addend':struct.unpack_from('<i',data,pos+8)[0] if kind==4 else None,'addend_source':'explicit' if kind==4 else 'in section bytes; not decoded here'})
 return {'elf_type':typ,'machine':machine,'sections':sections,'relocations':relocs,'exact_match_claim':False}
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('object',type=Path);a=p.parse_args();print(json.dumps(inspect(a.object.read_bytes()),indent=2))
if __name__=='__main__':main()
