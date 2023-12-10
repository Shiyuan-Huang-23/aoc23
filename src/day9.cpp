#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int extrapolateForwards(const vector<int> &sequence) {
    vector<int> diffs;
    bool allZeros = true;
    for (int i = 0; i < sequence.size() - 1; i++) {
        int diff = sequence[i + 1] - sequence[i]; 
        diffs.push_back(diff);
        if (diff != 0) {
            allZeros = false;
        }
    }
    int last = sequence[sequence.size() - 1];
    if (allZeros) {
        return last;
    } else {
        int lastDiff = extrapolateForwards(diffs);
        return last + lastDiff;
    }
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day9.txt");
    string line;
    int result = 0;
    while (getline(strm, line)) {
        stringstream ss(line);
        int n;
        vector<int> sequence;
        while (ss >> n) {
            sequence.push_back(n);
        }
        result += extrapolateForwards(sequence);
    }
    cout << result << endl;
}

int extrapolateBackwards(const vector<int> &sequence) {
    vector<int> diffs;
    bool allZeros = true;
    for (int i = 0; i < sequence.size() - 1; i++) {
        int diff = sequence[i + 1] - sequence[i]; 
        diffs.push_back(diff);
        if (diff != 0) {
            allZeros = false;
        }
    }
    int first = sequence[0];
    if (allZeros) {
        return first;
    } else {
        int firstDiff = extrapolateBackwards(diffs);
        return first - firstDiff;
    } 
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day9.txt");
    string line;
    int result = 0;
    while (getline(strm, line)) {
        stringstream ss(line);
        int n;
        vector<int> sequence;
        while (ss >> n) {
            sequence.push_back(n);
        }
        result += extrapolateBackwards(sequence);
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}