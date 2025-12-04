#include <iostream>
#include <fstream>
#include <string>
#include <array>


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

    // Define the offsets for the 3x3 neighborhood
    std::vector<std::pair<int,int>> offsets = {
        {-1, -1},
        {-1,  0},
        {-1,  1},
        { 0, -1},
        { 0,  1},
        { 1, -1},
        { 1,  0},
        { 1,  1}
    };


    //  Iterate through the grid and update new_grid based on conditions
    int new_row, new_col, around;
    long count = 0;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (grid[row][col] == '@') {
                around = 0;
                for (const auto& offset : offsets) {
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


    // Print the grid to verify
    for (const auto& row : new_grid) {
        for (const auto& ch : row) {
            std::cout << ch;
        }
        std::cout << std::endl;
    }

    std::cout << "Total paper: " << count << std::endl;


    file.close();
    return 0;
}

