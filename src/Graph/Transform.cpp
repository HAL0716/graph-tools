#include "Graph/Transform.hpp"

#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <deque>
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

int Transform::Encoder::encode(const std::string& val) {
    auto [it, inserted] = table_.insert({val, nextId_});
    if (inserted) ++nextId_;
    return it->second;
}

int Transform::Encoder::size() const {
    return static_cast<int>(table_.size());
}

void Transform::Encoder::clear() {
    table_.clear();
    nextId_ = 0;
}

Graph::Data Transform::mergeByMoore(const Graph::Data& data) {
    if (!data.isLabeled()) return data;

    const auto& adjList = data.getAdjList();
    const auto n = data.size();

    std::vector<int> node2id(n, -1);
    std::unordered_map<int, std::vector<int>> id2nodes;
    Encoder enc;

    for (int src = 0; src < n; ++src) {
        std::deque<std::string> key;
        for (const auto& edge : adjList[src])
            key.push_back(edge.label.value());
        std::sort(key.begin(), key.end());

        int id = enc.encode(key);
        node2id[src] = id;
        id2nodes[id].push_back(src);
    }

    std::vector<int> newNode2id(n, -1);
    std::unordered_map<int, std::vector<int>> newId2nodes;

    do {
        std::fill(newNode2id.begin(), newNode2id.end(), -1);
        newId2nodes.clear();
        enc.clear();

        for (const auto& [id, nodes] : id2nodes) {
            for (int src : nodes) {
                std::deque<std::string> key{std::to_string(id)};
                for (const auto& edge : adjList[src])
                    key.push_back(edge.label.value() + ":" + std::to_string(node2id[edge.dst]));
                std::sort(key.begin(), key.end());

                int newId = enc.encode(key);
                newNode2id[src] = newId;
                newId2nodes[newId].push_back(src);
            }
        }

        node2id.swap(newNode2id);
        id2nodes.swap(newId2nodes);
    } while (newId2nodes.size() != id2nodes.size());
    
    Graph::Data result(static_cast<unsigned int>(id2nodes.size()), data.isDirected(), data.isWeighted(), data.isLabeled());
    for (int src = 0; src < n; ++src)
        for (const auto& edge : adjList[src])
            result.addEdge(node2id[src], node2id[edge.dst], edge.weight, edge.label);
    
    return result;
}

} // namespace Graph