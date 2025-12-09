#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>

// const std::string INPUT_FILE = "example_input.txt";
// const int NUMBER_OF_CONNECTIONS = 10;

const std::string INPUT_FILE = "input.txt";
const int NUMBER_OF_CONNECTIONS = 1000;

/**
 * Structure to hold 3D coordinates.
 */
struct Location {
    long x;
    long y;
    long z;
};

/**
 * @brief Reads a file containing 3D coordinates to a vector of Location structs.
 * 
 * @param filename The name of the file to read.
 * 
 * @return std::vector<Location> A vector of Location structs.
 */
std::vector<Location> getLocations(std::string filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<Location> locations;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        exit(1);
    }

    while (std::getline(file, line)) {
        int x, y, z;
        char comma; // to consume the commas
        std::istringstream ss(line);
        ss >> x >> comma >> y >> comma >> z;
        locations.push_back({x, y, z});
    }

    return locations;
}

/**
 * @brief Calculates the Euclidean distance between two 3D locations.
 * 
 * @param loc1 The first location.
 * @param loc2 The second location.
 * 
 * @return double The Euclidean distance between loc1 and loc2.
 */
double calculateDistance(const Location& loc1, const Location& loc2) {
    return std::sqrt(
        (loc2.x - loc1.x) * (loc2.x - loc1.x) +
        (loc2.y - loc1.y) * (loc2.y - loc1.y) +
        (loc2.z - loc1.z) * (loc2.z - loc1.z)
    );
}

struct indexPair {
    size_t index1;
    size_t index2;
    double distance;
};

std::vector<indexPair> findClosestLocations(
    const std::vector<Location>& locations,
    size_t numConnections
) {
    std::vector<indexPair> closestPairs(
        numConnections, {0, 0, std::numeric_limits<double>::max()}
    );
    size_t biggest_pair_index = 0;

    for (size_t i = 0; i < locations.size(); ++i) {
        for (size_t j = i + 1; j < locations.size(); ++j) {
            double dist = calculateDistance(locations[i], locations[j]);
            if (dist < closestPairs[biggest_pair_index].distance) {
                closestPairs[biggest_pair_index] = {i, j, dist};

                // Update the index of the biggest distance pair
                biggest_pair_index = 0;
                for (size_t k = 1; k < closestPairs.size(); ++k) {
                    if (closestPairs[k].distance >
                        closestPairs[biggest_pair_index].distance) {
                        biggest_pair_index = k;
                    }
                }
            }
        }
    }

    return closestPairs;
}


/**
 * @brief Determines the sizes of connected trees formed by the closest location pairs.
 * 
 * @param linkedPairs A vector of indexPair structs holding connected location pairs.
 * 
 * @return std::vector<size_t> A vector containing the sizes of each connected tree.
 */
std::vector<size_t> getTreeSizes(
    const std::vector<indexPair>& linkedPairs
) {
    std::vector<size_t> tree_sizes;
    std::vector<bool> used_connections(linkedPairs.size(), false);
    std::vector<bool> new_used_connections(linkedPairs.size(), false);
    std::unordered_set<size_t> current_connected = {};

    for (size_t i_con = 0; i_con < linkedPairs.size(); ++i_con) {
        if (used_connections[i_con]) {
            continue;  // Already used in another tree
        }
        new_used_connections[i_con] = true;
        current_connected.insert(linkedPairs[i_con].index1);
        current_connected.insert(linkedPairs[i_con].index2);
        size_t connections_found = 0;
        while (connections_found < current_connected.size()) {
            connections_found = current_connected.size();
            for (size_t k_con = 0; k_con < linkedPairs.size(); ++k_con) {
                if (used_connections[k_con]) {
                    continue; // Already used in another tree
                }
                if (current_connected.count(linkedPairs[k_con].index1) > 0 ||
                    current_connected.count(linkedPairs[k_con].index2) > 0) {
                    // This connection connects to the current tree
                    new_used_connections[k_con] = true;
                    current_connected.insert(linkedPairs[k_con].index1);
                    current_connected.insert(linkedPairs[k_con].index2);
                }
            }
        }
        // Mark all new used connections as used
        for (size_t k_con = 0; k_con < linkedPairs.size(); ++k_con) {
            if (new_used_connections[k_con]) {
                used_connections[k_con] = true;
            }
        }
        std::fill(new_used_connections.begin(), new_used_connections.end(), false);

        // Store the size of the current tree
        tree_sizes.push_back(current_connected.size());
        current_connected.clear();
    }

    return tree_sizes;

}


int main() {
    // Read locations from the input file
    std::vector<Location> locations = getLocations(INPUT_FILE);

    // Print locations
    std::cout << "Locations:" << std::endl;
    int count = 0;
    for (const auto& loc : locations) {
        std::cout << "Index: " << count++ << " Coords: "
            << loc.x << ", " << loc.y << ", " << loc.z << std::endl;
    }
    std::cout << std::endl;

    // Find and print the closest location pairs
    std::vector<indexPair> connected_locations = findClosestLocations(
        locations, NUMBER_OF_CONNECTIONS
    );
    
    // Print closest location pairs
    std::cout << "Closest Locations:" << std::endl;
    for (const auto& pair : connected_locations) {
        std::cout << "Index: " << pair.index1
                  << " Index: " << pair.index2
                  << " Distance: " << pair.distance << std::endl;
    }
    std::cout << std::endl;

    // Get the tree sizes
    std::vector<size_t> tree_sizes = getTreeSizes(connected_locations);

    std::sort(tree_sizes.begin(), tree_sizes.end(), std::greater<size_t>());

    // Print tree sizes
    std::cout << "Tree Sizes:" << std::endl;
    int counter = 0;
    for (const auto& size : tree_sizes) {
        std::cout << size << ", ";
        counter++;
        if (counter % 10 == 0) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << std::endl;

    // Calculate and print the product of the top three tree sizes
    long product = tree_sizes[0] * tree_sizes[1] * tree_sizes[2];
    std::cout << "Product of Top Three Tree Sizes: " << product << std::endl;


    return 0;
}

