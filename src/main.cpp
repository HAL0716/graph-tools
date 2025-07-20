#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "Common.hpp"
#include "NodeEncoder.hpp"

std::ostream& operator<<(std::ostream& os, const std::pair<std::string, int>& node) {
    os << "(" << node.first << "," << node.second << ")";
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

    for (const auto& stNode : nodes) {
        if (stNode == std::make_pair(fword, 0)) continue;
        for (const auto& lbl : edgeLbls) {
            std::string tgtLbl = stNode.first + lbl;

            for (int i = 0; i <= static_cast<int>(tgtLbl.length()); ++i) {
                std::pair<std::string, int> edNode{tgtLbl.substr(i), (stNode.second + i) % T};

                if (enc.encode(edNode) < nodeCnt) {
                    std::cout << stNode << " -" << lbl << "-> " << edNode << '\n';
                    break;
                }
            }
        }
    }

    return 0;
}
