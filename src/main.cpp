#include <iostream>
#include <string>
#include <vector>
#include <cmath>
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

    Graph::Analyzer analyzer;
    std::vector<std::vector<std::string>> result;
    result.reserve(words.size() + 1);
    result.push_back({"禁止語", "半径", "直径", "平均経路長", "固有値", "符号化率"});

    for (const auto& fword : words) {
        const auto& data = pft.gen(fword);
        analyzer.setData(data);
        result.push_back({
            fword, std::to_string(analyzer.radius()), std::to_string(analyzer.diameter()),
            Utils::Func::toStrFix(analyzer.avgPathLength(), 3),
            Utils::Func::toStrFix(analyzer.maxEigenvalue(), 6),
            Utils::Func::toStrFix(log(analyzer.maxEigenvalue()) / log(Q), 6),
        });
    }

    const std::string filename = "Q=" + std::to_string(Q) + "_T=" + std::to_string(T) + "_K=" + std::to_string(K) + ".csv";
    if (Utils::CSV::write(filename, result))
        std::cout << "Saved: " << filename << std::endl;
    else
        std::cerr << "Error saving: " << filename << std::endl;

    return 0;
}
