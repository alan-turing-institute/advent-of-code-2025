#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Boost libraries for regex and math
#include <boost/regex.hpp>
#include <boost/math/special_functions/binomial.hpp>

// Timing
#include <chrono>
#include <thread>

// Eigen library for matrix operations
#include <eigen3/Eigen/Dense>

const std::string INPUT_FILE = "input.txt";
const int MAX_COMBINATIONS = 1e5;

struct Machine {
    std::vector<int> joltages;
    std::vector<std::vector<int>> buttons;
};

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
    std::vector<std::vector<int>> buttons;

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

void printMachine(
    const Machine& machine
) {
    std::cout << "{ ";
    for (const auto& joltage : machine.joltages) {
        std::cout << joltage << " ";
    }
    std::cout << "} - ";
    for (const auto& button : machine.buttons) {
        std::cout << "(";
        for (const auto& index : button) {
            std::cout << index << " ";
        }
        std::cout << ") ";
    }
    std::cout << std::endl;
}


void printMachines(const std::vector<Machine>& machines) {
    for (const auto& machine : machines) {
        printMachine(machine);
    }
}

std::vector<std::vector<int>> addRoundOfCombinations(
    const std::vector<std::vector<int>>& combinations,
    int total_numbers
) {
    std::vector<std::vector<int>> new_combinations;
    std::vector<int> new_combination;
    int it_val;

    for (const auto& combination : combinations) {
        if (combination.empty()) {
            it_val = 0;
        }
        else {
            it_val = combination.back();
        }
        for (int i = it_val; i < total_numbers; ++i) {
            new_combination = combination;
            new_combination.push_back(i);
            new_combinations.push_back(new_combination);
        }
    }

    return new_combinations;
}

std::vector<std::vector<int>> allCombinationsWithReplacement(
    int number_to_choose,
    int total_numbers
) {
    std::vector<std::vector<int>> combinations;
    std::vector<int> new_combination;

    combinations.push_back(std::vector<int>());
    int it_val;

    while (combinations[0].size() < number_to_choose) {
        combinations = addRoundOfCombinations(combinations, total_numbers);
    }

    return combinations;
}

long buttonPressesNeeded(
    const Machine& machine
) {
    bool found_answer = false;
    long presses = *std::min_element(
        machine.joltages.begin(), machine.joltages.end()
    ) - 1;
    std::vector<int> current_joltages(machine.joltages.size(), 0);
    std::vector<std::vector<int>> all_combinations;
    if (boost::math::binomial_coefficient<double>(
            presses + machine.buttons.size() - 1,
            machine.buttons.size() - 1
        ) > MAX_COMBINATIONS) {
        std::cerr << "Warning: Initial combinations exceed " << MAX_COMBINATIONS
                  << std::endl;
        return -1;
    }
    all_combinations = allCombinationsWithReplacement(
            presses, machine.buttons.size()
    );
    std::vector<std::vector<int>> next_combinations = all_combinations;
    std::vector<int> button_sequence;
    int j_idx;
    bool overloaded;

    std::cout << std::endl;
    while (!found_answer) {
        presses++;
        all_combinations = addRoundOfCombinations(
            next_combinations, machine.buttons.size()
        );
        if (all_combinations.size() > MAX_COMBINATIONS) {
            std::cerr << "Warning: Combinations exceed " << MAX_COMBINATIONS << 
                " at " << presses << " presses!" << std::endl;
            return -1;
        }
        next_combinations.clear();
        std::cout << "\rTrying with " << presses << " presses, "
                  << all_combinations.size() << " combinations." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        for (const auto& combination : all_combinations) {
            current_joltages.assign(machine.joltages.size(), 0);

            for (const auto& button_index : combination) {
                for (const auto& light_index : machine.buttons[button_index]) {
                    current_joltages[light_index]++;
                }
            }

            if (current_joltages == machine.joltages) {
                found_answer = true;
                break;
            }

            for (j_idx = 0; j_idx < machine.joltages.size(); ++j_idx) {
                if (current_joltages[j_idx] > machine.joltages[j_idx]) {
                    overloaded = true;
                    break;
                }
            }
            if (!overloaded) {
                next_combinations.push_back(combination);
            }
            overloaded = false;
        }
    }

    return presses;
}

void printMachineFacts(
    const Machine& machine
) {
    int smallest_joltage = *std::min_element(
        machine.joltages.begin(),
        machine.joltages.end()
    );
    int num_of_buttons = machine.buttons.size();
    int num_of_joltages = machine.joltages.size();

    std::cout << "Smallest joltage: " << smallest_joltage << std::endl;
    std::cout << "Number of buttons: " << num_of_buttons << std::endl;
    std::cout << "Number of joltages: " << num_of_joltages << std::endl;

    double starting_combinations = boost::math::binomial_coefficient<double>(
        smallest_joltage + num_of_buttons - 1,
        num_of_buttons - 1
    );
    std::cout << "Starting combinations: " << starting_combinations << std::endl;


    Eigen::MatrixXd button_matrix(num_of_joltages, num_of_buttons);
    button_matrix.setZero();
    for (int b = 0; b < num_of_buttons; b++) {
        for (const auto& j_idx : machine.buttons[b]) {
            button_matrix(j_idx, b) = 1;
        }
    }

    std::cout << "Button matrix:\n" << button_matrix << std::endl;

    Eigen::FullPivLU<Eigen::MatrixXd> lu(button_matrix);

    std::cout << "Button matrix rank: " << lu.rank() << std::endl;

    std::cout << "Null space " << lu.kernel().cols() << " vectors:\n" 
              << lu.kernel() << std::endl;


}

int main() {
    // Read locations from the input file
    std::vector<Machine> machines = readMachinesFromFile(INPUT_FILE);

    int m_idx = 0, solved = 0, total_machines = machines.size();
    long current_presses, total_presses = 0;
    for (const auto& machine : machines) {
        std::cout << std::endl << "+++++++++" << std::endl;
        std::cout << "Machine " << (m_idx) << " of " << total_machines << ": ";
        printMachine(machine);
        printMachineFacts(machine);
        m_idx++;
        current_presses = buttonPressesNeeded(machine);
        if (current_presses == -1) {
            std::cout << "Skipping machine due to excessive combinations."
                      << std::endl;
            continue;
        }
        solved++;
        total_presses += current_presses;
        std::cout << "Presses needed for machine: " << current_presses << std::endl;
    }

    std::cout << std::endl <<
        "Total machines solved: " << solved << " of " << total_machines
        << std::endl;

    std::cout << std::endl <<
        "Total presses needed: " << total_presses << std::endl;

    return 0;
}

