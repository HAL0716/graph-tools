#pragma once

#include <iostream>
#include <limits>
#include <string>

namespace Utils {

class Input {
public:
    template <typename T>
    static T read(const std::string& prompt = "") {
        T value;
        while (true) {
            if (!prompt.empty()) std::cout << prompt;
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Invalid input. Try again.\n";
            } else {
                return value;
            }
        }
    }

    template <typename T>
    static T readInRange(const std::string& prompt, T min, T max) {
        T value;
        while (true) {
            value = read<T>(prompt);
            if (value < min || value > max) {
                std::cerr << "Value must be between " << min << " and " << max << ". Try again.\n";
            } else {
                return value;
            }
        }
    }
};

}  // namespace Utils