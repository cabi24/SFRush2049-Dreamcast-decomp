"""Focused verifier controls; synthetic bytes are not decompilation progress."""
import struct, unittest
from compare import Artifact,compare_bytes,validate_spans,BASE
class Controls(unittest.TestCase):
 def setUp(self):
  self.target=bytes(range(64));self.spans=[{'target_offset':0,'size':8,'kind':'code'},{'target_offset':16,'size':4,'kind':'literal'},{'target_offset':24,'size':8,'kind':'data'}]
 def test_identical_control(self):self.assertTrue(compare_bytes(self.target,Artifact(self.target),self.spans)['exact'])
 def test_linked_elf_positive_control(self):
  b=bytearray(132+len(self.target));b[:6]=b'\x7fELF\x01\x01';struct.pack_into('<HH',b,16,2,42);struct.pack_into('<I',b,32,52);struct.pack_into('<HH',b,46,40,2)
  struct.pack_into('<10I',b,92,0,1,2,BASE,132,len(self.target),0,0,4,0);b[132:]=self.target
  self.assertTrue(compare_bytes(self.target,Artifact(b),self.spans)['exact'])
 def test_code_mutation(self):
  b=bytearray(self.target);b[3]^=1;self.assertFalse(compare_bytes(self.target,Artifact(b),self.spans)['exact'])
 def test_pool_mutation(self):
  b=bytearray(self.target);b[18]^=1;r=compare_bytes(self.target,Artifact(b),self.spans);self.assertFalse(r['exact']);self.assertEqual(r['spans'][1]['differing_bytes'],1)
 def test_relocation_target_mutation(self):
  b=bytearray(self.target);struct.pack_into('<I',b,24,0x8c010010);self.assertFalse(compare_bytes(self.target,Artifact(b),self.spans)['exact'])
 def test_absent_coverage(self):self.assertFalse(compare_bytes(self.target,Artifact(self.target[:16]),self.spans)['exact'])
 def test_overlap_rejected(self):
  with self.assertRaises(ValueError):validate_spans([{'target_offset':0,'size':8,'kind':'code'},{'target_offset':4,'size':8,'kind':'literal'}],64)
 def test_empty_rejected(self):
  with self.assertRaises(ValueError):validate_spans([],64)
 def test_wrong_machine_elf_rejected(self):
  b=bytearray(52);b[:6]=b'\x7fELF\x01\x01';struct.pack_into('<HH',b,16,2,20)
  with self.assertRaisesRegex(ValueError,'not SH'):Artifact(b)
 def test_relocatable_rejected(self):
  b=bytearray(52);b[:6]=b'\x7fELF\x01\x01';struct.pack_into('<HH',b,16,1,42)
  with self.assertRaisesRegex(ValueError,'ET_EXEC'):Artifact(b)
 def test_wrong_endian_rejected(self):
  with self.assertRaises(ValueError):Artifact(b'\x7fELF\x01\x02'+bytes(46))
if __name__=='__main__':unittest.main()
