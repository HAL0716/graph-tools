#pragma once

#include <map>
#include "Graph/Data.hpp"
#include "Graph/Encoder.hpp"

namespace Graph {
    
class Isomorphism {
public:
    using Group = std::vector<unsigned int>;
    using GroupPair = std::pair<Group, Group>;
    using GroupList = std::vector<GroupPair>;

    static bool solver(const Data& g1, const Data& g2);
    static bool solver(const Data& g1, const Data& g2, std::vector<int>& maps);

private:
    static std::map<std::string, std::vector<unsigned int>> genFeat2Nodes(const Data::AdjList& adj, const Data::AdjList& rev, Encoder& enc1, Encoder& enc2);
    static bool setGroups(
        const std::map<std::string, std::vector<unsigned int>>& featA,
        const std::map<std::string, std::vector<unsigned int>>& featB,
        GroupList& groups,
        std::vector<Group>& nodeToGroup
    );
    static bool matchGroups(
        const Data::AdjList& adjA, const Data::AdjList& revA,
        const Data::AdjList& adjB, const Data::AdjList& revB,
        GroupList& groups,
        const std::vector<Group>& nodeToGroup,
        std::vector<int>& maps,
        int groupIdx = 0,
        int permIdx = 0
    );
    static bool verifySubMapping(
        const Data::AdjList& adjA, const Data::AdjList& revA,
        const Data::AdjList& adjB, const Data::AdjList& revB,
        const std::vector<int>& maps,
        int srcA,
        const std::vector<Group>& nodeToGroup
    );
    // static bool Isomorphism::verifyMapping(const Data::AdjList& adjA, const Data::AdjList& adjB, const std::vector<int>& maps);
};

} // namespace Graph