#pragma once

#include <vector>
#include <string>
#include <optional>
#include <Eigen/Dense>

namespace Graph {

// エッジ情報を格納
struct Edge {
    unsigned int dst;                   // 目的ノードのインデックス
    std::optional<double> weight;       // 重み
    std::optional<std::string> label;   // ラベル
};

// グラフデータ管理クラス
class Data {

public:
    using AdjList = std::vector<std::vector<Edge>>;

    // コンストラクタ
    // nodeCount: ノード数
    // directed: 有向グラフかどうか
    // weighted: 重み付きグラフかどうか
    // labeled: ラベル付きグラフかどうか
    Data(unsigned int nodeCount, bool directed = true, bool weighted = false, bool labeled = false);

    // エッジの追加・削除（成功: true）
    bool addEdge(unsigned int src, unsigned int dst, 
                 std::optional<double> weight = std::nullopt, 
                 std::optional<std::string> label = std::nullopt);
    bool delEdge(unsigned int src, unsigned int dst, 
                 std::optional<double> weight = std::nullopt, 
                 std::optional<std::string> label = std::nullopt);
    
    bool delNodes(const std::vector<unsigned int>& tgtNodes);

    // 隣接リストの取得（変更不可）
    const Data::AdjList& getAdjList() const;
    const Data::AdjList getReversedAdjList() const;
    // 隣接行列の取得（変更不可）
    const Eigen::MatrixXd& getAdjMatrix() const;
    // ノード数の取得
    unsigned int size() const noexcept;

    // 各種フラグの取得
    bool isDirected() const noexcept { return directed; }
    bool isWeighted() const noexcept { return weighted; }
    bool isLabeled() const noexcept { return labeled; }

private:
    unsigned int nodeCount;           // ノード数（固定）
    bool directed;                    // 有向グラフフラグ
    bool weighted;                    // 重み付きフラグ
    bool labeled;                     // ラベル付きフラグ

    AdjList adjList; // 隣接リスト

    mutable Eigen::MatrixXd adjMatrix;      // 隣接行列
    mutable bool needsUpdate;               // 隣接行列の更新が必要: true

    // adjListを元にadjMatrixを再構築
    void buildMatrix() const;
};


inline bool hasEdge(const Data::AdjList& adj, unsigned int src, unsigned int dst) {
    if (src >= adj.size()) return false;
    const auto& edges = adj[src];
    return std::any_of(edges.begin(), edges.end(),
                       [dst](const Edge& e){ return e.dst == dst; });
}

} // namespace Graph