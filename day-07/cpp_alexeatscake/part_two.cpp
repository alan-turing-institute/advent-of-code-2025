#include <iostream>
#include <fstream>
#include <string>
#include <numeric>


/**
 * @brief Reads the grid information from the input file.
 * 
 * @param file The input file stream.
 * 
 * @return A tuple containing 
 *      - grid_x_points (width of the grid)
 *      - grid_y_points (height of the grid)
 *      - total_num_spliters (total number of splitters in the grid - this is an
 *          upper bound to the total number of splits encountered)    
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
 * @brief Populates the grid from the input file.
 * 
 * @param file The input file stream.
 * @param grid The 2D vector to populate with grid data.
 * @param grid_x_points The width of the grid.
 * @param grid_y_points The height of the grid.
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
 * @brief Follows the tachyon manifold through the grid and calculates time lines.
 * 
 * @param grid The 2D vector representing the grid.
 * @param time_grid The 2D vector to store time lines at each grid point.
 * @param grid_x_points The width of the grid.
 * @param grid_y_points The height of the grid.
 * 
 * @return The total number of time lines reaching the bottom of the grid.
 */
long followTachyonManifold(
    std::vector<std::vector<char>>& grid,
    std::vector<std::vector<long>>& time_grid,
    const int grid_x_points,
    const int grid_y_points
) {
    int x = 0, y = 0;
    for (x = 0; x < grid_x_points; x++) {
        if (grid[0][x] == 'S') {
            grid[1][x] = '|'; // Mark the starting point
            time_grid[1][x] += 1;
            break; // Found the starting point
        }
    }
    for (y = 1; y < grid_y_points; y++) {
        for (x = 0; x < grid_x_points; x++) {
            if (grid[y - 1][x] == '|') {
                if (grid[y][x] == '^') {
                    grid[y][x - 1] = '|';
                    time_grid[y][x - 1] += time_grid[y - 1][x];
                    grid[y][x + 1] = '|';
                    time_grid[y][x + 1] += time_grid[y - 1][x];
                } else {
                    grid[y][x] = '|';
                    time_grid[y][x] += time_grid[y - 1][x];
                }
            }
        }
    }

    long total_time_lines = std::accumulate(
        time_grid.back().begin(),
        time_grid.back().end(),
        0L
    );

    return total_time_lines;
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

    std::vector<std::vector<long>>
        time_grid(grid_y_points, std::vector<long>(grid_x_points, 0L));
    
    populateGrid(file, grid, grid_x_points, grid_y_points);

    long total_time_lines = followTachyonManifold(
        grid, time_grid, grid_x_points, grid_y_points
    );

    for (int y = 0; y < grid_y_points; y++) {
        for (int x = 0; x < grid_x_points; x++) {
            if (grid[y][x] == '|') {
                std::cout << time_grid[y][x] % 10; // Print last digit of time lines
            } else {
            std::cout << grid[y][x];
            }
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << 
        "\nTotal Time Lines: " << total_time_lines << std::endl;

    file.close();
    return 0;
}

