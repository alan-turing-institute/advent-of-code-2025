#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include <eigen3/Eigen/Dense>
#include <array>
#include <map>
#include <boost/functional/hash.hpp>

const std::string INPUT_FILE = "input.txt";

const int NUM_PRESENT_TYPES = 6, PRESENT_SIZE = 3;

typedef Eigen::Array<bool, PRESENT_SIZE, PRESENT_SIZE> PresentShape;

struct Tree {
    int x;
    int y;
    std::array<int, NUM_PRESENT_TYPES> present_counts;
};


std::pair<std::array<PresentShape, NUM_PRESENT_TYPES>, std::vector<Tree>> 
    readPresentsAndTrees(
        const std::string& filename
) {
    std::array<PresentShape, NUM_PRESENT_TYPES> present_shapes;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    int pres_idx, row_idx, col_idx;
    PresentShape this_present_shape;
    for (pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        std::getline(inputFile, line); // Do Nothing
        for (row_idx = 0; row_idx < PRESENT_SIZE; ++row_idx) {
            std::getline(inputFile, line);
            for (col_idx = 0; col_idx < PRESENT_SIZE; ++col_idx) {
                this_present_shape(row_idx, col_idx) = (line[col_idx] == '#');
            }
        }
        present_shapes[pres_idx] = this_present_shape;
        std::getline(inputFile, line); // Do Nothing
    }

    std::vector<Tree> trees;
    std::string regex_string = R"((\d+)x(\d+):\s*)";
    for (pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        regex_string += R"((\d+)\s*)";
    }
    boost::regex tree_regex(regex_string);
    boost::smatch match;
    Tree this_tree;
    while (std::getline(inputFile, line)) {
        if (boost::regex_match(line, match, tree_regex)) {
            this_tree.x = std::stoi(match[1]);
            this_tree.y = std::stoi(match[2]);
            for (pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
                this_tree.present_counts[pres_idx] = std::stoi(match[3 + pres_idx]);
            }
            trees.push_back(this_tree);
        } else {
            std::cerr << "Line did not match expected format: " << line << std::endl;
        }
    }

    return std::make_pair(present_shapes, trees);
}

