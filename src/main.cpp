#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <Eigen/Dense>
#include "Common.hpp"
#include "Graph/Gen/PFT.hpp"

int main() {
    constexpr int Q = 3;
    constexpr int T = 3;
    constexpr int K = 5;

    Graph::Gen::PFT pft(Q, T, K);

    for (const auto& fword : pft.getWords()) {
        auto graph = pft.gen(fword);

        Eigen::MatrixXd adjMat = graph.getAdjMatrix();

        Eigen::EigenSolver<Eigen::MatrixXd> solver(adjMat);
        const auto& eigvals = solver.eigenvalues();

        double maxReal = eigvals[0].real();
        for (int i = 1; i < eigvals.size(); ++i)
            maxReal = std::max(maxReal, eigvals[i].real());

        std::cout << "--- " << fword << " ---" << std::endl
                  << adjMat << std::endl << maxReal << std::endl;
    }

    return 0;
}