#include <cstdio>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr char ROCK = 1;
constexpr char PAPER = 2;
constexpr char SCISSORS = 3;
constexpr char LOSS = 1;
constexpr char DRAW = 2;
constexpr char WIN = 3;

inline int get_result(const char& myChoice, const char& opponentChoice) {
    if (myChoice == SCISSORS and opponentChoice == ROCK) { return 0; }
    else if (myChoice == ROCK and opponentChoice == SCISSORS) { return 6; }
    else if (myChoice == opponentChoice) { return 3; }
    else if (myChoice > opponentChoice) { return 6; }
    return 0;
}

inline char get_choice_for_result(const char& result, const int& opponentChoice) {
    if (result == DRAW) { return opponentChoice; }
    else if (result == WIN) { return opponentChoice < 3 ? opponentChoice + 1 : 1; }
    else if (result == LOSS) { return opponentChoice > 1 ? opponentChoice - 1 : 3; }
    throw std::runtime_error("Reached end of get_choice_for_result");
}

int main() {
    int score_1 = 0;
    int score_2 = 0;
    auto is = std::ifstream("scores.txt");
    std::string line;
    if (is.is_open()) {
        for (std::string line; std::getline(is, line); ) {
            char other = line.at(0) + 1 - 'A'; // convert to 1,2,3;
            char mine = line.at(2) + 1 - 'X'; // convert to 1,2,3
            score_1 += mine + get_result(mine, other);
            score_2 += (mine - 1) * 3 + get_choice_for_result(mine, other);
        }
    }
    else {
        std::cerr << "Could not open file scores.txt" << std::endl;
        return 1;
    }
    std::cout << "If XYZ is Rock/Paper/Scissors, my score is " << score_1 << std::endl;
    std::cout << "If XYZ is Loss/Draw/Win, my score is " << score_2 << std::endl;
    return 0;
}
