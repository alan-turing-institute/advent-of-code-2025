#include <iostream>
#include <fstream>
#include <string>
#include <array>


constexpr int NUM_BAT = 12;

struct GetScoreResult {
    int score;
    int loc;
};

GetScoreResult get_best_score(const std::string& line, int& start_pos, int& end_pos) {
    // This function finds the best score and its location in the given range
    // This will be used for each battery position
    int best_score = -1;
    int best_loc, current_value;
    for (int i = start_pos; i < end_pos; ++i) {
        int current_value = line[i] - '0';  // Convert char to int
        if (current_value > best_score) {
            best_score = current_value;
            best_loc = i;
        }
    }
    return {best_score, best_loc};
}

int main() {
    std::ifstream file("input.txt");  // the file you want to read
    std::string line;
    long total = 0;
    std::array<int, NUM_BAT> best_scores;
    std::array<int, NUM_BAT> best_locs;
    

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        int start_point = 0;
        int end_point;
        for (int i = 0; i < NUM_BAT; ++i) {
            int end_point = line.size() - (NUM_BAT - 1 - i);
            GetScoreResult gsr = get_best_score(line, start_point, end_point);
            best_scores[i] = gsr.score;
            best_locs[i] = gsr.loc;
            start_point = gsr.loc + 1;
        }
        long subtotal = 0;
        for (int i = 0; i < NUM_BAT; ++i) {
            subtotal = subtotal * 10 + best_scores[i];
        }
        std::string loc_string = "";
        for (int i = 0; i < NUM_BAT; ++i) {
            loc_string += std::to_string(best_locs[i]) + " ";
        }
        std::cout << "Subtotal: " << subtotal << std::endl;
        std::cout << "Best Locations: " << loc_string << std::endl;
        
        total += subtotal;
    }

    std::cout << "Total: " << total << std::endl;

    file.close();
    return 0;
}

