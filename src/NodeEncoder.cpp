#include "NodeEncoder.hpp"

int NodeEncoder::encode(const std::pair<std::string, int>& node) {
    auto [it, inserted] = table_.insert({node, nextId_});
    if (inserted) ++nextId_;
    return it->second;
}

void NodeEncoder::addNodes(const std::vector<std::pair<std::string, int>>& nodes) {
    for (const auto& node : nodes) {
        encode(node);
    }
}