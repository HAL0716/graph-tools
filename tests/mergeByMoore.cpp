#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "Graph/Data.hpp"
#include "Graph/Transform.hpp"

TEST_CASE("Graph::Transform::mergeByMoore") {
    Graph::Data data(7, true, false, true);
    std::vector<std::tuple<int, int, std::string>> edges = {{0, 3, "0"}, {0, 1, "1"}, {1, 4, "0"}, {2, 0, "0"}, {3, 5, "0"}, {3, 2, "1"}, {4, 3, "0"}, {4, 1, "1"}, {5, 6, "1"}, {6, 4, "0"}};
    for (const auto& [src, dst, label] : edges)
        data.addEdge(src, dst, std::nullopt, label);

    Graph::Data minimized = Graph::Transform::mergeByMoore(data);

    REQUIRE(minimized.size() == 4);

    std::vector<std::vector<std::string>> expLbls = {{"0"}, {"0", "1"}, {"0", "1"}, {"1"}};
    std::vector<std::vector<std::string>> actLbls(expLbls.size());

    const auto& adjList = minimized.getAdjList();
    for (size_t i = 0; i < adjList.size(); ++i) {
        for (const auto& edge : adjList[i])
            actLbls[i].push_back(edge.label.value());
        std::sort(actLbls[i].begin(), actLbls[i].end());
    }
    std::sort(actLbls.begin(), actLbls.end());

    REQUIRE(actLbls == expLbls);
}
