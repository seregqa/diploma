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


g0 = 100
a, b, c = 0.243, 2.367, 150.
D0, kT = 0.25, 1.5
NTr = 1000

#
Time = 50
h = 0.01
#
g_half_lim = HIST_STEP*HIST_BINS
HIST_RANGE = np.linspace(g0-g_half_lim, g0+g_half_lim, HIST_BINS)


if __name__=="__main__":

    foo= Cluster(g0, a,b,c, D0,kT, h,NTr)

    hist2d = [ 0 for i in range( int(Time/h) ) ]

    for t in range(int(Time/h)):

        hist2d[t] = np.histogram( a=list(foo.g), 
                                  bins=HIST_RANGE )[0]
                
        if (t%10==0):
            foo.change_coefs()
            foo.traectories()
        else:
            foo.traectories()

    # OUTPUT
    hist2d = np.array(hist2d)
    print( hist2d.shape )    

    with open('g_hist2d.txt', 'w') as f:
        for h in hist2d:
            f.write( ','.join( map(str,list(h)) ) )
	    f.write( '\n' )