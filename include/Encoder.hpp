#pragma once

#include <string>
#include <unordered_map>

class Encoder {
private:
    std::unordered_map<std::string, int> table_;
    int nextId_ = 0;

    std::string Node2Label(const std::pair<std::string, int>& node);

public:
    int encode(const std::string& label);
    int encode(const std::pair<std::string, int>& node);
};