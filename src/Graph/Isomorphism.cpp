#include "Graph/Isomorphism.hpp"
#include <set>
#include <queue>
#include <unordered_map>
#include <algorithm>

#include <iostream>
#include "Utils/Func.hpp"

namespace Graph {

//===========================================================
// Queue: BFS探索用のユーティリティ
//===========================================================
class BFSQueue {
public:
    struct State { int node, dist; };

    explicit BFSQueue(int nodeCount) {
        lastDist_.reserve(nodeCount);
    }

    bool push(int node, int dist) {
        auto it = lastDist_.find(node);
        if (it == lastDist_.end() || it->second == -dist) {
            lastDist_[node] = dist;
            queue_.push({node, dist});
            return true;
        }
        return false;
    }

    bool empty() const noexcept { return queue_.empty(); }

    State pop() {
        State front = queue_.front();
        queue_.pop();
        return front;
    }

private:
    std::queue<State> queue_;
    std::unordered_map<int, int> lastDist_;
};

//===========================================================
// Isomorphism::solver
//===========================================================
bool Isomorphism::solver(const Data& g1, const Data& g2) {
    std::vector<int> maps;
    return solver(g1, g2, maps);
}

bool Isomorphism::solver(const Data& g1, const Data& g2, std::vector<int>& maps) {
    if (g1.size() != g2.size()) return false;

    const auto& adj1 = g1.getAdjList(), rev1 = g1.getReversedAdjList();
    const auto& adj2 = g2.getAdjList(), rev2 = g2.getReversedAdjList();

    Encoder enc1, enc2;
    const auto& feat1 = genFeat2Nodes(adj1, rev1, enc1, enc2);
    const auto& feat2 = genFeat2Nodes(adj2, rev2, enc1, enc2);
    if (g1.size() != g2.size()) return false;

    GroupList groups;
    std::vector<Group> nodeToGroup(adj1.size());

    if (!setGroups(feat1, feat2, groups, nodeToGroup))
        return false;

    maps.assign((int)adj1.size(), -1);

    return matchGroups(adj1, rev1, adj2, rev2, groups, nodeToGroup, maps);
}

//===========================================================
// Isomorphism::genFeat2Nodes
//===========================================================
std::map<std::string, std::vector<unsigned int>> Isomorphism::genFeat2Nodes(const Data::AdjList& adj, const Data::AdjList& rev, Encoder& enc1, Encoder& enc2) {
    struct Deg {
        int out;
        int in;

        Deg(int o = 0, int i = 0) : out(o), in(i) {}

        std::string toStr() const {
            return "(" + std::to_string(out) + "," + std::to_string(in) + ")";
        }

        // map のキーとして使う場合は比較演算子が必要
        bool operator<(const Deg& other) const noexcept {
            return (out < other.out) || (out == other.out && in < other.in);
        }
    };

    using Dset = std::vector<int>;
    using FeatMap = std::map<Deg, std::vector<int>>;

    const int n = static_cast<int>(adj.size());

    // 各ノードの {出次数, 入次数} を計算
    std::vector<Deg> node2deg(n);
    for (int i = 0; i < n; ++i) {
        node2deg[i] = Deg(static_cast<int>(adj[i].size()), static_cast<int>(rev[i].size()));
    }

    // nodeToFeat[j][deg] -> エンコードされた距離集合ID
    std::vector<FeatMap> nodeToFeat(n);

    auto collectDistances = [&](int start) {
        std::vector<Dset> node2dset(n);
        BFSQueue queue(n);

        node2dset[start].push_back(0);
        queue.push(start, 0);

        while (!queue.empty()) {
            auto [src, dist] = queue.pop();

            // 順方向 BFS
            if (dist >= 0) {
                for (const auto& edge : adj[src]) {
                    node2dset[edge.dst].push_back(dist + 1);
                    queue.push(edge.dst, dist + 1);
                }
            }

            // 逆方向 BFS
            if (dist <= 0) {
                for (const auto& edge : rev[src]) {
                    node2dset[edge.dst].push_back(dist - 1);
                    queue.push(edge.dst, dist - 1);
                }
            }
        }
        return node2dset;
    };

    // 各ノード i を起点に BFS を行い、特徴量を登録
    for (int i = 0; i < n; ++i) {
        auto node2dset = collectDistances(i);
        const auto& deg = node2deg[i];

        for (int j = 0; j < n; ++j) {
            std::sort(node2dset[j].begin(), node2dset[j].end());
            nodeToFeat[j][deg].push_back(enc1.encode(node2dset[j]));
        }
    }

    // 特徴量マップを構築
    std::map<std::string, std::vector<unsigned int>> result;
    for (int i = 0; i < n; ++i) {
        std::vector<std::string> feat;
        for (auto& [deg, ids] : nodeToFeat[i]) {
            std::sort(ids.begin(), ids.end());
            feat.push_back(deg.toStr() + ":" + std::to_string(enc2.encode(ids)));
        }

        result[Utils::Func::join(feat)].push_back(i);
    }

    return result;
}

bool Isomorphism::setGroups(
    const std::map<std::string, std::vector<unsigned int>>& featA,
    const std::map<std::string, std::vector<unsigned int>>& featB,
    GroupList& groups,
    std::vector<Group>& nodeToGroup
) {
    if (featA.size() != featB.size())
        return false;

    groups.clear();
    auto itA = featA.begin();
    auto itB = featB.begin();

    while (itA != featA.end()) {
        auto& [fA, nodesA] = *itA++;
        const auto& [fB, nodesB] = *itB++;

        if (fA != fB || nodesA.size() != nodesB.size())
            return false;

        groups.emplace_back(nodesA, nodesB);

        for (int n : nodesA)
            nodeToGroup[n] = nodesB;
    }

    std::sort(
        groups.begin(), groups.end(),
        [](const GroupPair& a, const GroupPair& b) {
            return a.first.size() < b.first.size();
        }
    );

    return true;
}

bool Isomorphism::matchGroups(
    const Data::AdjList& adjA, const Data::AdjList& revA,
    const Data::AdjList& adjB, const Data::AdjList& revB,
    GroupList& groups,
    const std::vector<Group>& nodeToGroup,
    std::vector<int>& maps,
    int groupIdx,
    int permIdx
) {
    if (groupIdx == static_cast<int>(groups.size())) {
        return true;
        // return verifyMapping(adjA, adjB, maps);
    }

    auto& [groupA, groupB] = groups[groupIdx];
    const int N = static_cast<int>(groupB.size());

    if (permIdx == N)
        return matchGroups(adjA, revA, adjB, revB, groups, nodeToGroup, maps, groupIdx + 1, 0);

    for (int i = permIdx; i < N; ++i) {
        std::swap(groupB[permIdx], groupB[i]);

        int oldNode = groupA[permIdx];
        int newNode = groupB[permIdx];
        maps[oldNode] = newNode;

        if (verifySubMapping(adjA, revA, adjB, revB, maps, oldNode, nodeToGroup))
            if (matchGroups(adjA, revA, adjB, revB, groups, nodeToGroup, maps, groupIdx, permIdx + 1))
                return true;

        maps[oldNode] = -1;
        std::swap(groupB[permIdx], groupB[i]);
    }

    return false;
}

bool Isomorphism::verifySubMapping(
    const Data::AdjList& adjA, const Data::AdjList& revA,
    const Data::AdjList& adjB, const Data::AdjList& revB,
    const std::vector<int>& maps,
    int srcA,
    const std::vector<Group>& nodeToGroup
) {
    auto extractDst = [](const std::vector<Edge>& edges) {
        std::vector<unsigned int> res;
        res.reserve(edges.size());
        for (const auto& e : edges) res.push_back(e.dst);
        return res;
    };

    int srcB = maps[srcA];

    auto check = [&](const Data::AdjList& adjA, const Data::AdjList& adjB) {
        for (auto& edgeA : adjA[srcA]) {
            int dstB = maps[edgeA.dst];
            if ((dstB != -1 && !hasEdge(adjB, srcB, dstB)) ||
                (dstB == -1 && !Utils::Func::common(nodeToGroup[edgeA.dst], extractDst(adjB[srcB]))))
                return false;
        }
        return true;
    };

    return check(adjA, adjB) && check(revA, revB);
}

// bool Isomorphism::verifyMapping(const Data::AdjList& adjA, const Data::AdjList& adjB, const std::vector<int>& maps) {
//     Data::AdjList transformed;
//     for (int i = 0; i < adjA.size(); ++i) {
//         for (const auto& edge : adjA[i])
//             transformed[maps[i]].insert(maps[edge.dst]);

//     return transformed == adjB;
// }

} // namespace Graph