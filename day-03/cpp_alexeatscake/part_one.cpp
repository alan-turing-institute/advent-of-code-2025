#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;
    long total = 0;
    int best_loc1, best_loc2, current_value;
    int best_score1 = -1, best_score2 = -1;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        for (size_t i = 0; i < line.length() - 1; ++i) {
            current_value = line[i] - '0';  // Convert char to int
            if (current_value > best_score1) {
                best_score1 = current_value;
                best_loc1 = i;
            }
        }
        for (size_t i = best_loc1 + 1; i < line.length(); ++i) {
            current_value = line[i] - '0';  // Convert char to int
            if (current_value > best_score2) {
                best_score2 = current_value;
                best_loc2 = i;
            }
        }
        std::cout << "Best Joltage: " << (best_score1 * 10 + best_score2) << std::endl;
        total += (best_score1 * 10 + best_score2);
        best_score1 = -1;
        best_score2 = -1;

        
    }

    std::cout << "Total: " << total << std::endl;

    file.close();
    return 0;
}

