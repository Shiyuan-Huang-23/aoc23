#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

long getLocation(long seed, const vector<vector<vector<long>>> &maps) {
    long temp = seed;
    for (auto map : maps) {
        for (auto range : map) {
            if (temp >= range[1] && temp - range[1] < range[2]) {
                temp += range[0] - range[1];
                break;
            }
        }
    }
    return temp;
}

void parseInput(vector<long> &seeds, vector<vector<vector<long>>> &maps) {
    ifstream strm;
    strm.open("../../inputs/day5.txt");
    string line;
    while (getline(strm, line)) {
        if (seeds.empty()) {
            stringstream ss(line.substr(line.find(':') + 2));
            long seed;
            while (ss >> seed) {
                seeds.push_back(seed);
            }
        } else if (!line.empty() && !isdigit(line[0])) {
            // parsing new type of map
            vector<vector<long>> map;
            maps.push_back(map);
        } else if (!line.empty()) {
            // add new range to map
            vector<long> range;
            stringstream ss(line);
            long temp;
            while (ss >> temp) {
                range.push_back(temp);
            }
            maps[maps.size() - 1].push_back(range);
        }
    }
}

void part1() {
    vector<long> seeds;
    vector<vector<vector<long>>> maps;
    parseInput(seeds, maps);
    long minLocation = __LONG_MAX__;
    for (auto seed : seeds) {
        minLocation = min(minLocation, getLocation(seed, maps));
    }
    cout << minLocation << endl;
}

// maps points in the output space to corresponding points in the input space, updating points in place
void mapOutputToInput(set<long>& points, const vector<vector<long>> &map) {
    set<long> newPoints;
    for (auto point : points) {
        bool pointMapped = false;
        for (auto range : map) {
            if (point >= range[0] && point < range[0] + range[2]) {
                newPoints.insert(point - range[0] + range[1]);
                pointMapped = true;
                break;
            }
        }
        if (!pointMapped) {
            newPoints.insert(point);
        }
    }

    // update points in place
    points.clear();
    for (auto point : newPoints) {
        points.insert(point);
    }
}

void insertDestinationRangeStarts(set<long> &points, const vector<vector<long>> &map) {
    for (auto range : map) {
        // destination range start is point of interest
        points.insert(range[0]);
        // destination range start + range is point of interest
        points.insert(range[0] + range[2]);
        // these points split the range into segments where the mapping function is linear
    } 
}

bool isValidPoint(long point, const vector<long> &seeds) {
    for (int i = 0; i < seeds.size(); i += 2) {
        if (point >= seeds[i] && point < seeds[i] + seeds[i+1]) {
            return true;
        }
    }
    return false;
}

void part2() {
    vector<long> seeds;
    vector<vector<vector<long>>> maps;
    parseInput(seeds, maps);

    // points of interest to evaluate
    set<long> points;
    insertDestinationRangeStarts(points, maps[maps.size() - 1]);
    for (int i = maps.size() - 1; i >= 0; i--) {
        mapOutputToInput(points, maps[i]);
        if (i >= 1) {
            insertDestinationRangeStarts(points, maps[i - 1]);
        } else {
            // seed range starts are also points of interest
            for (int j = 0; j < seeds.size(); j += 2) {
                points.insert(seeds[j]);
                points.insert(seeds[j] + seeds[j + 1]);
            }
        }
    }

    long minLocation = __LONG_MAX__;
    for (auto point : points) {
        // evaluate all points of interest that are valid seeds
        if (isValidPoint(point, seeds)) {
            minLocation = min(minLocation, getLocation(point, maps));
        }
    }
    cout << minLocation << endl;
}

int main() { 
    part1();
    part2();
}