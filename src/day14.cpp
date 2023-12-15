#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// move lever north
long moveNorth(const vector<string> &grid) {
    long result = 0;
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    // iterate through each col
    // "move" each O upwards until it hits # or the topmost row
    // add its ending row to the sum
    // easier way to do this instead of actually moving them is to count how
    // many rocks will fall contiguously into a certain range e.g. count all
    // rocks in between # and the north edge in a column and calculate their
    // weight, then move past the # and see how many rocks will settle on the #
    for (int c = 0; c < NUM_COLS; c++) {
        int r = 0;
        int base = -1;
        int numRocksSeen = 0;
        while (r < NUM_ROWS) {
            char elem = grid[r][c];
            if (elem != '#' && base == -1) {
                base = r;
            }
            if (elem == 'O') {
                result += NUM_ROWS - base - numRocksSeen;
                numRocksSeen++;
            } else if (elem == '#') {
                base = -1;
                numRocksSeen = 0;
            }
            r++;
        }
    }
    return result;
}

void spinNorth(vector<string> &grid) {
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    for (int c = 0; c < NUM_COLS; c++) {
        int r = 0;
        int base = -1;
        int numRocksSeen = 0;
        while (r < NUM_ROWS) {
            char elem = grid[r][c];
            if (elem != '#' && base == -1) {
                base = r;
            }
            if (elem == 'O') {
                grid[r][c] = '.';
                grid[base + numRocksSeen][c] = 'O';
                numRocksSeen++;
            } else if (elem == '#') {
                base = -1;
                numRocksSeen = 0;
            }
            r++;
        }
    } 
}

void spinWest(vector<string> &grid) {
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    for (int r = 0; r < NUM_ROWS; r++) {
        int c = 0;
        int base = -1;
        int numRocksSeen = 0;
        while (c < NUM_COLS) {
            char elem = grid[r][c];
            if (elem != '#' && base == -1) {
                base = c;
            }
            if (elem == 'O') {
                grid[r][c] = '.';
                grid[r][base + numRocksSeen] = 'O';
                numRocksSeen++;
            } else if (elem == '#') {
                base = -1;
                numRocksSeen = 0;
            }
            c++;
        }
    } 
}

void spinSouth(vector<string> &grid) {
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    for (int c = 0; c < NUM_COLS; c++) {
        int r = NUM_ROWS - 1;
        int base = -1;
        int numRocksSeen = 0;
        while (r >= 0) {
            char elem = grid[r][c];
            if (elem != '#' && base == -1) {
                base = r;
            }
            if (elem == 'O') {
                grid[r][c] = '.';
                grid[base - numRocksSeen][c] = 'O';
                numRocksSeen++;
            } else if (elem == '#') {
                base = -1;
                numRocksSeen = 0;
            }
            r--;
        }
    } 
}

void spinEast(vector<string> &grid) {
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    for (int r = 0; r < NUM_ROWS; r++) {
        int c = NUM_COLS - 1;
        int base = -1;
        int numRocksSeen = 0;
        while (c >= 0) {
            char elem = grid[r][c];
            if (elem != '#' && base == -1) {
                base = c;
            }
            if (elem == 'O') {
                grid[r][c] = '.';
                grid[r][base - numRocksSeen] = 'O';
                numRocksSeen++;
            } else if (elem == '#') {
                base = -1;
                numRocksSeen = 0;
            }
            c--;
        }
    }  
}

long calculateNorthLoad(const vector<string> &grid) {
    long result = 0;
    int NUM_ROWS = grid.size();
    int NUM_COLS = grid[0].size();
    for (int c = 0; c < NUM_COLS; c++) {
        for (int r = 0; r < NUM_ROWS; r++) {
            if (grid[r][c] == 'O') {
                result += NUM_ROWS - r;
            }
        }
    } 
    return result;
}

// calculate load after 1000000000 notes
// idea: figure out a point when the grid cycles back to a past configuration, then use this to extrapolate 
long spin(vector<string> &grid) {
    vector<vector<string>> past;
    vector<string> origGridCopy(grid); 
    past.push_back(origGridCopy);
    long NUM_CYCLES = 1000000000; 
    long cycleStart;
    for (long i = 1; i <= NUM_CYCLES; i++) {
        spinNorth(grid);
        spinWest(grid);
        spinSouth(grid);
        spinEast(grid);
        cycleStart = -1;
        for (long j = 0; j < past.size(); j++) {
            if (past[j] == grid) {
                cycleStart = j;
            }
        }
        if (cycleStart != -1) {
            break;
        }
        vector<string> gridCopy(grid);
        past.push_back(gridCopy);
    }
    long numUniqueConfigs = past.size() - cycleStart;
    long cycleNum = (NUM_CYCLES - cycleStart) % numUniqueConfigs;
    vector<string> finalGrid = past[cycleStart + cycleNum];
    return calculateNorthLoad(finalGrid);
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day14.txt");
    string line;
    vector<string> grid;
    while (getline(strm, line)) {
        grid.push_back(line);
    }
    long result = moveNorth(grid);
    cout << result << endl;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day14.txt");
    string line;
    vector<string> grid;
    while (getline(strm, line)) {
        grid.push_back(line);
    }
    long result = spin(grid);
    cout << result << endl;
}

int main() { 
    part1(); 
    part2();
}