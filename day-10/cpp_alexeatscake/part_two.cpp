/* 
 * I found the idea to this this online at this page:
 * https://www.reddit.com/r/adventofcode/comments/1pk87hl/2025_day_10_part_2_bifurcate_your_way_to_victory/
 * 
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <boost/regex.hpp>


const std::string INPUT_FILE = "input.txt";

typedef std::vector<std::vector<int>> Buttons;

struct Machine {
    std::vector<int> joltages;
    Buttons buttons;
};

/*
 * @brief Reads the machine configurations from the input file.
 * This only reads the joltages and the button configurations.
 * @param filename The name of the input file.
 * @return A vector of Machine structs representing the configurations.
*/
std::vector<Machine> readMachinesFromFile(const std::string& filename) {
    std::vector<Machine> machines;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    std::string s_joltages, s_buttons;
    std::vector<int> joltages;
    std::vector<int> one_button;
    Buttons buttons;

    boost::regex re("(\\[[\\.#]+\\]) ((?:\\([\\d,]+\\) )+)(\\{[\\d,]+\\})");
    boost::smatch match;

    for (std::string line; std::getline(inputFile, line); ) {
        if (boost::regex_search(line, match, re)) {
            s_buttons = match.str(2);
            s_joltages = match.str(3);
        }
        else {
            std::cerr << "Line format incorrect: " << line << std::endl;
        }

        joltages.clear();
        std::istringstream s_jolt_stream(s_joltages.substr(1, s_joltages.size() - 2));
        std::string joltage_str;
        while (std::getline(s_jolt_stream, joltage_str, ',')) {
            joltages.push_back(std::stoi(joltage_str));
        }

        buttons.clear();
        
        for (const char& c : s_buttons) {
            if (c == '(') {
                one_button.clear();
            }
            else if (c == ')') {
                buttons.push_back(one_button);
            }
            else if (isdigit(c)) {
                one_button.push_back(c - '0');
            }
        }   

        machines.push_back({joltages, buttons});
    }

    return machines;
}


/*
 * @brief Prints the machine configuration to the console.
 * @param machine The Machine struct to print. joltages then buttons.
*/
void printMachine(
    const Machine& machine
) {
    for (const auto& jolts : machine.joltages) {
        std::cout << jolts << " ";
    }
    std::cout << " - ";
    for (const auto& button : machine.buttons) {
        std::cout << "(";
        for (const auto& index : button) {
            std::cout << index << " ";
        }
        std::cout << ") ";
    }
    std::cout << std::endl;
}


typedef std::vector<std::vector<bool>> ButtonCombinations;

/*
 * @brief Generates all possible combinations of button presses.
 * @param buttons The Buttons configuration.
 * @return A vector of vectors, each representing a combination of button presses.
*/
ButtonCombinations allButtonCombinations(
    const Buttons& buttons
) {
    ButtonCombinations all_combinations;
    size_t total_combinations = 1UL << buttons.size(); // 2^n combinations
    std::vector<bool> combination(buttons.size(), false);

    for (size_t i = 0; i < total_combinations; ++i) {
        for (size_t j = 0; j < buttons.size(); ++j) {
            combination[j] = (i & (1UL << j)) != 0;
        }
        all_combinations.push_back(combination);
    }

    return all_combinations;
}


struct PressAttempt {
    std::vector<int> remaining_joltages;
    long total_presses;
};

/*
 * @brief Attempts all button press combinations to achieve even joltages.
 * @param buttons The Buttons configuration.
 * @param starting_joltages The initial joltages before any button presses.
 * @return A vector of PressAttempt structs representing successful attempts.
*/
std::vector<PressAttempt> correctParityPresses(
    const Buttons& buttons,
    const std::vector<int>& starting_joltages
) {
    std::vector<PressAttempt> successful_attempts;
    std::vector<int> current_joltages = starting_joltages;
    long current_presses = 0;
    bool all_valid = true;
    
    ButtonCombinations button_combinations = allButtonCombinations(buttons);
    for (const std::vector<bool>& combination : button_combinations) {
        current_joltages = starting_joltages;
        current_presses = 0;
        for (size_t i = 0; i < combination.size(); ++i) {
            if (combination[i]) {
                for (const int& index : buttons[i]) {
                    current_joltages[index]--;
                }
                current_presses++;
            }
        }
        all_valid = true;
        for (const int& joltage : current_joltages) {
            if ((joltage % 2 != 0) || (joltage < 0)) {
                all_valid = false;
                break;
            }
        }
        if (all_valid) {
            successful_attempts.push_back(
                {current_joltages, current_presses}
            );
        }
    }
    return successful_attempts;
}

/*
 * @brief Checks if all joltages are zero.
 * @param joltages The vector of joltages to check.
 * @return True if all joltages are zero, false otherwise.
*/
bool isAllZeroJoltages(
    const std::vector<int>& joltages
) {
    for (const int& joltage : joltages) {
        if (joltage != 0) {
            return false;
        }
    }
    return true;
}

long recursiveAttemptFinder(
    const Buttons& buttons,
    const std::vector<int>& starting_joltages,
    std::map<std::vector<int>, long>& press_cache
) {
    if (press_cache.find(starting_joltages) != press_cache.end()) {
        return press_cache[starting_joltages];
    }

    std::vector<PressAttempt> all_attempts = correctParityPresses(
        buttons,
        starting_joltages
    );

    std::vector<int> half_joltages;
    std::vector<long> number_presses;
    for (const PressAttempt& attempt : all_attempts) {
        if (isAllZeroJoltages(attempt.remaining_joltages)) {
            number_presses.push_back(attempt.total_presses);
        }
        else {
            half_joltages.clear();
            for (const int& joltage : attempt.remaining_joltages) {
                half_joltages.push_back(joltage / 2);
            }
            long sub_presses = recursiveAttemptFinder(
                buttons,
                half_joltages,
                press_cache
            );
            if (sub_presses != std::numeric_limits<long>::max()) {
                number_presses.push_back(attempt.total_presses + sub_presses * 2);
            }
        }
    }
    long min_presses = std::numeric_limits<long>::max();
    for (const long& presses : number_presses) {
        if (presses < min_presses) {
            min_presses = presses;
        }
    }

    press_cache[starting_joltages] = min_presses;
    return min_presses;
}

long findOptimalPresses(
    const Machine& machine
) {
    std::map<std::vector<int>, long> press_cache = {};

    return recursiveAttemptFinder(
        machine.buttons,
        machine.joltages,
        press_cache
    );
}


int main() {
    // Read locations from the input file
    std::vector<Machine> machines = readMachinesFromFile(INPUT_FILE);

    long total_presses = 0;
    for (const Machine& machine : machines) {
        printMachine(machine);
        long presses = findOptimalPresses(machine);
        std::cout << "Optimal presses needed: " << presses << std::endl;
        total_presses += presses;
    }

    std::cout << "Total presses needed: " << total_presses << std::endl;

    return 0;
}