void printPresentShape(const PresentShape& present_shape) {
    for (int row_idx = 0; row_idx < PRESENT_SIZE; ++row_idx) {
        for (int col_idx = 0; col_idx < PRESENT_SIZE; ++col_idx) {
            std::cout << (present_shape(row_idx, col_idx) ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

void printTreeInfo(const Tree& tree) {
    std::cout << "Tree with size (" << tree.x << ", " << tree.y << "): ";
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        std::cout << "Present " << pres_idx << ": " << tree.present_counts[pres_idx] << " ";
    }
    std::cout << std::endl;
}

bool containsPresentShape(
    const std::vector<PresentShape>& shapes,
    const PresentShape& candidate
) {
    for (const auto& s : shapes) {
        if ((s == candidate).all()) {
            return true;
        }
    }
    return false;
}

std::vector<PresentShape> generateAllRotationsAndFlips(const PresentShape& original) {
    std::vector<PresentShape> transformations = {};
    PresentShape current = original;

    for (int flip = 0; flip < 2; ++flip) {
        for (int rot = 0; rot < 4; ++rot) {
            if (!containsPresentShape(transformations, current)) {
                transformations.push_back(current);
            }
            // Rotate 90 degrees clockwise
            PresentShape rotated;
            for (int i = 0; i < PRESENT_SIZE; ++i) {
                for (int j = 0; j < PRESENT_SIZE; ++j) {
                    rotated(j, PRESENT_SIZE - 1 - i) = current(i, j);
                }
            }
            current = rotated;
        }
        // Flip horizontally
        PresentShape flipped;
        for (int i = 0; i < PRESENT_SIZE; ++i) {
            for (int j = 0; j < PRESENT_SIZE; ++j) {
                flipped(i, j) = current(i, PRESENT_SIZE - 1 - j);
            }
        }
        current = flipped;
    }

    return transformations;
}

typedef std::array<std::vector<PresentShape>, NUM_PRESENT_TYPES> AllPresentShapes;
typedef std::array<std::vector<PresentShape>, NUM_PRESENT_TYPES + 1> AllPresentPlusNull;

AllPresentShapes getAllPresentShapes(
    const std::array<PresentShape, NUM_PRESENT_TYPES>& base_shapes
) {
    AllPresentShapes all_transformations;
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        all_transformations[pres_idx] = generateAllRotationsAndFlips(
            base_shapes[pres_idx]
        );
    }
    return all_transformations;
}

typedef Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> UnderTree;

bool canPlacePresent(
    const UnderTree& under_tree,
    const PresentShape& present_shape,
    const int x_idx,
    const int y_idx
) {
    for (int row_idx = 0; row_idx < PRESENT_SIZE; ++row_idx) {
        for (int col_idx = 0; col_idx < PRESENT_SIZE; ++col_idx) {
            if (present_shape(row_idx, col_idx)) {
                if (under_tree(y_idx + row_idx -1, x_idx + col_idx -1)) {
                    return false;
                }
            }
        }
    }
    return true;
}

void addPresentToUnderTree(
    UnderTree& under_tree,
    const PresentShape& present_shape,
    const int x_idx,
    const int y_idx
) {
    for (int row_idx = 0; row_idx < PRESENT_SIZE; ++row_idx) {
        for (int col_idx = 0; col_idx < PRESENT_SIZE; ++col_idx) {
            if (present_shape(row_idx, col_idx)) {
                under_tree(y_idx + row_idx -1, x_idx + col_idx -1) = true;
            }
        }
    }
}

void removePresentFromUnderTree(
    UnderTree& under_tree,
    const PresentShape& present_shape,
    const int x_idx,
    const int y_idx
) {
    for (int row_idx = 0; row_idx < PRESENT_SIZE; ++row_idx) {
        for (int col_idx = 0; col_idx < PRESENT_SIZE; ++col_idx) {
            if (present_shape(row_idx, col_idx)) {
                under_tree(y_idx + row_idx -1, x_idx + col_idx -1) = false;
            }
        }
    }
}

void printUnderTree(const UnderTree& under_tree) {
    for (int row_idx = 0; row_idx < under_tree.rows(); ++row_idx) {
        for (int col_idx = 0; col_idx < under_tree.cols(); ++col_idx) {
            std::cout << (under_tree(row_idx, col_idx) ? '#' : '.');
        }
        std::cout << std::endl;
    }
}

typedef std::vector<std::array<bool, 3> > RowStatus;

struct CurrentState {
    int x_loc;
    int y_loc;
    std::array<int, NUM_PRESENT_TYPES + 1> present_counts;
    RowStatus row_status;
};

bool operator==(const CurrentState& a, const CurrentState& b) {
    return a.x_loc == b.x_loc &&
           a.y_loc == b.y_loc &&
           a.present_counts == b.present_counts &&
           a.row_status == b.row_status;
}

struct CurrentStateHash {
    std::size_t operator()(const CurrentState& s) const {
        std::size_t seed = 0;
        boost::hash_combine(seed, s.x_loc);
        boost::hash_combine(seed, s.y_loc);
        boost::hash_combine(seed, s.present_counts);
        boost::hash_combine(seed, s.row_status);
        return seed;
    }
};

typedef std::unordered_map<CurrentState, bool, CurrentStateHash> StateCache;

typedef std::array<int, NUM_PRESENT_TYPES + 1> Volumes;

bool isSpaceAvailable(
    const Tree& tree,
    const Volumes& volumes,
    const std::array<int, NUM_PRESENT_TYPES + 1>& present_counts,
    const int x_idx,
    const int y_idx
) {
    int available_space = (tree.x * (tree.y + 1 - y_idx)) - x_idx + 1;
    int required_space = 0;
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES + 1; ++pres_idx) {
        required_space += present_counts[pres_idx] * volumes[pres_idx];
    }
    return required_space <= available_space;
}

bool recursiveFitPresent(
    const Tree& tree,
    const AllPresentPlusNull& all_presents,
    const Volumes& volumes,
    UnderTree& under_tree,
    std::array<int, NUM_PRESENT_TYPES + 1>& present_counts,
    int x_idx,
    int y_idx,
    StateCache& cache
) {
    if (y_idx == tree.y - 1) {
        return true; // Successfully placed all presents
    }

    if (!isSpaceAvailable(
        tree,
        volumes,
        present_counts,
        x_idx,
        y_idx
    )) {
        return false;
    }

    RowStatus row_status;

    for (int row = x_idx - 1; row < tree.x; ++row) {
        row_status.push_back({
            under_tree(y_idx -1, row),
            under_tree(y_idx, row),
            under_tree(y_idx +1, row)
        });
    }

    CurrentState current_state{x_idx, y_idx, present_counts, row_status};

    if (cache.find(current_state) != cache.end()) {
        return cache[current_state];
    }

    int next_x_idx, next_y_idx;

    if (x_idx == tree.x -2) {
        next_x_idx = 1;
        next_y_idx = y_idx + 1;
    } else {
        next_x_idx = x_idx + 1;
        next_y_idx = y_idx;
    }

    bool success = false;
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES + 1; ++pres_idx) {
        if (present_counts[pres_idx] <= 0) {
            continue;
        }
        for (const PresentShape& this_shape : all_presents[pres_idx]) {
            if (canPlacePresent(under_tree, this_shape, x_idx, y_idx)) {
                addPresentToUnderTree(under_tree, this_shape, x_idx, y_idx);
                present_counts[pres_idx] -= 1;
                success = recursiveFitPresent(
                    tree,
                    all_presents,
                    volumes,
                    under_tree,
                    present_counts,
                    next_x_idx,
                    next_y_idx,
                    cache       
                );
                cache[current_state] = success;
                if (success) {
                    return true;
                }
                removePresentFromUnderTree(under_tree, this_shape, x_idx, y_idx);
                present_counts[pres_idx] += 1;
            }
        }

    }
    return false;
}

