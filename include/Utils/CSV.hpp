#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iterator>
#include <type_traits>
#include <iomanip>

#include "Utils/Constants.hpp"

namespace Utils {

class CSV {
public:
    static std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter = ',');

    template <typename RowContainer>
    static bool write(const std::string& filename, const RowContainer& data, char delimiter = ',') {
        const auto filepath = Utils::OUTPUTDIR / filename;
        if (auto dir = filepath.parent_path(); !dir.empty())
            std::filesystem::create_directories(dir);

        std::ofstream ofs(filepath);
        if (!ofs.is_open())
            return false;

        for (const auto& row : data) {
            auto it = std::begin(row);
            const auto end = std::end(row);

            while (it != end) {
                ofs << toStr(*it);
                if (++it != end)
                    ofs << delimiter;
            }
            ofs << '\n';
        }

        return true;
    }

private:
    static std::vector<std::string> split(const std::string& line, char delimiter);

    template <typename T>
    static std::string toStr(const T& value, int precision = 9) {
        if constexpr (std::is_convertible_v<T, std::string>) {
            return std::string(value);
        } else if constexpr (std::is_arithmetic_v<T>) {
            if constexpr (std::is_floating_point_v<T>) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(precision) << value;
                return oss.str();
            } else {
                return std::to_string(value);
            }
        } else {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    }
};

} // namespace Utils
