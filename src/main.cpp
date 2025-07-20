#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "Common.hpp"

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

    for (const auto& [label, phase] : nodes)
        std::cout << "(" << label << "," << phase << ")\n";

    return 0;
}