bool doPresentsFitUnderTree(
    const Tree& tree,
    const AllPresentShapes& all_presents_shapes,
    Volumes& volumes
) {
    AllPresentPlusNull all_presents;
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        all_presents[pres_idx] = all_presents_shapes[pres_idx];
    }
    all_presents[NUM_PRESENT_TYPES] = {PresentShape::Constant(false)};
    
    UnderTree under_tree = UnderTree::Constant(tree.y, tree.x, false);

    std::array<int, NUM_PRESENT_TYPES + 1> present_counts;
    present_counts[NUM_PRESENT_TYPES] = (tree.x - 2) * (tree.y - 2); // Null presents
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        present_counts[pres_idx] = tree.present_counts[pres_idx];
        present_counts[NUM_PRESENT_TYPES] -= tree.present_counts[pres_idx];
    }

    StateCache cache = {};
    
    bool success = recursiveFitPresent(
        tree,
        all_presents,
        volumes,
        under_tree,
        present_counts,
        1,
        1,
        cache
    );
    printUnderTree(under_tree);
    for (const int& count: present_counts) {
        std::cout << count << " ";
    }
    std::cout << std::endl;
    return success;
}

int main() {
    // Read locations from the input file
    auto [present_shapes, trees] = readPresentsAndTrees(
        INPUT_FILE
    );

    AllPresentShapes all_presents = getAllPresentShapes(present_shapes);

    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        std::cout << "Present Type " << pres_idx << ":\n";
        std::cout << "This has " << 
            all_presents[pres_idx].size() << " unique rotations/flips:\n";
        printPresentShape(present_shapes[pres_idx]);
        std::cout << std::endl;
    }

    Volumes volumes;
    for (int pres_idx = 0; pres_idx < NUM_PRESENT_TYPES; ++pres_idx) {
        volumes[pres_idx] = present_shapes[pres_idx].count();
    }
    volumes[NUM_PRESENT_TYPES] = 0; // Null present

    std::cout << "Trees Information:\n";

    int tree_idx = 0;
    int tree_total = trees.size();
    int fitting_count = 0;
    for (const auto& tree : trees) {
        ++tree_idx;
        std::cout << "Tree " << tree_idx << " of " << tree_total << ":\n";
        printTreeInfo(tree);
        bool fits = doPresentsFitUnderTree(
            tree,
            all_presents,
            volumes
        );
        if (fits) {
            std::cout << "Presents fit under the tree.\n";
            fitting_count += 1;
        } else {
            std::cout << "Presents do NOT fit under the tree.\n";
        }
    }
    std::cout << std::endl << 
        fitting_count << " out of " << tree_total << " trees can fit their presents"
        << std::endl;

    return 0;
}

