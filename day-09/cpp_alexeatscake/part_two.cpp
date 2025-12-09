#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

const std::string INPUT_FILE = "input.txt";

const std::array<std::pair<int, int>, 4> directions = {{
    {0, 1},
    {1, 0},
    {0, -1},
    {-1, 0}
}};

struct TileLocation {
    long x;
    long y;
};

std::vector<TileLocation> readLocationsFromFile(const std::string& filename) {
    std::vector<TileLocation> locations;
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return locations;
    }

    int x, y;
    char comma;
    for (std::string line; std::getline(inputFile, line); ) {
        std::istringstream iss(line);
        if (iss >> x >> comma >> y) {
            locations.push_back({x, y});
        }
    }

    return locations;
}

struct gridSize {
    long minX;
    long maxX;
    long minY;
    long maxY;
};

gridSize calculateGridSize(
    const std::vector<TileLocation>& locations
) {
    gridSize size = {LONG_MAX, LONG_MIN, LONG_MAX, LONG_MIN};

    for (const auto& loc : locations) {
        if (loc.x < size.minX) size.minX = loc.x;
        if (loc.x > size.maxX) size.maxX = loc.x;
        if (loc.y < size.minY) size.minY = loc.y;
        if (loc.y > size.maxY) size.maxY = loc.y;
    }
    // Add a border around the grid
    size.minX--;
    size.maxX++;
    size.minY--;
    size.maxY++;

    return size;
}

std::vector<std::vector<char>> createGrid(
    const std::vector<TileLocation>& locations,
    const gridSize& size
) {
    long width = size.maxX - size.minX + 1;
    long height = size.maxY - size.minY + 1;

    std::vector<std::vector<char>> grid(height, std::vector<char>(width, 'X'));

    grid.front() = std::vector<char>(width, '.');
    grid.back() = std::vector<char>(width, '.');
    for (auto& row : grid) {
        row.front() = '.';
        row.back() = '.';
    }

    return grid;
}

void tileLine(
    std::vector<std::vector<char>>& grid,
    const long s_gridX,
    const long s_gridY,
    const long e_gridX,
    const long e_gridY
) {
    long start_line, end_line;
    if (s_gridX == e_gridX) {
        // Vertical line
        start_line = std::min(s_gridY, e_gridY);
        end_line = std::max(s_gridY, e_gridY);
        for (long y = start_line; y <= end_line; ++y) {
            grid[y][s_gridX] = '#';
        }
    } else if (s_gridY == e_gridY) {
        // Horizontal line
        start_line = std::min(s_gridX, e_gridX);
        end_line = std::max(s_gridX, e_gridX);
        for (long x = start_line; x <= end_line; ++x) {
            grid[s_gridY][x] = '#';
        }
    } else {
        std::cerr << "Only horizontal or vertical lines should exist." << std::endl;
    }
}

void addRedTilesToGrid(
    std::vector<std::vector<char>>& grid,
    const std::vector<TileLocation>& locations,
    const gridSize& size
) {
    // Do first line which connects to last
    long s_gridX = locations.back().x - size.minX;
    long s_gridY = locations.back().y - size.minY;
    long e_gridX = locations.front().x - size.minX;
    long e_gridY = locations.front().y - size.minY;

    tileLine(grid, s_gridX, s_gridY, e_gridX, e_gridY);

    for (size_t i = 0; i < locations.size() - 1; ++i) {
        s_gridX = locations[i].x - size.minX;
        s_gridY = locations[i].y - size.minY;
        e_gridX = locations[i + 1].x - size.minX;
        e_gridY = locations[i + 1].y - size.minY;

        tileLine(grid, s_gridX, s_gridY, e_gridX, e_gridY);
    }
        
}

void addGreenTilesToGrid(
    std::vector<std::vector<char>>& grid
) {
    int iteration = 0, x, y, it_y, it_x;
    long tiles_found = -1, new_tiles_found = 0;
    bool adjacent_blank_tile = false;
    while (tiles_found < new_tiles_found) {
        iteration++;
        tiles_found = new_tiles_found;
        std::cout << "It. " << iteration << " Co. " << tiles_found << std::endl;
        for (it_y = 1; it_y < grid.size() - 1; ++it_y) {
            if (iteration % 2 == 0) {
                // Even iteration - process from bottom to top
                y = grid.size() - 1 - it_y;
            }
            else {
                y = it_y;
            }
            for (it_x = 1; it_x < grid[0].size() - 1; ++it_x) {
                if (iteration % 4 <= 1) {
                    // Even iteration pair - process from right to left
                    x = grid[0].size() - 1 - it_x;
                } else {
                    x = it_x;
                }
                if (grid[y][x] == 'X') {
                    adjacent_blank_tile = false;
                    for (const auto& dir : directions) {
                        if (grid[y + dir.second][x + dir.first] == '.') {
                            adjacent_blank_tile = true;
                            break;
                        }
                    }
                    if (adjacent_blank_tile) {
                        grid[y][x] = '.';
                        new_tiles_found++;
                    }
                }
            }
        }
    }
}

