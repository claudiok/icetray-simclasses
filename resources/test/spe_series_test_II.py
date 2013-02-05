#!/usr/bin/env python
from icecube import icetray
from icecube.icetray.I3Test import ENSURE
from icecube import simclasses as sc
import random 
import time

binned_h = sc.I3MCSPESeries()
binned_h.bin_width = 1.0
unbinned_h = sc.I3MCSPESeries()

for p in range(4,7):
    for n in range(2,7):
        nhits = 10**n
        nbins = 10**p

        t0 = time.time()
        for i in range(nhits):
            t = random.uniform(0.,nbins)
            unbinned_h.fill(t)
        t0 = time.time() - t0

        t1 = time.time()
        for i in range(nhits):
            t = random.uniform(0.,nbins)
            binned_h.fill(t)
        t1 = time.time() - t1

        print "sampling range = (0,%d) nhits = %d nbins = %d : running time ratio = %f"\
              % (nbins,nhits,len(binned_h.npe_values),t1/t0)


