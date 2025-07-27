#pragma once

#include "Graph/Data.hpp"
#include <unordered_map>
#include <string>
#include <vector>

namespace Graph::Gen {

class PFT {
public:
    PFT(unsigned int alphabetSize, unsigned int period, unsigned int forbiddenLength);

    // 禁止語に基づくグラフ生成
    Graph::Data gen(const std::string& forbiddenWord) const;

    // 許容語一覧の取得
    std::vector<std::string> getWords(bool useFilter = true) const;

private:
    struct Node {
        std::string label;
        int phase;

        std::string toStr() const {
            return "(" + label + "," + std::to_string(phase) + ")";
        }

        bool operator==(const Node& other) const {
            return label == other.label && phase == other.phase;
        }

        struct hash {
            std::size_t operator()(const Node& n) const {
                return std::hash<std::string>()(n.label) ^ (std::hash<int>()(n.phase) << 1);
            }
        };
    };

    class Word {
    public:
        static std::vector<std::string> gen(int Q, int L, bool useFilter);
    private:
        static std::string toString(const std::vector<int>& indices);
    };

    unsigned int Q;  // アルファベットサイズ
    unsigned int T;  // 周期
    unsigned int K;  // 禁止語の長さ
};

} // namespace Graph::Gen