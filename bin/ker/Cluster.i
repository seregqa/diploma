%module Cluster
%{
#include "Cluster.hpp"
%}

%include <stl.i>

namespace std {
	%template(DoubleVector) std::vector < double >;
        %template(DoubleVector2d) std::vector < vector<double> >;
};

%include "Cluster.hpp"
