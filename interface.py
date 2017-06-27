import Cluster
import numpy as np



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



if __name__=="__main__":

    c = Cluster.Cluster(100,2,3,4,5,6,7,8)

    for t in range(int(Time/h)):
        if (t%10==0):
            c.change_coefs()
            c.traectories()
        else:
            c.traectories()

    # OUTPUT
    print (list(c.g))

