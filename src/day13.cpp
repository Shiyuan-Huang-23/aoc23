#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// number of rows above horizontal line of reflection, 0 if none exists
int horizRefl(const vector<string> &grid) {
    // line of reflection being evaluated is between rows i and i + 1
    int NUM_ROWS = grid.size();
    for (int refl = 0; refl < NUM_ROWS - 1; refl++) {
        bool isLineOfRefl = true;
        for (int r = 0; r < min(refl + 1, NUM_ROWS - refl - 1); r++) {
            if (grid[refl - r] != grid[refl + r + 1]) {
                isLineOfRefl = false;
                break;
            }
        }
        if (isLineOfRefl) {
            return refl + 1;
        }
    }
    return 0;
}

// number of rows above horizontal line of reflection if one change is made, 0
// if none exists
int horizReflSmudge(const vector<string> &grid) {
    // line of reflection being evaluated is between rows i and i + 1
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    for (int refl = 0; refl < NUM_ROWS - 1; refl++) {
        bool isLineOfRefl = false;
        bool diffFound = false;
        for (int r = 0; r < min(refl + 1, NUM_ROWS - refl - 1); r++) {
            // determine if current line could be line of reflection with 1
            // change
            string row1 = grid[refl - r];
            string row2 = grid[refl + r + 1];
            for (int c = 0; c < NUM_COLS; c++) {
                if (row1[c] != row2[c]) {
                    if (!diffFound) {
                        diffFound = true;
                        isLineOfRefl = true;
                    } else {
                        isLineOfRefl = false;
                        break;
                    }
                }
            }
        }
        if (isLineOfRefl) {
            return refl + 1;
        }
    }
    return 0;
}

int processGrid(const vector<string> &grid, const vector<string> &transpose) {
    int r = horizRefl(grid);
    if (r != 0) {
        return 100 * r;
    }
    return horizRefl(transpose);
}

int processGridSmudge(const vector<string> &grid,
                      const vector<string> &transpose) {
    int r = horizReflSmudge(grid);
    if (r != 0) {
        return 100 * r;
    }
    return horizReflSmudge(transpose);
}

void solve(bool smudge) {
    ifstream strm;
    strm.open("../../inputs/day13.txt");
    vector<string> grid;
    vector<string> transpose;
    string line;
    int result = 0;
    while (getline(strm, line)) {
        if (line.empty()) {
            // process existing grid
            result += smudge ? processGridSmudge(grid, transpose)
                             : processGrid(grid, transpose);
            // start new grid
            grid.clear();
            transpose.clear();
        } else {
            grid.push_back(line);
            if (transpose.empty()) {
                for (auto &c : line) {
                    transpose.push_back(string(1, c));
                }
            } else {
                for (int i = 0; i < line.size(); i++) {
                    transpose[i] += line[i];
                }
            }
        }
    }
    // process last grid
    result += smudge ? processGridSmudge(grid, transpose)
                     : processGrid(grid, transpose);
    cout << result << endl;
}

void part1() { solve(false); }

void part2() { solve(true); }

int main() {
    part1();
    part2();
}