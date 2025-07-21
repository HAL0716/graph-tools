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
    // コンストラクタ
    // nodeCount: ノード数
    // directed: 有向グラフかどうか
    // weighted: 重み付きグラフかどうか
    // labeled: ラベル付きグラフかどうか
    Data(unsigned int nodeCount, bool directed = true, bool weighted = false, bool labeled = false);

    // エッジの追加（成功: true）
    bool addEdge(unsigned int src, unsigned int dst, 
                 std::optional<double> weight = std::nullopt, 
                 std::optional<std::string> label = std::nullopt);
    // エッジの削除（成功: true）
    bool delEdge(unsigned int src, unsigned int dst, 
                 std::optional<double> weight = std::nullopt, 
                 std::optional<std::string> label = std::nullopt);

    // 隣接リストの取得（変更不可）
    const std::vector<std::vector<Edge>>& getAdjList() const;
    // 隣接行列の取得（変更不可）
    const Eigen::MatrixXd& getAdjMatrix() const;
    // ノード数の取得
    unsigned int size() const noexcept;

    // 各種フラグの取得
    bool isDirected() const noexcept { return directed; }
    bool isWeighted() const noexcept { return weighted; }
    bool isLabeled() const noexcept { return labeled; }

private:
    const unsigned int nodeCount;           // ノード数（固定）
    const bool directed;                    // 有向グラフフラグ
    const bool weighted;                    // 重み付きフラグ
    const bool labeled;                     // ラベル付きフラグ

    std::vector<std::vector<Edge>> adjList; // 隣接リスト

    mutable Eigen::MatrixXd adjMatrix;      // 隣接行列
    mutable bool needsUpdate;               // 隣接行列の更新が必要: true

    // adjListを元にadjMatrixを再構築
    void buildMatrix() const;
};

} // namespace Graph