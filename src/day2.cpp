#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

bool isPossibleSubset(const string &subset) {
    stringstream ss(subset);
    int temp;
    string color;
    while (ss >> temp) {
        ss >> color;
        if ((color[0] == 'r' && temp > 12) || (color[0] == 'g' && temp > 13) ||
            (color[0] == 'b' && temp > 14)) {
            return false;
        }
    }
    return true;
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day2.txt");
    string line;
    int result = 0;
    int gameNum = 1;
    while (getline(strm, line)) {
        bool isGamePossible = true;
        auto pos = line.find(":");
        line = line.substr(pos + 2);
        string subset;
        pos = line.find(";");
        while (pos != string::npos) {
            subset = line.substr(0, pos);
            line = line.substr(pos + 2);
            if (!isPossibleSubset(subset)) {
                isGamePossible = false;
                break;
            }
            pos = line.find(";");
        }
        if (isGamePossible && isPossibleSubset(line)) {
            result += gameNum;
        }
        gameNum++;
    }
    cout << result << endl;
}

struct Colors {
    int red;
    int green;
    int blue;
};

Colors findFewestCubes(const string &subset) {
    stringstream ss(subset);
    int red = 0;
    int green = 0;
    int blue = 0;
    int temp;
    string color;
    while (ss >> temp) {
        ss >> color;
        if (color[0] == 'r') {
            red = max(temp, red);
        } else if (color[0] == 'g') {
            green = max(temp, green);
        } else {
            blue = max(temp, blue);
        }
    }
    return {red, green, blue};
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day2.txt");
    string line;
    int result = 0;
    while (getline(strm, line)) {
        int red = 0;
        int green = 0;
        int blue = 0;

        auto pos = line.find(":");
        line = line.substr(pos + 2);
        string subset;
        pos = line.find(";");
        while (pos != string::npos) {
            subset = line.substr(0, pos);
            line = line.substr(pos + 2);
            auto [r, g, b] = findFewestCubes(subset);
            red = max(r, red);
            green = max(g, green);
            blue = max(b, blue);
            pos = line.find(";");
        }
        auto [r, g, b] = findFewestCubes(line);
        red = max(r, red);
        green = max(g, green);
        blue = max(b, blue);
        result += red * green * blue;
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}