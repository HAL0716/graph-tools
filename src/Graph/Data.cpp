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

bool Data::delNodes(const std::vector<unsigned int>& tgtNodes) {
    // 削除対象をbool配列に変換
    std::vector<bool> toDelete(nodeCount, false);
    for (auto v : tgtNodes) {
        if (v >= nodeCount) return false;
        toDelete[v] = true;
    }

    // 新ノードID割当
    std::vector<int> newId(nodeCount, -1);
    unsigned int newCount = 0;
    for (unsigned int i = 0; i < nodeCount; ++i)
        if (!toDelete[i])
            newId[i] = newCount++;

    std::vector<std::vector<Edge>> newAdjList(newCount);
    for (unsigned int u = 0; u < nodeCount; ++u) {
        if (toDelete[u]) continue;
        for (const auto& e : adjList[u]) {
            if (!toDelete[e.dst]) {
                newAdjList[newId[u]].push_back({static_cast<unsigned int>(newId[e.dst]), e.weight, e.label});
            }
        }
    }

    // 更新
    nodeCount = newCount;
    adjList = std::move(newAdjList);
    needsUpdate = true;

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
            adjMatrix(src, e.dst) += val;
        }
    }
}

} // namespace Graph