#pragma once

#include "Graph/Data.hpp"

namespace Graph {

class Transform {
public:
    static Graph::Data delSinkNodes(const Graph::Data& data, bool isRecursive = true);
};
    
} // namespace Graph