#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

constexpr char ROCK = 1;
constexpr char PAPER = 2;
constexpr char SCISSORS = 3;

inline int get_result(const char& myChoice, const char& opponentChoice) {
    if (myChoice == SCISSORS and opponentChoice == ROCK) { return 0; }
    else if (myChoice == ROCK and opponentChoice == SCISSORS) { return 6; }
    else if (myChoice == opponentChoice) { return 3; }
    else if (myChoice > opponentChoice) { return 6; }
    return 0;
}

int main() {
    int score = 0;
    auto is = std::ifstream("scores.txt");
    std::string line;
    if (is.is_open()) {
        for (std::string line; std::getline(is, line); ) {
            char other = line.at(0) + 1 - 'A'; // convert to 1,2,3;
            char mine = line.at(2) + 1 - 'X'; // convert to 1,2,3
            score += mine + get_result(mine, other);
        }
    }
    else {
        std::cerr << "Could not open file scores.txt" << std::endl;
        return 1;
    }
    std::cout << "My score is " << score << std::endl;
    return 0;
}
