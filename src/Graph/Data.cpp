#include "Graph/Data.hpp"
#include <algorithm>
#include <stdexcept>

namespace Graph {

Data::Data(unsigned int nodeCount, bool directed, bool weighted, bool labeled)
    : nodeCount(nodeCount),
      directed(directed),
      weighted(weighted),
      labeled(labeled),
      adjList(nodeCount),
      adjMatrix(Eigen::MatrixXd::Zero(nodeCount, nodeCount)),
      needsUpdate(true) {}

// エッジ追加
bool Data::addEdge(unsigned int src, unsigned int dst, std::optional<double> weight, std::optional<std::string> label) {
    if (src >= nodeCount || dst >= nodeCount)
        return false;

    if (weighted && !weight.has_value()) return false;
    if (!weighted) weight = std::nullopt;

    if (labeled && !label.has_value()) return false;
    if (!labeled) label = std::nullopt;

    auto& edges = adjList[src];

    // すでにエッジがあるか
    auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge& e) {
        return e.dst == dst && e.weight == weight && e.label == label;
    });
    if (it != edges.end())
        return false;

    edges.push_back({dst, weight, label});
    needsUpdate = true;

    // 無向グラフなら逆方向も追加
    if (!directed) {
        auto& revEdges = adjList[dst];
        auto revIt = std::find_if(revEdges.begin(), revEdges.end(), [&](const Edge& e) {
            return e.dst == src && e.weight == weight && e.label == label;
        });
        if (revIt == revEdges.end()) {
            revEdges.push_back({src, weight, label});
        }
    }

    return true;
}

// エッジ削除
bool Data::delEdge(unsigned int src, unsigned int dst, std::optional<double> weight, std::optional<std::string> label) {
    if (src >= nodeCount || dst >= nodeCount)
        return false;

    if (weighted && !weight.has_value()) return false;
    if (!weighted) weight = std::nullopt;

    if (labeled && !label.has_value()) return false;
    if (!labeled) label = std::nullopt;

    auto& edges = adjList[src];

    // すでにエッジがあるか
    auto it = std::find_if(edges.begin(), edges.end(), [&](const Edge& e) {
        return e.dst == dst && e.weight == weight && e.label == label;
    });
    if (it == edges.end())
        return false;

    edges.erase(it);
    needsUpdate = true;

    // 無向グラフなら逆方向も削除
    if (!directed) {
        auto& revEdges = adjList[dst];
        auto revIt = std::find_if(revEdges.begin(), revEdges.end(), [&](const Edge& e) {
            return e.dst == src && e.weight == weight && e.label == label;
        });
        if (revIt != revEdges.end()) {
            revEdges.erase(revIt);
        }
    }

    return true;
}

// 隣接リスト取得
const std::vector<std::vector<Edge>>& Data::getAdjList() const {
    return adjList;
}

// 隣接行列取得
const Eigen::MatrixXd& Data::getAdjMatrix() const {
    if (needsUpdate) {
        buildMatrix();
        needsUpdate = false;
    }
    return adjMatrix;
}

// ノード数取得
unsigned int Data::size() const noexcept {
    return nodeCount;
}

// 隣接リストから隣接行列を構築
void Data::buildMatrix() const {
    adjMatrix = Eigen::MatrixXd::Zero(nodeCount, nodeCount);

    for (unsigned int src = 0; src < nodeCount; ++src) {
        for (const Edge& e : adjList[src]) {
            double val = weighted ? e.weight.value_or(1.0) : 1.0;
            adjMatrix(src, e.dst) = val;
        }
    }
}

} // namespace Graph