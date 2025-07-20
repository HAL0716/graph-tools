#include "Encoder.hpp"

std::string Encoder::Node2Label(const std::pair<std::string, int>& node) {
    return node.first + ":" + std::to_string(node.second);
}

int Encoder::encode(const std::string& label) {
    auto [it, inserted] = table_.insert({label, nextId_});
    if (inserted) ++nextId_;
    return it->second;
}

int Encoder::encode(const std::pair<std::string, int>& node) {
    return encode(Node2Label(node));
}