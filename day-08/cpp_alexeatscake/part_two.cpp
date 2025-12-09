#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>

const std::string INPUT_FILE = "input.txt";
// const std::string INPUT_FILE = "input.txt";

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

std::vector<indexPair> findAllPairs(
    const std::vector<Location>& locations
) {
    std::vector<indexPair> allPairs;

    for (size_t i = 0; i < locations.size(); ++i) {
        for (size_t j = i + 1; j < locations.size(); ++j) {
            double dist = calculateDistance(locations[i], locations[j]);
            allPairs.push_back({i, j, dist});
        }
    }

    return allPairs;
}

bool areAllConnected(
    const std::vector<indexPair>& linkedPairs,
    size_t total_locations
) {
    std::unordered_set<size_t> connected_locations;

    connected_locations.insert(linkedPairs[0].index1);
    connected_locations.insert(linkedPairs[0].index2);
    size_t connections_found = 0;
    while (connections_found < connected_locations.size()) {
        connections_found = connected_locations.size();
        for (size_t k_con = 0; k_con < linkedPairs.size(); ++k_con) {
            if (connected_locations.count(linkedPairs[k_con].index1) > 0 ||
                connected_locations.count(linkedPairs[k_con].index2) > 0) {
                // This connection connects to the current tree
                connected_locations.insert(linkedPairs[k_con].index1);
                connected_locations.insert(linkedPairs[k_con].index2);
            }
        }
    }
    return connected_locations.size() == total_locations;
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
    std::vector<indexPair> connected_locations = findAllPairs(locations);

    std::sort(
        connected_locations.begin(),
        connected_locations.end(),
        [](const indexPair& a, const indexPair& b) {
              return a.distance < b.distance;
        }
    );

    for (int i=0; i<20 && i < connected_locations.size(); ++i) {
        const auto& pair = connected_locations[i];
        std::cout << "Pair " << i << ": Index1: " << pair.index1
            << " Index2: " << pair.index2
            << " Distance: " << pair.distance << std::endl;
    }

    std::vector<indexPair> working_connections;
    for (const auto& pair : connected_locations) {
        working_connections.push_back(pair);
        if (areAllConnected(working_connections, locations.size())) {
            break;
        }
    }

    indexPair last_added = working_connections.back();
    std::cout << "\nLast added connection to connect all locations: "
        << "Index1: " << last_added.index1
        << " Index2: " << last_added.index2
        << " Distance: " << last_added.distance << std::endl;
    
    std::cout << "Location 1 coords: "
        << locations[last_added.index1].x << ", "
        << locations[last_added.index1].y << ", "
        << locations[last_added.index1].z << std::endl;
    std::cout << "Location 2 coords: "
        << locations[last_added.index2].x << ", "
        << locations[last_added.index2].y << ", "
        << locations[last_added.index2].z << std::endl << std::endl;

    std::cout << "Extention Size needed " << 
        locations[last_added.index1].x << " * " << locations[last_added.index2].x <<
        " = " << locations[last_added.index1].x * locations[last_added.index2].x << 
        std::endl;
    
    





    return 0;
}

