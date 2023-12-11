#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const vector<int> NORTH{-1, 0};
const vector<int> EAST{0, 1};
const vector<int> SOUTH{1, 0};
const vector<int> WEST{0, -1};

int startRow = -1;
int startCol = -1;
map<char, vector<vector<int>>> deltas;

bool isValid(int r, int c, int numRows, int numCols) {
    return r >= 0 && r < numRows && c >= 0 && c < numCols;
}

void findPipesInLoop(vector<string> &grid, set<vector<int>> &loopPipes, map<vector<int>, vector<int>> &pred) {
    ifstream strm;
    strm.open("../../inputs/day10.txt");
    string line;
    int row = 0;
    while (getline(strm, line)) {
        grid.push_back(line);
        if (startRow == -1 && line.find('S') != string::npos) {
            startRow = row;
            startCol = line.find('S');
        }
        row++;
    }
    int numRows = grid.size();
    int numCols = grid[0].size();

    // DFS to find pipes in cycle

    // stack of pipes in current path
    vector<vector<int>> stack{{startRow, startCol}};
    set<vector<int>> visited;
    deltas['S'] = {NORTH, EAST, SOUTH, WEST};
    deltas['|'] = {NORTH, SOUTH};
    deltas['-'] = {EAST, WEST};
    deltas['L'] = {NORTH, EAST};
    deltas['J'] = {NORTH, WEST};
    deltas['7'] = {SOUTH, WEST};
    deltas['F'] = {SOUTH, EAST};

    bool cycleDetected = false;
    while (!stack.empty() && !cycleDetected) {
        vector<int> coords = stack.back();
        // don't visit an already-visited node
        if (visited.contains(coords)) {
            stack.pop_back();
            continue;
        }
        // mark this node as visited
        visited.insert(coords);
        char pipe = grid[coords[0]][coords[1]];
        // figure out possible successors based on current pipe
        for (auto &d : deltas[pipe]) {
            int r = coords[0] + d[0];
            int c = coords[1] + d[1];
            if (isValid(r, c, numRows, numCols) && grid[r][c] != '.') {
                // new coordinate contains a pipe
                vector<int> newCoords = {r, c};
                char newPipe = grid[r][c];
                // new coordinate is a predecessor, don't add to stack
                if (pred[coords] == newCoords) {
                    continue;
                }
                // new coordinate has been visited, don't add to stack
                if (visited.contains(newCoords) && newPipe != 'S') {
                    continue;
                }
                // looped back to starting point, cycle detected!
                if (newPipe == 'S') {
                    vector<int> startCoords{startRow, startCol};
                    pred[startCoords] = coords;
                    cycleDetected = true;
                    break;
                }
                // make sure the new pipe connects to the previous pipe
                vector<int> newPipeDir;
                if (d == NORTH) {
                    newPipeDir = SOUTH;
                } else if (d == EAST) {
                    newPipeDir = WEST;
                } else if (d == SOUTH) {
                    newPipeDir = NORTH;
                } else {
                    newPipeDir = EAST;
                }
                // new pipe doesn't connect to previous pipe
                if (find(deltas[newPipe].begin(), deltas[newPipe].end(), newPipeDir) == deltas[newPipe].end()) {
                    continue;
                }
                // valid child, push onto stack
                stack.push_back(newCoords);
                pred[newCoords] = coords;
            }
        }
    }
    // populate set of all pipes in loop
    for (auto &coords : stack) {
        loopPipes.insert(coords);
    }
}

void part1() {
    vector<string> grid;
    set<vector<int>> loopPipes;
    map<vector<int>, vector<int>> pred;
    findPipesInLoop(grid, loopPipes, pred);
    cout << loopPipes.size() / 2 << endl;
}

// find the pipe that 'S' should be to complete the loop
char findStartPipe(map<vector<int>, vector<int>> &pred, map<vector<int>, vector<int>> &succ) {
    vector<int> startCoords{startRow, startCol};
    vector<int> deltaPred{pred[startCoords][0] - startRow, pred[startCoords][1] - startCol};
    vector<int> deltaSucc{succ[startCoords][0] - startRow, succ[startCoords][1] - startCol};
    for (auto &entry : deltas) {
        if ((entry.second[0] == deltaPred || entry.second[1] == deltaPred) && (entry.second[0] == deltaSucc || entry.second[1] == deltaSucc)) {
            return entry.first;
        }
    }
    // should be unreachable
    return '?';
}

