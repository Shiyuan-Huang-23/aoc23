#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream strm;
    strm.open("../../inputs/day8.txt");
    string insns;
    getline(strm, insns);
    string line;
    map<string, vector<string>> nodes;
    getline(strm, line);
    while (getline(strm, line)) {
        string node = line.substr(0, 3);
        string left = line.substr(7, 3);
        string right = line.substr(12, 3);
        nodes[node].push_back(left);
        nodes[node].push_back(right);
    }
    string curr = "AAA";
    int steps = 0;
    int count = 0;
    int numInsns = insns.size();
    while (curr != "ZZZ") {
        curr = nodes[curr][insns[count] == 'L' ? 0 : 1];
        steps++;
        count++;
        if (count == numInsns) {
            count = 0;
        }
    }
    cout << steps << endl;
}

long numStepsToZNode(string start, const string &insns,
                     map<string, vector<string>> &nodes) {
    string curr = start;
    long steps = 0;
    int count = 0;
    int numInsns = insns.size();
    while (curr[2] != 'Z') {
        curr = nodes[curr][insns[count] == 'L' ? 0 : 1];
        steps++;
        count++;
        if (count == numInsns) {
            count = 0;
        }
    }
    return steps;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day8.txt");
    string insns;
    getline(strm, insns);
    string line;
    vector<string> startNodes;
    map<string, vector<string>> nodes;
    getline(strm, line);
    while (getline(strm, line)) {
        string node = line.substr(0, 3);
        string left = line.substr(7, 3);
        string right = line.substr(12, 3);
        nodes[node].push_back(left);
        nodes[node].push_back(right);
        if (node[2] == 'A') {
            startNodes.push_back(node);
        }
    }
    vector<long> cycleLengths;
    for (const auto &node : startNodes) {
        cycleLengths.push_back(numStepsToZNode(node, insns, nodes));
    }
    long result = 1;
    for (const auto &cycleLength : cycleLengths) {
        result = lcm(result, cycleLength);
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}