#include "Utils/CSV.hpp"
#include "Utils/Constants.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

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

bool CSV::write(const std::string& filename, const std::vector<std::vector<std::string>>& data, char delimiter) {
    const auto& filepath = Utils::OUTPUTDIR / filename;
    if (auto dir = filepath.parent_path(); !dir.empty())
        fs::create_directories(dir);

    std::ofstream ofs(filepath);
    if (!ofs.is_open())
        return false;

    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            ofs << row[i];
            if (i + 1 < row.size())
                ofs << delimiter;
        }
        ofs << '\n';
    }
    return true;
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
