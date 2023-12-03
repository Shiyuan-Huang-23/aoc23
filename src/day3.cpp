#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool isValid(int r, int c, int num_rows, int num_cols) {
    return r >= 0 && r < num_rows && c >= 0 && c < num_cols;
}

bool isSymbol(char c) { return c != '.' && !isdigit(c); }

bool hasSymbolNeighbor(int r, int c, const vector<string> &schematic) {
    int num_rows = schematic.size();
    int num_cols = schematic[0].size();
    for (int delta_r = -1; delta_r <= 1; delta_r++) {
        for (int delta_c = -1; delta_c <= 1; delta_c++) {
            if (isValid(r + delta_r, c + delta_c, num_rows, num_cols) &&
                isSymbol(schematic[r + delta_r][c + delta_c])) {
                return true;
            }
        }
    }
    return false;
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day3.txt");
    string line;
    int result = 0;
    vector<string> schematic;
    while (getline(strm, line)) {
        schematic.push_back(line);
    }
    for (int r = 0; r < schematic.size(); r++) {
        line = schematic[r];
        int temp = 0;
        bool isPartNum = false;
        for (int c = 0; c < line.size(); c++) {
            if (isdigit(line[c])) {
                temp = temp * 10 + (line[c] - '0');
                isPartNum = hasSymbolNeighbor(r, c, schematic) || isPartNum;
                // potentially add temp to result at end of row
                if (c + 1 == line.size() && isPartNum) {
                    result += temp;
                }
            } else if (temp != 0) {
                // figure out if temp should be added to result
                if (isPartNum) {
                    result += temp;
                }
                temp = 0;
                isPartNum = false;
            }
        }
    }
    cout << result << endl;
}

void updateAdjGears(int r, int c, const vector<string> &schematic,
                    set<vector<int>> &adjGears) {
    int num_rows = schematic.size();
    int num_cols = schematic[0].size();
    for (int delta_r = -1; delta_r <= 1; delta_r++) {
        for (int delta_c = -1; delta_c <= 1; delta_c++) {
            if (isValid(r + delta_r, c + delta_c, num_rows, num_cols) &&
                (schematic[r + delta_r][c + delta_c] == '*')) {
                vector<int> coords{r + delta_r, c + delta_c};
                adjGears.insert(coords);
            }
        }
    }
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day3.txt");
    string line;
    vector<string> schematic;
    while (getline(strm, line)) {
        schematic.push_back(line);
    }
    map<vector<int>, vector<int>> gears;
    for (int r = 0; r < schematic.size(); r++) {
        line = schematic[r];
        int temp = 0;
        set<vector<int>> adjGears;
        for (int c = 0; c < schematic.size(); c++) {
            if (isdigit(line[c])) {
                temp = temp * 10 + (line[c] - '0');
                updateAdjGears(r, c, schematic, adjGears);
                if (c + 1 == line.size()) {
                    for (auto gearCoords : adjGears) {
                        gears[gearCoords].push_back(temp);
                    }
                }
            } else if (temp != 0) {
                for (auto gearCoords : adjGears) {
                    gears[gearCoords].push_back(temp);
                }
                temp = 0;
                adjGears.clear();
            }
        }
    }
    int result = 0;
    for (const auto &[gear, parts] : gears) {
        if (parts.size() == 2) {
            result += parts[0] * parts[1];
        }
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}