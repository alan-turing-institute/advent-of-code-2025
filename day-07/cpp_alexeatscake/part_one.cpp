#include <iostream>
#include <fstream>
#include <string>

/**
 * @brief Reads a grid from a file and calculates its dimensions and the total 
 *      number of splitters.
 *
 * @param file An input file stream containing the grid data.
 *      Each line represents a row of the grid, and '^' characters are counted 
 *      as splitters.
 * @return A tuple containing:
 *      - The number of columns in the grid (int).
 *      - The number of rows in the grid (int).
 *      - The total number of '^' characters in the grid (int).
 */
std::tuple<int, int, int> getGridInfo(
    std::ifstream& file
) {
    int grid_x_points, grid_y_points, total_num_spliters;
    std::string line;
    // Read the first section (ranges)
    std::getline(file, line);
    grid_x_points = line.length();
    grid_y_points = 1;
    while (std::getline(file, line)) {
        grid_y_points++;
        for (char c : line) {
            if (c == '^') {
                total_num_spliters++;
            }
        }
        if (grid_x_points != line.length()) {
            std::cerr << "Error: Inconsistent line lengths in grid." << std::endl;
            exit(1);
        }
    }
    // Reset the file stream to the beginning for further processing if needed
    file.clear();
    file.seekg(0, std::ios::beg);
    return std::make_tuple(grid_x_points, grid_y_points, total_num_spliters);
}

/**
 * @brief Populates a 2D grid with characters read from a file.
 *
 * @param file An input file stream containing the grid data.
 * @param grid A 2D vector to be populated with characters from the file.
 * @param grid_x_points The number of columns in the grid.
 * @param grid_y_points The number of rows in the grid.
 * 
 * @return void
 */

void populateGrid(
    std::ifstream& file,
    std::vector<std::vector<char>>& grid,
    const int grid_x_points,
    const int grid_y_points
) {
    std::string line;
    int y = 0;
    while (std::getline(file, line) && y < grid_y_points) {
        for (int x = 0; x < grid_x_points; x++) {
            grid[y][x] = line[x];
        }
        y++;
    }
    // Reset the file stream to the beginning for further processing if needed
    file.clear();
    file.seekg(0, std::ios::beg);
}

/**
 * @brief Follows the tachyon manifold through the grid, marking the path and 
 *      counting splits.
 *
 * @param grid A 2D vector representing the grid.
 * @param grid_x_points The number of columns in the grid.
 * @param grid_y_points The number of rows in the grid.
 * 
 * @return The total number of splits encountered.
 */
int followTachyonManifold(
    std::vector<std::vector<char>>& grid,
    const int grid_x_points,
    const int grid_y_points
) {
    int x = 0, y = 0;
    int total_splits = 0;
    for (x = 0; x < grid_x_points; x++) {
        if (grid[0][x] == 'S') {
            grid[1][x] = '|'; // Mark the starting point
            break; // Found the starting point
        }
    }
    for (y = 1; y < grid_y_points; y++) {
        for (x = 0; x < grid_x_points; x++) {
            if (grid[y - 1][x] == '|') {
                if (grid[y][x] == '^') {
                    total_splits++;
                    grid[y][x - 1] = '|';
                    grid[y][x + 1] = '|';
                } else {
                    grid[y][x] = '|';
                }
            }
        }
    }

    return total_splits;
}



int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;    

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    int grid_x_points, grid_y_points, total_num_spliters;
    std::tie(grid_x_points, grid_y_points, total_num_spliters) = getGridInfo(file);

    std::cout << "Grid Information:" << std::endl << 
        "Grid X Points: " << grid_x_points << std::endl << 
        "Grid Y Points: " << grid_y_points << std::endl << 
        "Total Number of Spliters: " << total_num_spliters << std::endl << std::endl;

    std::vector<std::vector<char>>
        grid(grid_y_points, std::vector<char>(grid_x_points));
    
    populateGrid(file, grid, grid_x_points, grid_y_points);

    int total_splits = followTachyonManifold(grid, grid_x_points, grid_y_points);

    for (const auto& row : grid) {
        for (const auto& cell : row) {
            std::cout << cell;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << 
        "\nTotal Splits Encountered: " << total_splits << std::endl;


    file.close();
    return 0;
}

