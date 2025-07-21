#pragma once

#include "Graph/Data.hpp"

namespace Graph {

class Analyzer {
public:
    static double maxEigenvalue(const Data& g);
    static int diameter(const Data& g);
    static double avgPathLength(const Data& g);
};

} // namespace Graph