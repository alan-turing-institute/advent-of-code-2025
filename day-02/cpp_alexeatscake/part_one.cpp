#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("input.txt");
    std::string chunk;
    size_t dashLoc;
    long startNum, endNum, current, length;
    long total = 0;
    std::string current_str, half1, half2;

    if (!file.is_open()) {
        std::cerr << "Could not open file\n";
        return 1;
    }

    // Read values separated by commas:
    while (std::getline(file, chunk, ',')) {
        std::cout << "Processing chunk: " << chunk << std::endl;
        dashLoc = chunk.find('-');
        startNum = std::stol(chunk.substr(0, dashLoc));
        endNum = std::stol(chunk.substr(dashLoc + 1));
        for (current = startNum; current <= endNum; ++current) {
            current_str = std::to_string(current);
            length = current_str.length();
            if (length % 2 == 0) {
                half1 = current_str.substr(0, length / 2);
                half2 = current_str.substr(length / 2, length / 2);
                if (half1 == half2) {
                    std::cout << current_str << std::endl;
                    total += current;
                }
            }
        }
    }
    std::cout << "Total: " << total << std::endl;

    return 0;
}
