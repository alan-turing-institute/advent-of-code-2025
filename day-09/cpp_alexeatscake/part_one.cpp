#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

const std::string INPUT_FILE = "input.txt";

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

struct Square {
    long loc_idx1;
    long loc_idx2;
    long area;
};

Square biggestSquare(
    const std::vector<TileLocation>& locations
) {
    Square largestSquare = {-1, -1, 0};
    long height, width, area;

    for (long i = 0; i < locations.size(); ++i) {
        for (long j = i + 1; j < locations.size(); ++j) {
            height = std::abs(locations[j].y - locations[i].y) + 1;
            width = std::abs(locations[j].x - locations[i].x) + 1;
            area = height * width;

            if (area > largestSquare.area) {
                largestSquare = {i, j, area};
            }
        }
    }

    return largestSquare;
}

int main() {
    // Read locations from the input file

    std::vector<TileLocation> locations = readLocationsFromFile(INPUT_FILE);

    std::cout << "Read " << locations.size() << " tile locations from file." << std::endl;

    for (const auto& loc : locations) {
        std::cout << "Tile Location - X: " << loc.x << ", Y: " << loc.y << std::endl;
    }

    // Find the biggest square
    Square largestSquare = biggestSquare(locations);

    std::cout << "The largest square is  between locations index " << 
        largestSquare.loc_idx1 << " and " << largestSquare.loc_idx2 << std::endl <<
        "Location 1 - X: " << 
        locations[largestSquare.loc_idx1].x << ", Y: " << 
        locations[largestSquare.loc_idx1].y << std::endl <<
        "Location 2 - X: " << 
        locations[largestSquare.loc_idx2].x << ", Y: " << 
        locations[largestSquare.loc_idx2].y << std::endl <<
        "With an area of " << largestSquare.area << std::endl;

    return 0;
}

