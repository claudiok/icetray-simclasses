#!/usr/bin/env python
import unittest

class TestStr(unittest.TestCase):

    def test_str_empty(self):
        import icecube.simclasses
        import re

        # skipping maps which don't have proper __str__ method
        # skipping I3MMCTrack*, where __str__ is not implemented yet
        classNameList = ['CorsikaLongProfile',
                         'CorsikaLongStep',
                         'I3CorsikaShowerInfo',
                         'I3MCPE',
                         'I3MCPESeries',
                         'I3MCPMTResponse',
                         'I3MCPulse',
                         'I3MCPulseSeries']

        hasBadStr = []
        for className in classNameList:
            instance = eval("icecube.simclasses." + className + "()")
            s = str(instance)
            if re.search("object at [01]x[0-9a-f]+", s) is not None:
                hasBadStr.append(className)

        if hasBadStr:
            print "Bad __str__:", ", ".join(hasBadStr)

        self.assertEqual(len(hasBadStr), 0)

if __name__ == "__main__":
    unittest.main()
