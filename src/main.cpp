#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>

#include "Graph/Gen/PFT.hpp"
#include "Graph/Analyzer.hpp"
#include "Utils/Func.hpp"
#include "Utils/Input.hpp"
#include "Utils/CSV.hpp"

int main() {
    const int Q = Utils::Input::readInRange<int>("Enter Q (alphabet): ", 2, 20);
    const int T = Utils::Input::readInRange<int>("Enter T ( period ): ", 1, 20);
    const int K = Utils::Input::readInRange<int>("Enter K ( length ): ", 1, 20);

    Graph::Gen::PFT pft(Q, T, K);
    std::vector<std::string> words = pft.getWords();

    std::vector<std::vector<std::string>> result;
    result.reserve(words.size() + 1);
    result.push_back({"fword", "maxEigen", "avgPathLen"});

    for (const auto& fword : words) {
        const auto& graph = pft.gen(fword);
        const double maxEV = Graph::Analyzer::maxEigenvalue(graph);
        const double avgPL = Graph::Analyzer::avgPathLength(graph);
        result.push_back({fword, Utils::Func::toStrFix(maxEV, 6), Utils::Func::toStrFix(avgPL, 6)});
    }

    const std::string filename = "Q=" + std::to_string(Q) + "_T=" + std::to_string(T) + "_K=" + std::to_string(K) + ".csv";
    if (Utils::CSV::write(filename, result))
        std::cout << "Saved: " << filename << std::endl;
    else
        std::cerr << "Error saving: " << filename << std::endl;

    return 0;
}