void printGrid(
    const std::vector<std::vector<char>>& grid
) {
    if (grid.size() < 30) {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Printing just top corner." << std::endl;
        for (size_t y = 0; y < 20; ++y) {
            for (size_t x = 0; x < 20; ++x) {
                std::cout << grid[y][x];
            }
            std::cout << std::endl;
        }
    }
}

struct Square {
    long loc_idx1;
    long loc_idx2;
    long area;
};

std::vector<Square> makeSquareList(
    const std::vector<TileLocation>& locations
) {
    std::vector<Square> square_list;
    long height, width, area;

    for (long i = 0; i < locations.size(); ++i) {
        for (long j = i + 1; j < locations.size(); ++j) {
            height = std::abs(locations[j].y - locations[i].y) + 1;
            width = std::abs(locations[j].x - locations[i].x) + 1;
            area = height * width;

            square_list.push_back({i, j, area});
        }
    }

    return square_list;
}

bool descendingAreaSort(
    const Square& a,
    const Square& b
) {
    return a.area > b.area;
}

bool isSquareAllTiled(
    const std::vector<std::vector<char>>& grid,
    const std::vector<TileLocation>& locations,
    const Square& square,
    const gridSize& size
) {
    long startX = std::min(
        locations[square.loc_idx1].x, locations[square.loc_idx2].x
    ) - size.minX;
    long endX = std::max(
        locations[square.loc_idx1].x, locations[square.loc_idx2].x
    ) - size.minX;
    long startY = std::min(
        locations[square.loc_idx1].y, locations[square.loc_idx2].y
    ) - size.minY;
    long endY = std::max(
        locations[square.loc_idx1].y, locations[square.loc_idx2].y
    ) - size.minY;

    // Start by checking the perimeter
    for (long x = startX; x <= endX; ++x) {
        if (grid[startY][x] == '.' || grid[endY][x] == '.') {
            return false;
        }
    }
    for (long y = startY; y <= endY; ++y) {
        if (grid[y][startX] == '.' || grid[y][endX] == '.') {
            return false;
        }
    }

    // Now check the interior
    for (long y = startY; y <= endY; ++y) {
        for (long x = startX; x <= endX; ++x) {
            if (grid[y][x] == '.') {
                return false;
            }
        }
    }

    return true;
}
    

int main() {
    // Read locations from the input file

    std::vector<TileLocation> locations = readLocationsFromFile(INPUT_FILE);

    std::cout << "Read " << locations.size() << " tile locations from file." << std::endl;

    gridSize size = calculateGridSize(locations);

    std::cout << "Grid Size - MinX: " << size.minX << ", MaxX: " << size.maxX <<
        ", MinY: " << size.minY << ", MaxY: " << size.maxY << std::endl;

    std::vector<std::vector<char>> grid = createGrid(locations, size);

    addRedTilesToGrid(grid, locations, size);

    printGrid(grid);

    addGreenTilesToGrid(grid);

    printGrid(grid);

    std::vector<Square> square_list = makeSquareList(locations);

    std::sort(square_list.begin(), square_list.end(), descendingAreaSort);

    std::cout << "Top 3 largest squares:" << std::endl;

    for (int i = 0; i < 3; ++i) {
        std::cout << "Square " << i + 1 << ": LocIdx1: " << square_list[i].loc_idx1 <<
            ", LocIdx2: " << square_list[i].loc_idx2 <<
            ", Area: " << square_list[i].area << std::endl;
    }

    Square largest_tiled_square;
    size_t s_idx = 0, total_s = square_list.size();
    for (s_idx = 0; s_idx < total_s; ++s_idx) {
        std::cout << "\rProgress: " << s_idx << "/" << total_s << std::flush;
        if (isSquareAllTiled(grid, locations, square_list[s_idx], size)) {
            largest_tiled_square = square_list[s_idx];
            break;
        }
    }
    std::cout << std::endl;

    std::cout << "Largest tiled square found: " <<
        "LocIdx1: " << largest_tiled_square.loc_idx1 <<
        ", LocIdx2: " << largest_tiled_square.loc_idx2 <<
        ", Area: " << largest_tiled_square.area << std::endl;


    return 0;
}

