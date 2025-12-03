#include <iostream>
#include <fstream>
#include <string>

long process_id(long id) {
    // This function checks if the ID consists of repeated patterns
    std::string id_str = std::to_string(id);
    size_t length = id_str.length();
    int patLen;
    for (patLen = 1; patLen <= length / 2; ++patLen) {
        if (length % patLen == 0) {
            std::string pattern = id_str.substr(0, patLen);
            bool allMatch = true;
            for (size_t pos = patLen; pos < length; pos += patLen) {
                if (id_str.substr(pos, patLen) != pattern) {
                    allMatch = false;
                    break;
                }
            }
            if (allMatch) {
                std::cout << id_str << " matches with pattern length " << patLen << std::endl;
                return id;
            }
        }
    }
    return 0;
}

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
            total += process_id(current);
        }
    }
    std::cout << "Total: " << total << std::endl;

    return 0;
}
