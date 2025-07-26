#include "Graph/Transform.hpp"

// #include <vector>
// #include <algorithm>
// #include <utility>
#include "Graph/Encoder.hpp"
#include "Utils/Func.hpp"

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

Graph::Data Transform::mergeByMoore(const Graph::Data& data) {
    if (!data.isLabeled()) return data;

    const auto& adjList = data.getAdjList();
    const int N = data.size();
    std::vector<int> curr(N, -1), next(N, -1);

    auto genKey = [&](int node) {
        std::vector<std::string> key{std::to_string(curr[node])};
        for (const auto& edge : adjList[node])
            if (edge.label) key.emplace_back(edge.label.value() + ":" + std::to_string(curr[edge.dst]));
        std::sort(key.begin(), key.end());
        return key;
    };

    Encoder enc;
    while (true) {
        enc.clear();
        for (int node = 0; node < N; ++node)
            next[node] = enc.encode(genKey(node));

        if (next == curr)
            break;
        
        curr = next;
    }

    Graph::Data result(enc.size(), data.isDirected(), data.isWeighted(), data.isLabeled());

    for (int src = 0; src < N; ++src)
        for (const auto& edge : adjList[src])
            result.addEdge(curr[src], curr[edge.dst], edge.weight, edge.label);
    
    return result;
}

} // namespace Graph