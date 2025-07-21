#include "Utils/Func.hpp"
#include <sstream>
#include <iomanip>

namespace Utils {

std::string Func::toStrFix(double value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

} // namespace Utils