#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include <array>
#include <map>

const std::string INPUT_FILE = "input.txt";


typedef std::array<char, 3> ServerID;
typedef std::map<ServerID, std::vector<ServerID>> ServerMap;

ServerMap readServersFromFile(const std::string& filename) {
    ServerMap servers;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    boost::regex pattern("(\\w{3}):((?: \\w{3})*)");
    ServerID this_id = {'\0', '\0', '\0'};
    std::vector<ServerID> this_connections;
    std::string s_connections;

    while (std::getline(inputFile, line)) {
        boost::smatch matches;
        if (boost::regex_match(line, matches, pattern)) {

            this_id = {matches[1].str()[0], matches[1].str()[1], matches[1].str()[2]};
            s_connections = matches[2].str();
            this_connections.clear();
            for (size_t i = 0; i < s_connections.size(); i += 4) {
                if (s_connections[i] == ' ') {
                    this_connections.push_back({
                        s_connections[i + 1],
                        s_connections[i + 2],
                        s_connections[i + 3]
                    });
                }
            }
            servers[this_id] = this_connections;
        }
    }
    return servers;
}

void printServers(const ServerMap& servers) {
    for (const auto& server : servers) {
        const ServerID& id = server.first;
        const std::vector<ServerID>& connections = server.second;

        std::cout << id[0] << id[1] << id[2] << ":";
        for (const auto& conn : connections) {
            std::cout << " " << conn[0] << conn[1] << conn[2];
        }
        std::cout << std::endl;
    }
}

struct PathCounter {
    long visit_none;
    long visit_fft;
    long visit_dac;
    long visit_both;
};

PathCounter& operator+=(PathCounter& a, const PathCounter& b) {
    a.visit_none += b.visit_none;
    a.visit_fft  += b.visit_fft;
    a.visit_dac  += b.visit_dac;
    a.visit_both += b.visit_both;
    return a;
}

typedef std::map<ServerID, PathCounter> PathCache;

PathCounter pathSearch(
    const ServerMap& servers,
    const ServerID& location,
    PathCache& cache
) {
    if (cache.find(location) != cache.end()) {
        return cache[location];
    }

    if (location == ServerID{'o', 'u', 't'}) {
        return PathCounter{1, 0, 0, 0};
    }

    auto it = servers.find(location);
    PathCounter total_paths{0, 0, 0, 0};
    for (const auto& conn : it->second) {
        total_paths += pathSearch(servers, conn, cache);
    }

    if (location == ServerID{'f', 'f', 't'}) {
        total_paths.visit_fft += total_paths.visit_none;
        total_paths.visit_none = 0;
        total_paths.visit_both += total_paths.visit_dac;
        total_paths.visit_dac = 0;
    } else if (location == ServerID{'d', 'a', 'c'}) {
        total_paths.visit_dac += total_paths.visit_none;
        total_paths.visit_none = 0;
        total_paths.visit_both += total_paths.visit_fft;
        total_paths.visit_fft = 0;
    }

    cache[location] = total_paths;

    return total_paths;
}

int main() {
    // Read locations from the input file
    ServerMap servers = readServersFromFile(INPUT_FILE);

    // printServers(servers);

    PathCache cache;

    PathCounter total_paths = pathSearch(servers, ServerID{'s', 'v', 'r'}, cache);

    long overall_total = (
        total_paths.visit_none +
        total_paths.visit_fft +
        total_paths.visit_dac +
        total_paths.visit_both
    );

    std::cout << "Total paths from 'svr' to 'out': " << overall_total << std::endl <<
        "  Without visiting 'fft' or 'dac': " << total_paths.visit_none << std::endl <<
        "  Visiting 'fft' only: " << total_paths.visit_fft << std::endl <<
        "  Visiting 'dac' only: " << total_paths.visit_dac << std::endl <<
        "  Visiting both 'fft' and 'dac': " << total_paths.visit_both << std::endl;

    return 0;
}

