#pragma once
#include <vector>
#include <string>

class WordGenerator {
public:
    WordGenerator() = default;

    std::vector<std::string> genWords(int Q, int T, int K, bool isFilter = false) const;
    std::vector<std::string> genWords(int Q, int L, bool isFilter = false) const;

private:
    std::string toString(const std::vector<int>& indices) const;
    bool filter(const std::vector<int>& indices, std::vector<int>& uniq, std::vector<bool>& seen, int Q) const;
};