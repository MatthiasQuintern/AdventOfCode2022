#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <gz-util/string/utility.hpp>
#include <gz-util/string/from_string.hpp>
#include <gz-util/string/to_string.hpp>

#define PART2

#ifdef PART2
constexpr unsigned CAVE_WIDTH = 401;
#else
constexpr unsigned CAVE_WIDTH = 201;
#endif
constexpr unsigned CAVE_HEIGHT = 170;
constexpr char EMPTY = '.';
constexpr char WALL = '#';
constexpr char SAND = 'O';


class Cave {
    public:
        Cave(const char* filename);
        char at(unsigned x, unsigned y) const { return cave.at(transform(x) + y * charsPerRow); }
        void set(unsigned x, unsigned y, char c) { cave.at(transform(x) + y * charsPerRow) = c; }
        void print() const { std::cout << cave << std::endl; };
        // ret true if sand stays somewhere
        bool addSand(unsigned x, unsigned y);
    private:
        // return true if sand was moved
        bool moveSand(unsigned& x, unsigned& y);
        // position to x-index, 500 should be in the middle
        unsigned transform(unsigned x) const { return x - 500 + ((charsPerRow - 1) / 2) ; }  
        std::string cave;
        unsigned charsPerRow = CAVE_WIDTH + 1;  // +1 for newline
        unsigned rows = CAVE_HEIGHT;
};

Cave::Cave(const char* filename) {
    cave.resize(charsPerRow * rows, EMPTY);
    for (std::string::size_type i = charsPerRow-1; i < cave.size(); i += charsPerRow) {
        cave[i] = '\n';
    }
    std::ifstream file(filename);
    std::vector<std::string_view> walls;
    std::string buf;
    unsigned maxY = 0;
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    while (std::getline(file, buf)) {
        walls = gz::util::splitStringInVector<std::string_view>(std::string_view(buf), " -> ");
        unsigned startX = 0, startY = 0;
        unsigned endX = 0, endY = 0;
        std::cout << gz::toString(walls);
        for (auto& wall : walls) {
            auto comma = wall.find(',');
            endX = gz::fromString<unsigned>(std::string(wall.substr(0, comma)));
            endY = gz::fromString<unsigned>(std::string(wall.substr(comma + 1)));
            if (endY > maxY) { maxY = endY; }
            if (startX != 0) {
                this->set(endX, endY, WALL);   // will be left out by while loops
                if (startX == endX) {
                    while (startY != endY) {
                        this->set(startX, startY, WALL);
                        startY += startY < endY ? 1 : -1;
                    }
                }
                else {
                    while (startX != endX) {
                        this->set(startX, startY, WALL);
                        startX += startX < endX ? 1 : -1;
                    }
                }
            }
            startX = endX;
            startY = endY;
        }
    }
#ifdef PART2
    // draw line
    for (unsigned x = 0; x < charsPerRow - 1; x++) {
        cave.at(x + charsPerRow * (maxY+2)) = WALL;
    }
#endif
}

bool Cave::addSand(unsigned x, unsigned y) {
    if (at(x, y) == SAND) { return false; }  // entrance blocked
    set(x, y, SAND);
    bool hasMoved = moveSand(x, y);
    while (hasMoved) {
        hasMoved = moveSand(x, y);
        /* printf("Falling sand: x=%u, y=%u\n", x, y); */
        if (y == rows - 1) {  // falling through floor
            set(x, y, EMPTY);
            return false;
        }
    }
    return true;
}


bool Cave::moveSand(unsigned& x, unsigned& y) {
    // try falling down
    if (at(x, y+1) == EMPTY) {
        set(x, y, EMPTY);
        set(x, ++y, SAND);
        return true;
    }
    // try fall to left
    else if (at(x-1, y+1) == EMPTY) {
        set(x, y, EMPTY);
        set(--x, ++y, SAND);
        return true;
    }
    // try fall to right
    else if (at(x+1, y+1) == EMPTY) {
        set(x, y, EMPTY);
        set(++x, ++y, SAND);
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    const char* filename;
    if (argc != 2) {
        /* printf("Need exactly one argument, the filename\n"); */
        /* return 1; */
        filename = "i-dont-like-sand.txt";
    }
    else {
        filename = argv[1];
    }
    Cave cave(filename);
    cave.print();
    unsigned sandCount = 0;
    while (cave.addSand(500, 0)) {
        /* std::cout << "." << std::endl; */
        sandCount++;
    }
    cave.set(500, 0, 'S');
    cave.print();
    printf("I DONT LIKE SAND %d/10!!!\n", sandCount);
}
