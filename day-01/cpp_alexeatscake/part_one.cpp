#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;
    int dial = 50;
    int counter = 0;

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
        // Extract everything after the first character
  
        dial = ((dial % 100) + 100) % 100;
        if (dial == 0) {
            counter++;
        }

        std::cout << "Current dial: " << dial << ", Counter: " << counter << std::endl;
        
    }

    file.close();
    return 0;
}

