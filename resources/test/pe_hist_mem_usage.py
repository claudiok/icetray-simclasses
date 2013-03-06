#!/usr/bin/env python
from icecube import icetray
from icecube.icetray.I3Test import ENSURE
from icecube import simclasses as sc
import random 
import time

try:
    from memory_profiler import memory_usage
    
    NHITS = 1000000000
    
    print "expected unbinned mem usage = %.3f MB"\
          % (NHITS * 8. * 1e-6)

    print "NHITS = ",NHITS
    nbins = 10**3

    mem_usage = memory_usage(-1, interval=1, timeout = 1)
    print "baseline mem usage: %.3f MB" % mem_usage[0]

    unbinned_h = sc.I3MCSPEHistogramFloat()
    for i in range(NHITS):
        t = random.uniform(0.,nbins)
        unbinned_h.fill(t)

    mem_usage = memory_usage(-1, interval=1, timeout = 1)
    print "unbinned mem usage: %.3f MB" % mem_usage[0]
    del unbinned_h

    binned_h = sc.I3MCSPEHistogramFloat(1.0)
    for i in range(NHITS):
        t = random.uniform(0.,nbins)
        binned_h.fill(t)
    mem_usage = memory_usage(-1, interval=1, timeout = 1)
    print "binned mem usage: %.3f MB" % mem_usage[0]
except ImportError:
    print "You must install memory_profiler for this to run"
    print "http://pypi.python.org/pypi/memory_profiler"





