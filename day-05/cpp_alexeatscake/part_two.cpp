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
        start_value[i] = std::stol(line.substr(0, dash_loc));
        end_value[i] = std::stol(line.substr(dash_loc + 1));
        i++;
    }

    std::cout << "Finished reading ranges." << std::endl;
    std::cout << "Number of ranges: " << num_of_ranges << std::endl;

    std::vector<long> new_start_value = start_value;
    std::vector<long> new_end_value = end_value;
    std::vector<bool> valid_range(num_of_ranges, true);
    int number_clashes = 1;
    long overlap_start, overlap_end, overlap_size;

    while (number_clashes > 0) {
        number_clashes = 0;

        for (size_t j = 0; j < num_of_ranges; j++) {
            if (valid_range[j] == false) {
                continue; // Skip invalid ranges
            }
            for (size_t k = j + 1; k < num_of_ranges; k++) {
                if (valid_range[k] == false) {
                    continue; // Skip invalid ranges
                }
                overlap_start = std::max(start_value[j], start_value[k]);
                overlap_end = std::min(end_value[j], end_value[k]);
                overlap_size = overlap_end - overlap_start + 1;
                if (overlap_size > 0) {
                    // There is an overlap
                    new_start_value[j] = std::min(start_value[j], start_value[k]);
                    new_end_value[j] = std::max(end_value[j], end_value[k]);

                    // Mark the k-th range as invalid since it's merged into j-th range
                    valid_range[k] = false;

                    // Indicate that a clash was found
                    number_clashes++;
                    break;
                }
                
            }
        }

        start_value = new_start_value;
        end_value = new_end_value;

        for (size_t j = 0; j < num_of_ranges; j++) {
            if (valid_range[j] == false) {
                continue; // Skip invalid ranges
            }
            std::cout << "Range " << j << ": " <<
                start_value[j] << " - " << end_value[j] << std::endl;
        }

        std::cout << "Number of clashes found in this iteration: " << number_clashes << std::endl;
    }

    long total = 0;

    for (size_t j = 0; j < num_of_ranges; j++) {
        if (valid_range[j] == false) {
            continue; // Skip invalid ranges
        }
        total += (end_value[j] - start_value[j] + 1);
    }

    std::cout << "Total values within ranges: " << total << std::endl;


    file.close();
    return 0;
}

