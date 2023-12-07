#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void addInputsToVector(stringstream &ss, vector<int> &v) {
    int temp;
    while (ss >> temp) {
        v.push_back(temp);
    }
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day6.txt");
    string line;
    vector<int> times;
    vector<int> distances;
    while (getline(strm, line)) {
        stringstream ss(line.substr(line.find(':') + 1));
        addInputsToVector(ss, times.empty() ? times : distances);
    }
    int result = 1;
    for (int i = 0; i < times.size(); i++) {
        int record = distances[i];
        int waysToWin = 0;
        int time = times[i];
        for (int hold = 0; hold < time; hold++) {
            if (hold * (time - hold) > record) {
                waysToWin++;
            }
        }
        result *= waysToWin;
    }
    cout << result << endl;
}

long getInputFromLine(stringstream &ss) {
    string result;
    string temp;
    while (ss >> temp) {
        result = result + temp;
    }
    return stol(result);
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day6.txt");
    string line;
    long time = 0;
    long record = 0;
    while (getline(strm, line)) {
        stringstream ss(line.substr(line.find(':') + 1));
        if (time == 0) {
            time = getInputFromLine(ss);
        } else {
            record = getInputFromLine(ss);
        }
    }
    long discriminant = time * time - 4 * record;
    double root1 = (-time + sqrt(discriminant)) / (-2);
    double root2 = (-time - sqrt(discriminant)) / (-2);
    cout << fixed << (long)(floor(root2) - ceil(root1) + 1) << endl;
}

int main() {
    part1();
    part2();
}