#include <cstdio>
#include <fstream>
#include <sstream>
#include <set>
#include <string>
#include <algorithm>

class Forest {
    public:
        Forest(std::string&& forestS) : forest(std::move(forestS)) {
            lineLength = forest.find('\n') + 1;
            lineCount = std::count(forest.begin(), forest.end(), '\n') + 1;
        }
        char at(unsigned x, unsigned y) const { return forest.at(x + lineLength * y); }
        void set(unsigned x, unsigned y, char c) { forest[x + lineLength * y] = c; }
        unsigned getXTreeCount() const { return lineLength - 1; }
        unsigned getYTreeCount() const { return lineCount - 1; }
        const std::string& getForest() const { return forest; }
    private:
        std::string forest;
        unsigned lineLength;
        unsigned lineCount;
};

std::string readFileToString(const char* filename) {
    std::ifstream file(filename);
    if (file.is_open()){
        std::ostringstream ss;
        file >> ss.rdbuf();
        return ss.str();
    } 
    return "";
}

struct XY {
    unsigned x;
    unsigned y;
    operator unsigned long() {
        return *reinterpret_cast<unsigned long*>(this);
    }
};

template<typename F>
void insertVisibleTrees(const Forest& forest, std::set<unsigned long>& visibleTrees, unsigned startX, unsigned startY, F&& increment) {
    char previousMaxHeight = '0' - 1;
    char currentHeight = '0';
    while (startX < forest.getXTreeCount() and startY < forest.getYTreeCount()) {
        currentHeight = forest.at(startX, startY);
        /* std::cout << "insertVisibleTrees() at x=" << startX << ", y=" << startY << ", char=" << currentHeight << ", visibleTreeCount=" << visibleTrees.size() << std::endl; */
        if (forest.at(startX, startY) > previousMaxHeight) {
            visibleTrees.insert(XY{startX, startY});
            previousMaxHeight = currentHeight;
        }
        increment(startX, startY);
    }
}

template<typename F>
inline unsigned getDirectionalScenicScore(const Forest& forest, unsigned x, unsigned y, char maxHeight, F&& increment) {
    unsigned scenicScore = 1;
    increment(x, y);
    while (x != INT32_MAX and y != INT32_MAX and x < forest.getXTreeCount() and y < forest.getYTreeCount()) {
        if (forest.at(x, y) >= maxHeight) { return scenicScore; }
        scenicScore++;
        increment(x, y);
    }
    return scenicScore-1;
}

unsigned getScenicScore(const Forest& forest, unsigned treeX, unsigned treeY) {
    unsigned scenicScore = 1;
    char maxHeight = forest.at(treeX, treeY);
    scenicScore *= getDirectionalScenicScore(forest, treeX, treeY, maxHeight, [](unsigned& x, unsigned& y){ x++; });
    if (scenicScore == 0) { return 0; }
    scenicScore *= getDirectionalScenicScore(forest, treeX, treeY, maxHeight, [](unsigned& x, unsigned& y){ x--; });
    if (scenicScore == 0) { return 0; }
    scenicScore *= getDirectionalScenicScore(forest, treeX, treeY, maxHeight, [](unsigned& x, unsigned& y){ y++; });
    if (scenicScore == 0) { return 0; }
    scenicScore *= getDirectionalScenicScore(forest, treeX, treeY, maxHeight, [](unsigned& x, unsigned& y){ y--; });
    return scenicScore;
}

int main(int argc, char** argv) {
    // only the ones inside
    std::set<unsigned long> visibleTrees;
    if (argc != 2) {
        printf("Need exactly one argument, the filename\n");
        return 1;
    }
    Forest forest(readFileToString(argv[1]));
    unsigned totalTreeCount = forest.getXTreeCount() * forest.getYTreeCount();
    printf("Analyzing forest with [%d, %d] trees in [x, y] direction and a total tree count of %d.\n", forest.getXTreeCount(), forest.getYTreeCount(), totalTreeCount);

    for (unsigned y = 0; y < forest.getYTreeCount(); y++) {
        // border trees
        visibleTrees.insert(XY{0, y});
        visibleTrees.insert(XY{forest.getXTreeCount() - 1, y});
        // from left to right;
        insertVisibleTrees(forest, visibleTrees, 0, y, [](unsigned& x_, unsigned&y_){ x_++; });
        // from right to left
        insertVisibleTrees(forest, visibleTrees, forest.getXTreeCount() - 1, y, [](unsigned& x_, unsigned&y_){ x_--; });
    }
    for (unsigned x = 0; x < forest.getXTreeCount(); x++) {
        // border trees
        visibleTrees.insert(XY{x, 0});
        visibleTrees.insert(XY{x, forest.getYTreeCount() - 1});
        // from top to bottom
        insertVisibleTrees(forest, visibleTrees, x, 0, [](unsigned& x_, unsigned&y_){ y_++; });
        // from bottom to top
        insertVisibleTrees(forest, visibleTrees, x, forest.getYTreeCount() - 1, [](unsigned& x_, unsigned&y_){ y_--; });
    }
    std::string visualizedForestStr(totalTreeCount + forest.getYTreeCount() - 1, '_');  // +y for the newlines
    for (unsigned i = 0; i < forest.getYTreeCount() - 1; i++) {  // set newlines
        visualizedForestStr[i * (forest.getXTreeCount() + 1) + forest.getXTreeCount()] = '\n';
    }
    unsigned visibleTreeCount = visibleTrees.size();

    Forest visualizedForest(std::move(visualizedForestStr));
    for (auto it = visibleTrees.begin(); it != visibleTrees.end(); it++) {
        unsigned long copy = *it;
        XY tree = *reinterpret_cast<XY*>(&copy);
        visualizedForest.set(tree.x, tree.y, 'X');
    }
    printf("%s\n", visualizedForest.getForest().c_str());

    XY bestTree{};
    unsigned maxScenicScore = 0;
    // task 2
    for (unsigned x = 1; x < forest.getXTreeCount() - 1; x++) {
        for (unsigned y = 1; y < forest.getYTreeCount() - 1; y++) {
            unsigned scenicScore = getScenicScore(forest, x, y);
            if (scenicScore > maxScenicScore) {
                bestTree.x = x;
                bestTree.y = y;
                maxScenicScore = scenicScore;
            }
        }
    }

    printf("In this forest, there are %u visible trees\n", visibleTreeCount);
    printf("The tree with the best scenic score is at [%d, %d] and has a score of %d\n", bestTree.x, bestTree.y, maxScenicScore);
}
