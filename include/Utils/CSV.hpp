#pragma once

#include <string>
#include <vector>

namespace Utils {

class CSV {
public:
    static std::vector<std::vector<std::string>> read(const std::string& filename, char delimiter = ',');
    static bool write(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter = ',');

private:
    static std::vector<std::string> split(const std::string& line, char delimiter);
};

}  // namespace Utils
