P = /usr/include/python2.7

Cluster : Cluster.py _Cluster.so;

Cluster.py Cluster_wrap.cxx : Cluster.i Cluster.hpp
	swig -python -c++ -E Cluster.i

Cluster_wrap.o : Cluster_wrap.cxx Cluster.hpp
	g++ -g -Wall -I$(P) -fPIC -std=c++11 -O3 -c Cluster_wrap.cxx

Cluster.o : Cluster.hpp Cluster.cpp
	g++ -g -Wall -I$(P) -fPIC -O3 -std=c++11 -c Cluster.cpp

_Cluster.so : Cluster_wrap.o Cluster.o
	g++ -shared -o _Cluster.so Cluster_wrap.o Cluster.o -lz
