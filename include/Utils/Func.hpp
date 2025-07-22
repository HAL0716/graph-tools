#pragma once

#include <string>
#include <sstream>
#include <iterator>

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
};

} // namespace Utils