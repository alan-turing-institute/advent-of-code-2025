#include <iostream>
#include <fstream>
#include <string>


int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;    

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }


    int num_of_ranges = 0;
    // Read the first section (ranges)
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;  // Stop reading at the first empty line
        }
        std::cout << line << std::endl;
        num_of_ranges++;
    }

    std::vector<long> start_value(num_of_ranges);
    std::vector<long> end_value(num_of_ranges);

    // Reset file read position to the beginning
    file.clear();
    file.seekg(0);

    size_t i = 0, dash_loc = 0;
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;  // Stop reading at the first empty line
        }
        dash_loc = line.find('-');
        std::cout << "Dash located at: " << dash_loc << std::endl;
        start_value[i] = std::stol(line.substr(0, dash_loc));
        end_value[i] = std::stol(line.substr(dash_loc + 1));
        i++;
    }

    std::cout << "Finished reading ranges." << std::endl;
    std::cout << "Number of ranges: " << num_of_ranges << std::endl;

    long value, total = 0;
    bool after_start, before_end, found;
    // work through the values
    while (std::getline(file, line)) {
        if (line.empty()) {
            break;  // Stop reading at the first empty line
        }
        found = false;
        value = std::stol(line);
        for (size_t j = 0; j < num_of_ranges; j++) {
            after_start = (value >= start_value[j]);
            before_end = (value <= end_value[j]);
            if (after_start && before_end) {
                std::cout << "Value " << value << " is within range " 
                          << start_value[j] << "-" << end_value[j] << std::endl;
                total++;
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout << "Value " << value << " is not within any range." << std::endl;
        }
    }

    std::cout << "Total values within ranges: " << total << std::endl;


    file.close();
    return 0;
}

