#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct PairHash {
    std::size_t operator()(const std::pair<std::string, int>& p) const noexcept {
        std::size_t h1 = std::hash<std::string>{}(p.first);
        std::size_t h2 = std::hash<int>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

class NodeEncoder {
private:
    std::unordered_map<std::pair<std::string, int>, int, PairHash> table_;
    int nextId_ = 0;

public:
    int encode(const std::pair<std::string, int>& node);

    void addNodes(const std::vector<std::pair<std::string, int>>& nodes);

    int size() const { return nextId_; }
};