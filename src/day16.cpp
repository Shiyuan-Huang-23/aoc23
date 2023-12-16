#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

const vector<vector<int>> DELTAS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

enum class Direction { UP, RIGHT, DOWN, LEFT };

struct beam {
    int r;
    int c;
    Direction d;
};
// need to override < to use beam in a set
inline bool operator<(const beam& lhs, const beam& rhs) {
    return (lhs.r < rhs.r) || (lhs.c < rhs.c) || (int(lhs.d) < int(rhs.d));
}
// need to override == for find to work
inline bool operator==(const beam& lhs, const beam& rhs) { 
    return lhs.r == rhs.r && lhs.c == rhs.c && lhs.d == rhs.d;
}

// fill in new direction(s) of beam given current direction and tile
void getNewDirections(Direction d, char tile, vector<Direction>& newDirs) {
    using enum Direction;
    if (tile == '.') {
        newDirs.push_back(d);
    } else if (tile == '/') {
        Direction newDir;
        if (d == UP) {
            newDir = RIGHT;
        } else if (d == RIGHT) {
            newDir = UP;
        } else if (d == DOWN) {
            newDir = LEFT;
        } else {
            newDir = DOWN;
        }
        newDirs.push_back(newDir);
    } else if (tile == '\\') {
        Direction newDir;
        if (d == UP) {
            newDir = LEFT;
        } else if (d == RIGHT) {
            newDir = DOWN;
        } else if (d == DOWN) {
            newDir = RIGHT;
        } else {
            newDir = UP;
        }
        newDirs.push_back(newDir);
    } else if (tile == '|') {
        if (d == UP || d == DOWN) {
            newDirs.push_back(d);
        } else {
            newDirs.push_back(UP);
            newDirs.push_back(DOWN);
        }
    } else if (tile == '-') {
        if (d == LEFT || d == RIGHT) {
            newDirs.push_back(d);
        } else {
            newDirs.push_back(LEFT);
            newDirs.push_back(RIGHT);
        }
    } else {
        throw "Invalid tile encountered";
    }
}

void disp(beam curr) {
    cout << "(" << curr.r << ',' << curr.c << ',' << int(curr.d) << ")";
}

int getEnergizedTiles(const vector<string> &grid, beam start) {
    const int NUM_ROWS = grid.size();
    const int NUM_COLS = grid[0].size();
    // all beams (tile and dir) that have been simulated
    set<beam> simulated;
    // beams that still need to be simulated
    vector<beam> activeBeams;
    // simulate
    activeBeams.push_back(start);
    while (!activeBeams.empty()) {
        beam curr = activeBeams.back();
        activeBeams.pop_back();
        auto temp = find(simulated.begin(), simulated.end(), curr);
        if (temp != simulated.end()) {
            continue;
        }
        simulated.insert(curr);
        char tile = grid[curr.r][curr.c];
        vector<Direction> newDirs;
        getNewDirections(curr.d, tile, newDirs);
        for (auto d : newDirs) {
            vector<int> delta = DELTAS[int(d)];
            // make sure new tile is valid
            int r = curr.r + delta[0];
            int c = curr.c + delta[1];
            if (r < 0 || r >= NUM_ROWS || c < 0 || c >= NUM_COLS) {
                continue;
            }
            beam newBeam = {r, c, d};
            auto pos = find(activeBeams.begin(), activeBeams.end(), newBeam);
            if (pos == activeBeams.end()) {
                activeBeams.push_back({r, c, d});
            }
        }
    }
    // simulation complete, calculate energized tiles
    // all unique energized tiles
    set<vector<int>> energizedTiles;
    for (auto b : simulated) {
        vector<int> coords = {b.r, b.c};
        energizedTiles.insert(coords);
    }
    return energizedTiles.size();
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day16.txt");
    vector<string> grid;
    string line;
    while (getline(strm, line)) {
        grid.push_back(line);
    }
    cout << getEnergizedTiles(grid, {0, 0, Direction::RIGHT}) << endl;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day16.txt");
    vector<string> grid;
    string line;
    while (getline(strm, line)) {
        grid.push_back(line);
    }
    const int NUM_ROWS = grid.size();
    const int NUM_COLS = grid[0].size();
    int maxTiles = 0;
    // top and bottom
    for (int c = 0; c < NUM_COLS; c++) {
        int temp = getEnergizedTiles(grid, {0, c, Direction::DOWN});
        maxTiles = max(maxTiles, temp);
        temp = getEnergizedTiles(grid, {NUM_ROWS - 1, c, Direction::UP});
        maxTiles = max(maxTiles, temp);
    }
    // left and right
    for (int r = 0; r < NUM_ROWS; r++) {
        int temp = getEnergizedTiles(grid, {r, 0, Direction::RIGHT});
        maxTiles = max(maxTiles, temp);
        temp = getEnergizedTiles(grid, {r, NUM_COLS - 1, Direction::LEFT});
        maxTiles = max(maxTiles, temp);
    }
    cout << maxTiles << endl;
}

int main() {
    part1();
    part2();
}