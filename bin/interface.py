from ker.Cluster import Cluster
import numpy as np


HIST_STEP = 0.1
HIST_BINS = 200

###
# params: Cluster(g0, a, b, c, D0, kT, h, NTr)
#
# g0 - initial cluster size (quantity of drops)
# a -
# b -
# c -
# D0 -
# kT -
# h - time step (time/space ? )
# NTr - number of traectories
###


g0 = 274
a, b, c = 0.24318, 2.366613, 25.
D0, kT = 0.25, 1.5
NTr = 1000000

#
Time = 50
h = 0.05
#
g_half_lim = HIST_STEP*HIST_BINS
HIST_RANGE = np.linspace(g0-g_half_lim, g0+g_half_lim, HIST_BINS)


if __name__=="__main__":

    foo= Cluster(g0, a,b,c, D0,kT, h,NTr, Time)
    foo.time_loop(change_coefs_freq=5)

    print(foo.g)

    hist2d = [ 0 for i in range( int(Time/h) ) ]
    for t,hist in enumerate(list(foo.g_hist)):
        hist2d[t] = np.histogram( a=list(foo.g),
                                  bins=HIST_RANGE )[0]
    print( hist2d.shape )

    hist2d = np.array(hist2d)
    with open('g_hist2d.txt', 'w') as f:
        for h in hist2d:
            f.write( ','.join( map(str,list(h)) ) )
	    f.write( '\n' )