int floodNumRows = -1;
int floodNumCols = -1;

void flood(int r, int c, vector<string> &floodGrid) {
    vector<int> coords{r, c};
    if (floodGrid[r][c] == '*' || floodGrid[r][c] == '+') {
        vector<vector<int>> stack;
        stack.push_back(coords);
        while(!stack.empty()) {
            vector<int> curr = stack.back(); 
            stack.pop_back();
            if (isValid(curr[0], curr[1], floodNumRows, floodNumCols) && (floodGrid[curr[0]][curr[1]] == '*' || floodGrid[curr[0]][curr[1]] == '+')) {
                floodGrid[curr[0]][curr[1]] = '.';
                for (auto &d : deltas['S']) {
                    vector<int> cand{curr[0] + d[0], curr[1] + d[1]}; 
                    stack.push_back(cand);
                }
            }
        }
    }
}

void part2() {
    vector<string> grid;
    set<vector<int>> loopPipes;
    map<vector<int>, vector<int>> pred;
    findPipesInLoop(grid, loopPipes, pred);

    // mark all coordinates that are not part of the loop as '*' 
    int numRows = grid.size();
    int numCols = grid[0].size();
    for (int r = 0; r < numRows; r++) {
        for (int c = 0; c < numCols; c++) {
            vector<int> coords{r, c};
            if (!loopPipes.contains(coords)) {
                grid[r][c] = '*';
            }
        }
    }

    // construct a new grid ~4 times size of original grid
    // to add channels indicating gaps between pipes
    vector<string> floodGrid;
    for (int r = 0; r < numRows * 2 - 1; r++) {
        stringstream ss;
        for (int c = 0; c < numCols * 2 - 1; c++) {
            if (r % 2 == 0 && c % 2 == 0) {
                ss << grid[r / 2][c / 2];
            } else {
                ss << '+';
            }
        }
        floodGrid.push_back(ss.str());
    }

    vector<int> startCoords{startRow, startCol};
    // build mapping that's the reverse of the pred map
    map<vector<int>, vector<int>> succ;
    for (auto &coords : loopPipes) {
        succ[pred[coords]] = coords;
    }

    // figure out what S should be to complete the loop
    char startPipe = findStartPipe(pred, succ);
    floodGrid[startRow * 2][startCol * 2] = startPipe;

    // complete the loop in the floodGrid
    for (auto &coords : loopPipes) {
        vector<int> s = succ[coords];
        vector<int> diff{s[0] - coords[0], s[1] - coords[1]};
        if (diff == NORTH) {
            floodGrid[coords[0] * 2 - 1][coords[1] * 2] = '|';
        } else if (diff == EAST) {
            floodGrid[coords[0] * 2][coords[1] * 2 + 1] = '-';
        } else if (diff == SOUTH) {
            floodGrid[coords[0] * 2 + 1][coords[1] * 2] = '|'; 
        } else {
            floodGrid[coords[0] * 2][coords[1] * 2 - 1] = '-'; 
        }
    }

    // floodfill grid from every eligible place on the border of the grid
    // to mark all areas outside of the loop as '.'
    floodNumRows = floodGrid.size();
    floodNumCols = floodGrid[0].size();
    // top and bottom border
    for (int r = 0; r < floodNumRows; r += floodNumRows - 1) {
        for (int c = 0; c < floodNumCols; c++) {
            flood(r, c, floodGrid);
        }
    }
    // left and right border
    for (int c = 0; c < floodNumCols; c += floodNumCols - 1) {
        for (int r = 0; r < floodNumRows; r++) {
            flood(r, c, floodGrid);
        }
    }

    // count number of '*' in floodGrid to get original area inside loop
    int result = 0;
    for (int r = 0; r < floodNumRows; r++) {
        for (int c = 0; c < floodNumCols; c++) {
            if (floodGrid[r][c] == '*') {
                result++;
            }
        }
    }
    cout << result << endl;
}

int main() { 
    part1(); 
    part2();
}