%module Cluster
%{
#include "Cluster.hpp"
%}

%include <stl.i>

namespace std {
	%template(DoubleVector) std::vector < double >;
};

%include "Cluster.hpp"
