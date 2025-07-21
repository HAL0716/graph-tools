#pragma once

#include <filesystem>
#include <string_view>

namespace Utils {

inline const std::string_view SYMBOLS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
inline const std::filesystem::path OUTPUTDIR("output");

} // namespace Utils