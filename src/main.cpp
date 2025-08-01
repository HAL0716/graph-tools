#include <iostream>
#include <set>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "Graph/Gen/PFT.hpp"
#include "Graph/Transform.hpp"
#include "Graph/Analyzer.hpp"
#include "Graph/Isomorphism.hpp"
#include "Utils/Func.hpp"
#include "Utils/Input.hpp"
#include "Utils/CSV.hpp"

// === Utility ===

std::string formatNum(int width, int num) {
    std::ostringstream oss;
    oss << std::setw(width) << num;
    return oss.str();
}

std::string formatSummary(int wordCount, int groupCount, int entropyCount) {
    return formatNum(4, wordCount) + "->" + formatNum(2, groupCount) + "->" + formatNum(2, entropyCount);
}

// === CSV Path Helper ===

std::string makeCsvPath(const std::string& type, int q, int t = -1, int k = -1) {
    return type + "/Q=" + std::to_string(q)
         + (t >= 0 ? "/T=" + std::to_string(t) : "")
         + (k >= 0 ? "_K=" + std::to_string(k) : "")
         + ".csv";
}

// === CSV Table Builders ===

using Table = std::vector<std::vector<std::string>>;

Table initTypeTable(const std::pair<int, int>& kRange) {
    Table table;
    table.push_back({"G_DB -> G_min -> EV"});

    std::vector<std::string> header{"T \\ K"};
    for (int k = kRange.first; k <= kRange.second; ++k)
        header.push_back(std::to_string(k));

    table.push_back(std::move(header));
    return table;
}

Table makeIsoTable(
    const std::deque<std::vector<std::pair<std::string, Graph::Data>>>& groups) {

    Table table;
    table.reserve(groups.size());

    for (const auto& group : groups) {
        std::vector<std::string> words;
        words.reserve(group.size());

        for (const auto& [word, _] : group)
            words.push_back(word);

        std::sort(words.begin(), words.end());
        table.push_back(std::move(words));
    }

    std::sort(table.begin(), table.end());
    return table;
}

std::pair<Table, int> makeEntropyTable(
    const std::deque<std::vector<std::pair<std::string, Graph::Data>>>& groups, int q) {

    Graph::Analyzer analyzer;
    Table table;
    std::set<std::string> entropys;

    for (const auto& group : groups) {
        analyzer.setData(group.front().second);
        double eigen   = analyzer.maxEigenvalue();
        double entropy = std::log(eigen) / std::log(q);

        table.push_back({
            group.front().first,
            Utils::Func::toStrFix(entropy, 9),
            Utils::Func::toStrFix(analyzer.avgPathLength(), 9)
        });

        entropys.insert(Utils::Func::toStrFix(entropy, 9));
    }

    std::sort(table.begin(), table.end());
    return {table, static_cast<int>(entropys.size())};
}

// === Core Algorithm ===

std::deque<std::vector<std::pair<std::string, Graph::Data>>>
groupIsomorphic(int q, int t, int k, std::vector<std::string>& words) {

    Graph::Gen::PFT pft(q, t, k);
    words = pft.getWords();

    std::cout << "(" << q << "," << t << "," << k << ") : " << words.size() << std::endl;

    using WordGraphPair = std::pair<std::string, Graph::Data>;
    std::deque<std::vector<WordGraphPair>> groups;

    for (const auto& word : words) {
        auto graph = pft.gen(word);
        graph = Graph::Transform::delSinkNodes(graph);
        graph = Graph::Transform::mergeByMoore(graph);

        bool matched = false;
        for (auto& group : groups) {
            if (Graph::Isomorphism::solver(graph, group.front().second)) {
                group.emplace_back(word, graph);
                matched = true;
                break;
            }
        }

        if (!matched)
            groups.emplace_front(std::vector<WordGraphPair>{{word, graph}});
    }

    return groups;
}

// === CSV Writers ===

void writeIsoTable(int q, int t, int k, const Table& table) {
    Utils::CSV::write(makeCsvPath("Iso", q, t, k), table);
}

void writeEntropyTable(int q, int t, int k, const Table& table) {
    Utils::CSV::write(makeCsvPath("Entropy", q, t, k), table, {"禁止語", "エントロピー", "平均経路長"});
}

void writeTypeTable(int q, const Table& table) {
    Utils::CSV::write(makeCsvPath("Types", q), table);
}

// === Main ===

int main() {
    const std::pair<int, int> qRange = {2,  5};
    const std::pair<int, int> tRange = {2, 10};
    const std::pair<int, int> kRange = {1, 10};

    for (int q = qRange.first; q <= qRange.second; ++q) {
        auto typeTable = initTypeTable(kRange);

        for (int t = tRange.first; t <= tRange.second; ++t) {
            std::vector<std::string> row{formatNum(2, t)};

            for (int k = kRange.first; k <= kRange.second; ++k) {
                if (k <= t) {
                    row.push_back("     --     ");
                    continue;
                }

                std::vector<std::string> words;
                auto isoGroups = groupIsomorphic(q, t, k, words);
                auto isoTable = makeIsoTable(isoGroups);
                auto [entropyTable, entropyCount] = makeEntropyTable(isoGroups, q);

                writeIsoTable(q, t, k, isoTable);
                writeEntropyTable(q, t, k, entropyTable);

                row.push_back(formatSummary(static_cast<int>(words.size()), static_cast<int>(isoGroups.size()), entropyCount));
            }

            typeTable.push_back(std::move(row));
        }

        writeTypeTable(q, typeTable);
    }

    return 0;
}