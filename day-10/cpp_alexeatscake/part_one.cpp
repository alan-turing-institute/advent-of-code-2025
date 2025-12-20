#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <boost/regex.hpp>

const std::string INPUT_FILE = "input.txt";

struct Machine {
    std::vector<bool> lights;
    std::vector<std::vector<int>> buttons;
};

std::vector<Machine> readMachinesFromFile(const std::string& filename) {
    std::vector<Machine> machines;

    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }

    std::string s_lights, s_buttons;
    std::vector<bool> lights;
    std::vector<int> one_button;
    std::vector<std::vector<int>> buttons;

    boost::regex re("(\\[[\\.#]+\\]) ((?:\\([\\d,]+\\) )+)(\\{[\\d,]+\\})");
    boost::smatch match;

    for (std::string line; std::getline(inputFile, line); ) {
        if (boost::regex_search(line, match, re)) {
            s_lights = match.str(1);
            s_buttons = match.str(2);
        }
        else {
            std::cerr << "Line format incorrect: " << line << std::endl;
        }

        lights.clear();
        for (const char& c : s_lights) {
            if (c == '#') {
                lights.push_back(true);
            }
            else if (c == '.') {
                lights.push_back(false);
            }
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

        machines.push_back({lights, buttons});
    }

    return machines;
}

void printMachine(
    const Machine& machine
) {
    for (const auto& light : machine.lights) {
        std::cout << light;
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


void printMachines(const std::vector<Machine>& machines) {
    for (const auto& machine : machines) {
        printMachine(machine);
    }
}

std::vector<std::vector<int>> allCombinationsWithReplacement(
    int number_to_choose,
    int total_numbers
) {
    std::vector<std::vector<int>> combinations;
    std::vector<std::vector<int>> next_combinations;
    std::vector<int> new_combination;

    combinations.push_back(std::vector<int>());
    int it_val;

    while (combinations[0].size() < number_to_choose) {
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
                next_combinations.push_back(new_combination);
            }
        }
        combinations = next_combinations;
        next_combinations.clear();
    }

    return combinations;
}

long buttonPressesNeeded(
    const Machine& machine
) {
    bool found_answer = false;
    long presses = 0;
    std::vector<bool> current_lights(machine.lights.size(), false);
    std::vector<std::vector<int>> all_combinations;
    std::vector<int> button_sequence;

    while (!found_answer) {
        presses++;
        all_combinations = allCombinationsWithReplacement(
            presses, machine.buttons.size()
        );

        for (const auto& combination : all_combinations) {
            current_lights.assign(machine.lights.size(), false);

            for (const auto& button_index : combination) {
                for (const auto& light_index : machine.buttons[button_index]) {
                    current_lights[light_index] = !current_lights[light_index];
                }
            }

            if (current_lights == machine.lights) {
                found_answer = true;
                break;
            }
        }
        
    }

    return presses;
}


int main() {
    // Read locations from the input file
    std::vector<Machine> machines = readMachinesFromFile(INPUT_FILE);

    long current_presses, total_presses = 0;
    for (const auto& machine : machines) {
        current_presses = buttonPressesNeeded(machine);
        total_presses += current_presses;
        printMachine(machine);
        std::cout << "Presses needed for machine: " << current_presses << std::endl;
    }

    std::cout << std::endl <<
        "Total presses needed: " << total_presses << std::endl;

    return 0;
}

