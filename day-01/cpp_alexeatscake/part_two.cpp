#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;
    int dial = 50;
    int counter = 0;
    int full_cycles = 0;
    bool on_zero = false;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    while (std::getline(file, line)) {
        std::string numberPart = line.substr(1);
        int value = std::stoi(numberPart);
        if (!line.empty() && line[0] == 'R') {
            dial += value;
        }
        else if (!line.empty() && line[0] == 'L') {
            dial -= value;
        }
        else if (!line.empty()) {
            std::cerr << "Error: Invalid line format." << std::endl;
            continue; // Skip invalid lines
        }
        if (dial < 0) {
            full_cycles = (-dial / 100) + 1;
            if (on_zero) {
                full_cycles -= 1;
            }
        } else if (dial >= 100) {
            full_cycles = dial / 100;
        } else if (dial == 0) {
            full_cycles = 1;
        } else {
            full_cycles = 0;
        }
        counter += full_cycles;
        dial = ((dial % 100) + 100) % 100;
        on_zero = (dial == 0);

        std::cout << "Turned: " << line << ", Current dial: " << dial << ", Counter: " << counter << std::endl;
        
    }

    file.close();
    return 0;
}

