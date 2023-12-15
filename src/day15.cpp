#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using namespace std;

int getHash(const string& s) {
    int curr = 0;
    for (auto &c : s) {
        curr += int(c);
        curr *= 17;
        curr = curr % 256;
    }
    return curr;
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day15.txt");
    string line;
    getline(strm, line);
    int start = 0;
    int result = 0;
    while (start < line.size()) {
        auto end = line.find(',', start);
        string step;
        if (end != string::npos) {
            step = line.substr(start, end - start);
            start = end + 1;
        } else {
            step = line.substr(start, line.size() - start);
            start = line.size();
        }
        result += getHash(step);
    }
    cout << result << endl;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day15.txt");
    string line;
    getline(strm, line);
    int start = 0;
    map<int,vector<pair<string, int>>> boxes;

    while (start < line.size()) {
        auto end = line.find(',', start);
        string step;
        if (end != string::npos) {
            step = line.substr(start, end - start);
            start = end + 1;
        } else {
            step = line.substr(start, line.size() - start);
            start = line.size();
        }
        auto eq = step.find('=');
        if (eq != string::npos) {
            string label = step.substr(0, eq);
            int focalLength = stoi(step.substr(eq + 1));
            auto &box = boxes[getHash(label)];
            bool updated = false;
            for (auto &p : box) {
                if (p.first == label) {
                    p.second = focalLength;
                    updated = true;
                    break;
                }
            }
            if (!updated) {
                box.push_back(make_pair(label, focalLength));
            }
        } else {
            string label = step.substr(0, step.size() - 1);
            auto &box = boxes[getHash(label)];
            int index = -1;
            for (int i = 0; i < box.size(); i++) {
                if (box[i].first == label) {
                    index = i;
                    break;
                }
            }
            if (index != -1) {
                box.erase(box.begin() + index);
            }
        }
    }
    int result = 0;
    for (auto elem : boxes) {
        int boxNum = elem.first;
        auto box = elem.second;
        for (int i = 0; i < box.size(); i++) {
            result += (boxNum + 1) * (i + 1) * box[i].second;
        }
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}