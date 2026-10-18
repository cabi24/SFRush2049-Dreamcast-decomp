"""Synthetic typed-relocation controls; never accepted as original target objects."""
import struct,unittest
from object_info import inspect

def fixture():
 names=b'\0.text\0.shstrtab\0.rel.text\0.rela.text\0'
 b=bytearray(252+len(names)+4+8+12)
 b[:6]=b'\x7fELF\x01\x01';struct.pack_into('<HH',b,16,1,42)
 struct.pack_into('<I',b,32,52);struct.pack_into('<HHH',b,46,40,5,2)
 p=252;b[p:p+len(names)]=names;p+=len(names)
 struct.pack_into('<10I',b,92,1,1,6,0,p,4,0,0,2,0);p+=4
 struct.pack_into('<10I',b,132,7,3,0,0,252,len(names),0,0,1,0)
 struct.pack_into('<10I',b,172,17,9,0,0,p,8,0,1,4,8);struct.pack_into('<II',b,p,2,(7<<8)|5);p+=8
 struct.pack_into('<10I',b,212,27,4,0,0,p,12,0,1,4,12);struct.pack_into('<IIi',b,p,0,(8<<8)|1,-4)
 return b

class ObjectControls(unittest.TestCase):
 def test_typed_rel_and_rela(self):
  r=inspect(fixture());self.assertEqual(r['relocations'][0]['type'],5);self.assertEqual(r['relocations'][0]['symbol_index'],7);self.assertIsNone(r['relocations'][0]['addend']);self.assertEqual(r['relocations'][1]['addend'],-4);self.assertFalse(r['exact_match_claim'])
 def test_truncated_rejected(self):
  with self.assertRaisesRegex(ValueError,'truncated'):inspect(fixture()[:-1])
 def test_short_rejected(self):
  with self.assertRaises(ValueError):inspect(b'\x7fELF')
if __name__=='__main__':unittest.main()
