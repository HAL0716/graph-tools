#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "Graph/Data.hpp"
#include "Graph/Isomorphism.hpp"

TEST_CASE("Graph::Isomorphism::solver") {
    Graph::Data data1(7, true, false, true);
    std::vector<std::tuple<int, int, std::string>> edges1 = {
        {0,3,"0"}, {0,1,"1"}, {1,4,"0"}, {2,0,"0"}, {3,5,"0"},
        {3,2,"1"}, {4,3,"0"}, {4,1,"1"}, {5,6,"1"}, {6,4,"0"}
    };
    for (const auto& [src, dst, label] : edges1)
        data1.addEdge(src, dst, std::nullopt, label);

    Graph::Data data2(7, true, false, true);
    std::vector<std::tuple<int, int, std::string>> edges2 = {
        {4,0,"0"}, {4,5,"1"}, {5,1,"0"}, {6,4,"0"}, {0,2,"0"},
        {0,6,"1"}, {1,0,"0"}, {1,5,"1"}, {2,3,"1"}, {3,1,"0"}
    };
    for (const auto& [src, dst, label] : edges2)
        data2.addEdge(src, dst, std::nullopt, label);

    std::vector<int> maps;
    REQUIRE(Graph::Isomorphism::solver(data1, data2, maps));
    REQUIRE(maps == std::vector<int>{4, 5, 6, 0, 1, 2, 3});
}
