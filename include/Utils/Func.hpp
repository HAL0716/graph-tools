#pragma once

#include <string>

namespace Utils {

class Func {
public:
    // 小数点以下 precision 桁で double を文字列に変換
    static std::string toStrFix(double value, int precision);
};

} // namespace Utils