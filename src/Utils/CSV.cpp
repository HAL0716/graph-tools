#include "Utils/CSV.hpp"

namespace Utils {

std::vector<std::vector<std::string>> CSV::read(const std::string& filename, char delimiter) {
    std::ifstream ifs(filename);
    if (!ifs.is_open())
        throw std::runtime_error("Failed to open file: " + filename);

    std::vector<std::vector<std::string>> result;
    std::string line;

    while (std::getline(ifs, line))
        result.push_back(split(line, delimiter));

    return result;
}

std::vector<std::string> CSV::split(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);

    return tokens;
}

}  // namespace Utils