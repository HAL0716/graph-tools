#pragma once

#include <unordered_map>
#include <string>
#include <sstream>
#include <iterator>
#include "Graph/Data.hpp"
#include "Utils/Func.hpp"

namespace Graph {

class Transform {
public:
    static Graph::Data delSinkNodes(const Graph::Data& data, bool isRecursive = true);
    static Graph::Data mergeByMoore(const Graph::Data& data);
};

} // namespace Graph
