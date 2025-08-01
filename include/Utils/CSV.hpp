#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <filesystem>
#include <type_traits>
#include <iomanip>

#include "Utils/Constants.hpp"

namespace Utils {

class CSV {
public:
    static std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter = ',');

    template <typename RowContainer>
    static bool write(const std::string& filename,
                      const RowContainer& data,
                      const std::vector<std::string>& headers = {},
                      char delimiter = ',',
                      bool verbose = true) {
        const auto filepath = Utils::OUTPUTDIR / filename;
        std::filesystem::create_directories(filepath.parent_path());

        std::ofstream ofs(filepath);
        if (!ofs.is_open()) {
            if (verbose)
                std::cerr << "Failed to open file: " << filepath << std::endl;
            return false;
        }

        if (!headers.empty()) {
            for (size_t i = 0; i < headers.size(); ++i) {
                ofs << headers[i];
                if (i + 1 < headers.size()) ofs << delimiter;
            }
            ofs << '\n';
        }

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

        if (verbose)
            std::cout << "Saved CSV: " << filepath << std::endl;

        return true;
    }

private:
    static std::vector<std::string> split(const std::string& line, char delimiter);

    template<typename T>
    struct is_string_like : std::disjunction<
        std::is_same<std::decay_t<T>, std::string>,
        std::is_same<std::decay_t<T>, const char*>,
        std::is_same<std::decay_t<T>, char*>
    > {};

    template <typename T>
    static std::string toStr(const T& value, int precision = 9) {
        if constexpr (is_string_like<T>::value) {
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