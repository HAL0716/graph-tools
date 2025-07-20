#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <Eigen/Dense>
#include "Common.hpp"
#include "NodeEncoder.hpp"

std::ostream& operator<<(std::ostream& os, const std::pair<std::string, int>& node) {
    os << "(" << node.first << "," << node.second << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<int>>& matrix) {
    for (const auto& raw : matrix) {
        for (int val : raw)
            os << val << " ";
        os << std::endl;
    }
    return os;
}

int main() {
    constexpr int Q = 3;
    constexpr int T = 3;
    constexpr int K = 4;
    const std::string fword = "0110";

    std::vector<std::pair<std::string, int>> nodes;
    for (int i = 0; i <= K; ++i)
        nodes.emplace_back(fword.substr(0, i), 0);
    for (int i = 1; i < T; ++i)
        nodes.emplace_back("", i);

    NodeEncoder enc;
    enc.addNodes(nodes);

    const auto edgeLbls = SYMBOLS.substr(0, Q);
    const int nodeCnt = enc.size();

    std::vector<std::vector<int>> adjMatrix(nodeCnt, std::vector<int>(nodeCnt, 0));

    for (const auto& stNode : nodes) {
        if (stNode == std::pair<std::string, int>{fword, 0}) continue;
        const int stId = enc.encode(stNode);

        for (const auto& lbl : edgeLbls) {
            const std::string tgtLbl = stNode.first + lbl;

            for (int i = 0; i <= static_cast<int>(tgtLbl.length()); ++i) {
                const std::pair<std::string, int> edNode{tgtLbl.substr(i), (stNode.second + i) % T};
                const int edId = enc.encode(edNode);

                if (edId < nodeCnt) {
                    adjMatrix[stId][edId]++;
                    break;
                }
            }
        }
    }

    std::cout << adjMatrix << std::endl;

    return 0;
}
