#include "Graph/Gen/PFT.hpp"
#include "Utils/Constants.hpp"
#include <Eigen/Dense>
#include <cmath>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>

namespace Graph::Gen {

PFT::PFT(unsigned int alphabetSize, unsigned int period, unsigned int length)
    : Q(alphabetSize), T(period), K(length) {}

// グラフ生成
Graph::Data PFT::gen(const std::string& fWord) const {
    auto createNodes = [&](const std::string& word) {
        std::vector<Node> nodes;
        for (int i = 0; i <= static_cast<int>(K); ++i)
            nodes.emplace_back(Node{word.substr(0, i), 0});
        for (int i = 1; i < static_cast<int>(T); ++i)
            nodes.emplace_back(Node{"", i});
        return nodes;
    };

    const auto nodes = createNodes(fWord);

    Encoder encoder;
    encoder.addNodes(nodes);

    const int n = encoder.size();
    Graph::Data graph(n, true, false, true);
    const Node fNode(fWord, 0);
    const auto edgeLabels = Utils::SYMBOLS.substr(0, Q);

    for (const auto& stNode : nodes) {
        if (stNode == fNode) continue;
        int stId = encoder.encode(stNode);

        for (const auto& lbl : edgeLabels) {
            std::string tgtLbl = stNode.label + lbl;

            for (int i = 0; i <= static_cast<int>(tgtLbl.length()); ++i) {
                Node edNode{tgtLbl.substr(i), static_cast<int>((stNode.phase + i) % T)};
                int edId = encoder.encode(edNode);
                
                if (graph.addEdge(stId, edId, std::nullopt, std::string(1, lbl)))
                    break;
            }
        }
    }

    return graph;
}

// --- 許容語の生成 ---
std::vector<std::string> PFT::getWords(bool useFilter) const {
    auto calcLen = [&](int i) -> int {
        return (K / T) + (i < (K % T) ? 1 : 0);
    };

    std::vector<std::vector<std::string>> segments(T);
    for (int i = 0; i < T; ++i)
        segments[i] = Word::gen(Q, calcLen(i), useFilter);

    std::vector<size_t> indices(T, 0);
    std::vector<size_t> limits(T);
    size_t total = 1;

    for (int i = 0; i < T; ++i) {
        limits[i] = segments[i].size();
        total *= limits[i];
    }

    std::vector<std::string> result;
    result.reserve(total);
    std::string word(K, Utils::SYMBOLS[0]);

    while (true) {
        for (int i = 0; i < K; ++i)
            word[i] = segments[i % T][indices[i % T]][i / T];
        result.push_back(word);

        int pos = T - 1;
        while (pos >= 0 && ++indices[pos] == limits[pos])
            indices[pos--] = 0;
        if (pos < 0) break;
    }

    std::sort(result.begin(), result.end());

    return result;
}

// --- Encoder 実装 ---
void PFT::Encoder::addNodes(const std::vector<Node>& nodes) {
    for (const auto& node : nodes) {
        encode(node);
    }
}

int PFT::Encoder::encode(const Node& node) {
    auto [it, inserted] = table_.insert({node, nextId_});
    if (inserted) ++nextId_;
    return it->second;
}

int PFT::Encoder::size() const {
    return static_cast<int>(table_.size());
}

// --- Word 実装 ---
std::vector<std::string> PFT::Word::gen(int Q, int L, bool useFilter) {
    const size_t total = static_cast<size_t>(std::pow(Q, L));
    std::vector<std::string> result;
    result.reserve(total);

    std::vector<int> indices(L, 0);

    // フィルター判定lambda
    auto passesFilter = [&](const std::vector<int>& idx) {
        if (!useFilter) return true;

        std::vector<int> uniq;
        uniq.reserve(idx.size());
        for (int val : idx)
            if (std::find(uniq.begin(), uniq.end(), val) == uniq.end())
                uniq.push_back(val);

        int cnt = 0;
        for (size_t i = 0; i + 1 < uniq.size(); ++i)
            cnt += (uniq[i + 1] - uniq[i] == 1);

        return *std::max_element(idx.begin(), idx.end()) == cnt;
    };

    while (true) {
        if (passesFilter(indices))
            result.emplace_back(toString(indices));

        int pos = L - 1;
        while (pos >= 0 && ++indices[pos] == Q)
            indices[pos--] = 0;
        if (pos < 0) break;
    }
    return result;
}

std::string PFT::Word::toString(const std::vector<int>& indices) {
    std::string s;
    s.reserve(indices.size());
    for (int idx : indices)
        s.push_back(Utils::SYMBOLS[idx]);
    return s;
}

} // namespace Graph::Gen