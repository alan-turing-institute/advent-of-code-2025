#include <iostream>
#include <fstream>
#include <string>
#include <array>


// Define the offsets for the 3x3 neighborhood
constexpr std::array<std::pair<int,int>, 8> OFFSETS {{
    {-1, -1},
    {-1,  0},
    {-1,  1},
    { 0, -1},
    { 0,  1},
    { 1, -1},
    { 1,  0},
    { 1,  1}
}};

long remove_paper(
    std::vector<std::vector<char>>& grid,
    std::vector<std::vector<char>>& new_grid,
    int width,
    int height
) {
    //  Iterate through the grid and update new_grid based on conditions
    int new_row, new_col, around;
    long count = 0;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (grid[row][col] == '@') {
                around = 0;
                for (const auto& offset : OFFSETS) {
                    new_row = row + offset.first;
                    new_col = col + offset.second;
                    if (new_row >= 0 && new_row < height && new_col >= 0 && new_col < width) {
                        if (grid[new_row][new_col] == '@') {
                            around++;
                        }
                    }
                }
                if (around < 4) {
                    new_grid[row][col] = 'X';
                    count++;
                }
            }
        }
    }
    return count;
}

int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;
    int width, height = 0;
    

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    // Get the dimensions of the grid
    while (std::getline(file, line)) {
        width = line.length();
        height++;
    }
    std::cout << "Width: " << width << ", Height: " << height << std::endl;

    // Initialize a 2D grid of characters
    std::vector<std::vector<char>> grid(height, std::vector<char>(width, 0));

    // Reset file read position to the beginning
    file.clear();
    file.seekg(0);

    // Fill the grid with characters from the file
    for (int row = 0; row < height; row++) {
        std::getline(file, line);
        for (int col = 0; col < width; col++) {
            grid[row][col] = line[col];
        }
    }
    
    // Make a copy of the grid
    std::vector<std::vector<char>> new_grid = grid;

    long total_paper = 0;

    while (true) {

        long count = remove_paper(grid, new_grid, width, height);

        grid = new_grid;
        total_paper += count;

        // Print the grid to verify
        for (int row = 0; row < height; row++) {
            for (int col = 0; col < width; col++) {
                std::cout << new_grid[row][col];
                if (new_grid[row][col] == 'X') {
                    grid[row][col] = '.';
                }
            }
            std::cout << std::endl;
        }
        new_grid = grid;

        std::cout << "Removed this round: " << count << std::endl << std::endl;

        if (count == 0) {
            break;
        }
    }

    std::cout << "Total paper: " << total_paper << std::endl;


    file.close();
    return 0;
}

