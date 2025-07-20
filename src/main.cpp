#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include "Common.hpp"
#include "NodeEncoder.hpp"

int main() {
    constexpr int Q = 3;
    constexpr int T = 3;
    constexpr int K = 4;
    const std::string fword = "0001";

    std::vector<std::pair<std::string, int>> nodes;
    for (int i = 0; i <= K; ++i)
        nodes.emplace_back(fword.substr(0, i), 0);
    for (int i = 1; i < T; ++i)
        nodes.emplace_back("", i);

    NodeEncoder enc;
    enc.addNodes(nodes);

    const auto edgeLbls = SYMBOLS.substr(0, Q);
    const int nodeCnt = enc.size();

    Eigen::MatrixXd adjMat = Eigen::MatrixXd::Zero(nodeCnt, nodeCnt);

    for (const auto& stNode : nodes) {
        if (stNode == std::pair<std::string, int>{fword, 0}) continue;
        int stId = enc.encode(stNode);

        for (const auto& lbl : edgeLbls) {
            std::string tgtLbl = stNode.first + lbl;

            for (int i = 0; i <= static_cast<int>(tgtLbl.length()); ++i) {
                std::pair<std::string, int> edNode{tgtLbl.substr(i), (stNode.second + i) % T};
                int edId = enc.encode(edNode);

                if (edId < nodeCnt) {
                    adjMat(stId, edId) += 1;
                    break;
                }
            }
        }
    }

    Eigen::EigenSolver<Eigen::MatrixXd> solver(adjMat);
    const auto& eigvals = solver.eigenvalues();

    double maxReal = eigvals[0].real();
    for (int i = 1; i < eigvals.size(); ++i)
        maxReal = std::max(maxReal, eigvals[i].real());

    std::cout << std::endl << fword << " : " << maxReal << std::endl;

    return 0;
}