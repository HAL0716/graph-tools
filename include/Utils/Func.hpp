#pragma once

#include <string>
#include <sstream>
#include <iterator>
#include <unordered_set>

namespace Utils {

class Func {
public:
    static std::string toStrFix(double value, int precision);

    template <typename Iterable>
    static std::string join(const Iterable& data, char delimiter = ',') {
        auto it = std::begin(data), end = std::end(data);
        if (it == end) return "";

        std::ostringstream oss;
        oss << *it++;
        for (; it != end; ++it)
            oss << delimiter << *it;
        return oss.str();
    }

    template <typename Container1, typename Container2>
    static bool common(const Container1& a, const Container2& b) {
        if (a.size() > b.size()) return common(b, a);

        using ValueType = typename Container1::value_type;
        std::unordered_set<ValueType> setA(a.begin(), a.end());
        for (const auto& e : b) {
            if (setA.count(e)) return true;
        }
        return false;
    }
};

} // namespace Utils
