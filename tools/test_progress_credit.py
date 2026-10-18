"""Focused controls for the observed linker-fill overcount; no target execution."""
import unittest
from dc_build import produced_credit,interval_union_size
class Credits(unittest.TestCase):
 def test_internal_linker_hole_is_not_source(self):self.assertEqual(produced_credit({0,1,4,5},0,6,True),4)
 def test_explicit_zero_bytes_still_count(self):self.assertEqual(produced_credit(set(range(6)),0,6,True),6)
 def test_differing_range_earns_nothing(self):self.assertEqual(produced_credit(set(range(6)),0,6,False),0)
 def test_outside_bytes_do_not_count(self):self.assertEqual(produced_credit({-1,0,1,5,6},0,6,True),3)
 def test_data_overlap_does_not_overstate_coverage(self):self.assertEqual(interval_union_size([(0,100),(20,25),(80,120)]),120)
if __name__=='__main__':unittest.main()
