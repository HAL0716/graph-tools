#include "WordGenerator.hpp"
#include "Common.hpp"
#include <unordered_set>
#include <algorithm>
#include <cmath>

std::vector<std::string> WordGenerator::genWords(int Q, int T, int K, bool isFilter) const {
    auto lengthForSegment = [&](int i) -> int {
        return (K / T) + (i < (K % T) ? 1 : 0);
    };

    std::vector<std::vector<std::string>> segments(T);
    for (int i = 0; i < T; ++i)
        segments[i] = genWords(Q, lengthForSegment(i), isFilter);

    std::vector<size_t> indices(T, 0), limits(T);
    size_t totalCombinations = 1;
    for (int i = 0; i < T; ++i) {
        limits[i] = segments[i].size();
        totalCombinations *= limits[i];
    }

    std::vector<std::string> result;
    result.reserve(totalCombinations);
    std::string word(K, SYMBOLS[0]);

    while (true) {
        for (int i = 0; i < K; ++i) {
            const int t = i % T, p = i / T;
            word[i] = segments[t][indices[t]][p];
        }
        result.push_back(word);

        int pos = T - 1;
        while (pos >= 0 && ++indices[pos] == limits[pos])
            indices[pos--] = 0;
        if (pos < 0) break;
    }

    return result;
}

std::vector<std::string> WordGenerator::genWords(int Q, int L, bool isFilter) const {
    const size_t total = static_cast<size_t>(std::pow(Q, L));
    std::vector<std::string> result;
    result.reserve(total);

    std::vector<int> indices(L, 0);
    std::vector<int> uniq;
    std::vector<bool> seen(Q, false);
    uniq.reserve(L);

    while (true) {
        if (!isFilter || filter(indices, uniq, seen, Q)) {
            result.emplace_back(toString(indices));
        }

        int pos = L - 1;
        while (pos >= 0 && ++indices[pos] == Q)
            indices[pos--] = 0;
        if (pos < 0) break;
    }

    return result;
}

bool WordGenerator::filter(const std::vector<int>& indices, std::vector<int>& uniq, std::vector<bool>& seen, int Q) const {
    uniq.clear();
    std::fill(seen.begin(), seen.end(), false);

    for (int idx : indices) {
        if (!seen[idx]) {
            seen[idx] = true;
            uniq.push_back(idx);
        }
    }

    int cnt = 0;
    for (size_t i = 0; i + 1 < uniq.size(); ++i)
        cnt += (uniq[i + 1] - uniq[i] == 1);

    return *std::max_element(indices.begin(), indices.end()) == cnt;
}

std::string WordGenerator::toString(const std::vector<int>& indices) const {
    std::string s;
    s.reserve(indices.size());
    for (int idx : indices)
        s.push_back(SYMBOLS[idx]);
    return s;
}
