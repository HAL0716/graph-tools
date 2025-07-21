#include "Graph/Transform.hpp"

namespace Graph {

Graph::Data Transform::delSinkNodes(const Graph::Data& data, bool isRecursive) {
    Graph::Data current = data;

    while (true) {
        const auto& adj = current.getAdjList();
        int n = static_cast<int>(adj.size());

        std::vector<unsigned int> delNodes;
        for (unsigned int u = 0; u < n; ++u)
            if (!adj[u].size())
                delNodes.push_back(u);

        if (delNodes.empty())
            break;
        
        current.delNodes(delNodes);

        if (!isRecursive)
            break;
    }

    return current;
}

} // namespace Graph