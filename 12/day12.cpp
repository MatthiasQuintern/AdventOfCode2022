#include <fstream> 
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_set>
#include <gz-util/container/queue.hpp>


constexpr char UP = (1 << 1);
constexpr char DOWN = (1 << 2);
constexpr char LEFT = (1 << 3);
constexpr char RIGHT = (1 << 4);

using ssize = std::string::size_type;

struct XY {
    ssize x;
    ssize y;
    bool operator==(const XY& other) const { return this->x == other.x && this->y == other.y; };
    struct Hash {
        size_t operator()(const XY& xy) const {
            return (std::hash<ssize>()(xy.x) << 1) ^ (std::hash<ssize>()(xy.y));
        }
    };
};

class String2D {
    public:
        String2D(ssize w, std::string::size_type h, char c) : s((w + 1) * h, c), width(w), height(h) {
            for (ssize i = width; i < s.size(); i += width + 1) {
                s.at(i) = '\n';
            }
        }
        String2D(std::string&& str) : s(std::move(str)) {
            width = s.find('\n');
            height = s.size() / (width + 1);
        }
        const char& at(ssize x, std::string::size_type y) const {
            assert(x < width);
            assert(y < height);
            return s.at(x + y * (width + 1));
        }
        char& at(ssize x, std::string::size_type y) {
            assert(x < width);
            assert(y < height);
            return s.at(x + y * (width + 1));
        }
        void set(ssize x, std::string::size_type y, char c) {
            assert(x < width);
            assert(y < height);
            s.at(x + y * (width + 1)) = c;
        }
        XY find(char c) {
            ssize i = s.find(c);
            if (i != std::string::npos) { return indexToPos(i); }
            return XY{std::string::npos, std::string::npos};
        }
        XY indexToPos(ssize i) { return XY{.x = i % (width + 1), .y = i / (width + 1) }; }
        inline const ssize& getWidth() const { return width; }
        inline const ssize& getHeight() const { return height; }
        const std::string& getString() const { return s; }
    private:
        std::string s;
        ssize width, height;
};

bool isReachable(char c) {
    return c <= 1;
}

String2D createNodes(const String2D& hill) {
    String2D dirs(hill.getWidth(), hill.getHeight(), 0);
    for (size_t x = 0; x < hill.getWidth(); x++) {
        for (size_t y = 0; y < hill.getHeight(); y++) {
            char c = hill.at(x, y);
            if (x > 0) {
                if (isReachable(hill.at(x - 1, y) - c)) { dirs.at(x, y) |= LEFT; }
            }
            if (x < hill.getWidth() - 1) {
                if (isReachable(hill.at(x + 1, y) - c)) { dirs.at(x, y) |= RIGHT; }
            }
            if (y > 0) {
                if (isReachable(hill.at(x, y - 1) - c)) { dirs.at(x, y) |= UP; }
            }
            if (y < hill.getHeight() - 1) {
                if (isReachable(hill.at(x, y + 1) - c)) { dirs.at(x, y) |= DOWN; }
            }
        }
    }
    return dirs;
}

ssize shortestPath(const String2D& nodes, XY start, XY end) {
    gz::Queue<XY> waitlists[2];
    unsigned currentWaitlist = 0;
    unsigned otherWaitlist = 1;
    std::unordered_set<XY, XY::Hash> processed;
    ssize length = 0;
    auto queue = [&processed](gz::Queue<XY>& q, XY xy) {
        if (!processed.contains(xy)) {
            processed.insert(xy);
            q.push_back(xy);
        }
    };
    queue(waitlists[currentWaitlist], start);
    while (waitlists[0].hasElement() || waitlists[1].hasElement()) {
        while (waitlists[currentWaitlist].hasElement()) {
            XY& pos = waitlists[currentWaitlist].getRef();
            if (pos == end) { goto done; }
            char c = nodes.at(pos.x, pos.y);
            if (c & LEFT) { queue(waitlists[otherWaitlist], XY{pos.x - 1, pos.y}); }
            if (c & RIGHT) { queue(waitlists[otherWaitlist], XY{pos.x + 1, pos.y}); }
            if (c & UP) { queue(waitlists[otherWaitlist], XY{pos.x, pos.y - 1}); }
            if (c & DOWN) { queue(waitlists[otherWaitlist], XY{pos.x, pos.y + 1}); }
        }
        otherWaitlist = currentWaitlist;
        currentWaitlist = ++currentWaitlist % 2;
        length++;
    }
    std::cerr << "Could not find path\n";
    return std::string::npos;
done:
    return length;
}


int main(int argc, const char** argv) {
    const char* filename;
    if (argc != 2) { filename = "hill.txt"; }
    else { filename = argv[1]; }

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Could not open file\n";
        return 1;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    file.close();
    String2D hill2D(ss.str());
    XY start = hill2D.find('S');
    XY end = hill2D.find('E');
    hill2D.at(start.x, start.y) = 'a';
    hill2D.at(end.x, end.y) = 'z';
    /* printf("Start: (%lu, %lu), End: (%lu, %lu)\n", start.x, start.y, end.x, end.y); */
    auto nodes = createNodes(hill2D);
    ssize length = shortestPath(nodes, start, end);
    std::cout << "Shortest path has length: " << length << "\n";

    // Task 2
    ssize minLength = std::string::npos;
    ssize startAt = 0;
    ssize aIndex = hill2D.getString().find('a', startAt);
    while (aIndex != std::string::npos) {
        XY pos = hill2D.indexToPos(aIndex);
        ssize length_ = shortestPath(nodes, hill2D.indexToPos(aIndex), end);
        /* printf("Path from (%lu, %lu) to end has length: %lu\n", pos.x, pos.y, length_); */
        if (length_ < minLength) { minLength = length_; }
        startAt = aIndex + 1;
        aIndex = hill2D.getString().find('a', startAt);
    }
    printf("Shortest path from any a has length %lu\n", minLength);
}
