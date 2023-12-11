#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool hasGalaxies(const string &s) {
    for (auto &c : s) {
        if (c == '#') {
            return true;
        }
    }
    return false;
}

long calculateDistancesSum(long expansion) {
    ifstream strm;
    strm.open("../../inputs/day11.txt");
    string line;
    // map of galaxies
    vector<string> grid;
    // the distance each row represents
    vector<long> rowDists;

    while(getline(strm, line)) {
        grid.push_back(line);
        rowDists.push_back(hasGalaxies(line) ? 1 : expansion);
    }

    // the distance each col represents
    vector<long> colDists;
    // coordinates of galaxies
    vector<vector<int>> galaxies;

    int numRows = grid.size();
    int numCols = grid[0].size();
    for (int c = 0; c < numCols; c++) {
        bool colHasGalaxies = false;
        for (int r = 0; r < numRows; r++) {
            if (grid[r][c] == '#') {
                colHasGalaxies = true;
                vector<int> galaxy{r, c};
                galaxies.push_back(galaxy);
            }
        }
        colDists.push_back(colHasGalaxies ? 1 : expansion);
    }

    // find sum of shortest distances between each pair of galaxies
    long result = 0;
    for (int i = 0; i < galaxies.size() - 1; i++) {
        const vector<int> g1 = galaxies[i];
        for (int j = i + 1; j < galaxies.size(); j++) {
            const vector<int> g2 = galaxies[j];
            long rowDist = 0;
            for (int k = min(g1[0], g2[0]); k < max(g1[0], g2[0]); k++) {
                rowDist += rowDists[k];
            }
            long colDist = 0;
            for (int k = min(g1[1], g2[1]); k < max(g1[1], g2[1]); k++) {
                colDist += colDists[k];
            }
            result += rowDist + colDist;
        }
    }
    return result; 
}

void part1() {
    long result = calculateDistancesSum(2);
    cout << result << endl;
}

void part2() {
    long result = calculateDistancesSum(1000000);
    cout << result << endl;
}

int main() {
    part1();
    part2();
}