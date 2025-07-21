#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include "Common.hpp"
#include "Graph/Gen/PFT.hpp"
#include "Graph/Analyzer.hpp"

int main() {
    constexpr int Q = 3;
    constexpr int T = 3;
    constexpr int K = 5;

    Graph::Gen::PFT pft(Q, T, K);

    for (const auto& fword : pft.getWords()) {
        auto graph = pft.gen(fword);

        std::cout << "--- " << fword << " ---" << std::endl
                  << graph.getAdjMatrix() << std::endl
                  << Graph::Analyzer::maxEigenvalue(graph) << std::endl
                  << Graph::Analyzer::avgPathLength(graph) << std::endl
                  << Graph::Analyzer::diameter(graph) << std::endl;
    }

    return 0;
}