#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>

/**
 * @brief Counts the number of calculations and numbers in a file.
 *
 * This function reads through a file stream and calculates two values:
 * 1. The number of calculation operators ('+' or '*') present in the last line of the file.
 * 2. The total number of lines in the file, excluding the calculation line.
 *
 * The file stream is reset to the beginning after processing, allowing further operations on the file.
 *
 * @param file A reference to an input file stream (std::ifstream) that is already opened.
 * @return A std::pair<int, int> where:
 *         - The first element is the count of calculation operators ('+' or '*') in the last line.
 *         - The second element is the count of lines in the file, excluding the last line.
 */
std::pair<int, int> countCalculationsAndNumbers(
    std::ifstream& file
) {
    int num_calculations = 0;
    int num_of_numbers = -1;  // Need to adjust (-1) for the calculation line
    std::string line;
    std::string previous_line;
    // Read the first section (ranges)
    while (std::getline(file, line)) {
        previous_line = line;
        num_of_numbers++;
    }
    std::cout << line << std::endl;
    for (char c : previous_line) {
        if (c == '+' || c == '*') {
            num_calculations++;
        }
    }
    // Reset the file stream to the beginning for further processing if needed
    file.clear();
    file.seekg(0, std::ios::beg);
    return std::make_pair(num_calculations, num_of_numbers);
}

/**
 * @brief Fills a grid with values from a file and extracts operations.
 * 
 * This function reads data from the provided input file stream and populates
 * a 2D grid with integer values. It also extracts a sequence of operations
 * (characters) from the file and stores them in a separate vector.
 * 
 * @param file The input file stream to read data from.
 * @param grid A 2D vector to be filled with integer values. The grid is 
 *             assumed to have dimensions [num_calculations][num_of_numbers].
 * @param operations A vector to store the operations (characters) extracted 
 *                   from the file. The size of this vector should match 
 *                   num_calculations.
 * @param num_calculations The number of calculations (rows in the grid).
 * @param num_of_numbers The number of numbers (columns in the grid).
 * 
 * @return void - The function modifies the grid and operations vector in place.
 */
void fillGrid(
    std::ifstream& file,
    std::vector<std::vector<int>>& grid,
    std::vector<char>& operations,
    int num_calculations,
    int num_of_numbers
) {
    std::string line;
    int cal_idx = 0;
    
    // Read the number grid values
    for (int num_idx = 0; num_idx < num_of_numbers; ++num_idx) {
        cal_idx = 0;
        std::getline(file, line);
        std::istringstream iss(line);
        int value;
        while (iss >> value) {
            grid[cal_idx][num_idx] = value;
            cal_idx++;
        }
    }

    // Read the operations line
    std::getline(file, line);
    std::istringstream iss(line);
    char operation;
    cal_idx = 0;
    while (iss >> operation) {
        operations[cal_idx] = operation;
        cal_idx++;
    }
}


/**
 * @brief Performs calculations on a grid based on specified operations.
 * 
 * This function processes a 2D grid of integers and applies a series of
 * operations (addition or multiplication) to compute results for each number.
 * 
 * @param grid A 2D vector containing integer values to be processed.
 * @param operations A vector of characters representing the operations to be
 *     applied between rows of the grid. Each character should be either '+' or '*'.
 * @param num_calculations The number of calculation steps (rows in the grid).
 * @param num_of_numbers The number of numbers (columns in the grid).
 * 
 * @return A vector of integers containing the results of the calculations.
 *     This vector has a size equal to num_calculations.
 */ 
std::vector<long> doCalculations(
    const std::vector<std::vector<int>>& grid,
    const std::vector<char>& operations,
    int num_calculations,
    int num_of_numbers
) {
    std::vector<long> results(num_calculations, 0L);

    for (int calc_idx = 0; calc_idx < num_calculations; ++calc_idx) {
        for (int num_idx = 0; num_idx < num_of_numbers; ++num_idx) {
            if (num_idx == 0) {
                results[calc_idx] = grid[calc_idx][num_idx];
            } else {
                if (operations[calc_idx] == '+') {
                    results[calc_idx] += grid[calc_idx][num_idx];
                } else if (operations[calc_idx] == '*') {
                    results[calc_idx] *= grid[calc_idx][num_idx];
                }
                else {
                    std::cerr << "Error: Unknown operation '" 
                        << operations[calc_idx] << "'." << std::endl;
                }
            }
        }
    }

    return results;
}


int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;    

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    int num_calculations, num_of_numbers;
    std::tie(num_calculations, num_of_numbers) = countCalculationsAndNumbers(file);


    std::cout << "Number of Calculations: " << num_calculations << std::endl;
    std::cout << "Number of Numbers: " << num_of_numbers << std::endl;

    std::vector<std::vector<int>> 
        grid(num_calculations, std::vector<int>(num_of_numbers, 0));

    std::vector<char> operations(num_calculations);

    fillGrid(file, grid, operations, num_calculations, num_of_numbers);

    std::vector<long> results = doCalculations(
        grid, operations, num_calculations, num_of_numbers
    );

    long total = std::accumulate(results.begin(), results.end(), 0L);

    for (int i = 0; i < results.size(); ++i) {
        std::cout << "Result " << i << ": " << results[i];
        if (operations[i] == '+') {
            std::cout << " <- Addition of : ";
        } else if (operations[i] == '*') {
            std::cout << " <- Multiplication of : ";
        }
        for (int j = 0; j < num_of_numbers; ++j) {
            std::cout << grid[i][j];
            if (j < num_of_numbers - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Total of results: " << total << std::endl;

    file.close();
    return 0;
}

