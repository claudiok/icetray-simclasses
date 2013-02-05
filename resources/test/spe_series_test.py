#!/usr/bin/env python
from icecube import icetray
from icecube.icetray.I3Test import ENSURE
from icecube import simclasses as sc
import random 
import time

NHITS = 10000000

binned_h = sc.I3MCSPESeries()
binned_h.bin_width = 1.0
unbinned_h = sc.I3MCSPESeries()

print "NHITS = ",NHITS
for p in range(1,7):
    
    nbins = 10**p

    t0 = time.time()
    for i in range(NHITS):
        t = random.uniform(0.,nbins)
        unbinned_h.fill(t)
    t0 = time.time() - t0

    t1 = time.time()
    for i in range(NHITS):
        t = random.uniform(0.,nbins)
        binned_h.fill(t)
    t1 = time.time() - t1

    print "sampling range = (0,%d) nbins = %d : binned/unbinned running time = %f" \
          % (nbins,len(binned_h.npe_values),t1/t0)


