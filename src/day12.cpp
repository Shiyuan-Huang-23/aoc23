#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

// reduces the problem of finding spring arrangements to an equivalent problem
// with fewer springs and counts by modifying springs and counts in place
// returns true if the reduction was done successfully, false if springs do
// not match counts
bool reduce(string &springs, vector<int> &counts) {
    if (!counts.empty()) {
        int minLength =
            accumulate(counts.begin(), counts.end(), 0) + counts.size() - 1;
        if (springs.size() < minLength) {
            return false;
        }
    }
    // length of current contiguous group of damaged springs
    int len = 0;
    // how many groups have been completed so far
    int groupsCompleted = 0;
    int endOfLastGroup = -1;
    bool containsUnknown = false;
    for (int i = 0; i < springs.size(); i++) {
        char c = springs[i];
        if (c == '.') {
            // complete current group of damaged springs
            if (len != 0) {
                if (groupsCompleted >= counts.size() ||
                    len != counts[groupsCompleted]) {
                    return false;
                }
                len = 0;
                groupsCompleted++;
                endOfLastGroup = i;
            }
        } else if (c == '#') {
            len++;
        } else if (c == '?') {
            containsUnknown = true;
            break;
        }
    }
    // if we end on a group of damaged springs, take that into account
    if (len != 0 && !containsUnknown) {
        if (groupsCompleted >= counts.size() ||
            len != counts[groupsCompleted]) {
            return false;
        }
        groupsCompleted++;
        endOfLastGroup = springs.size() - 1;
    }
    if (!containsUnknown && groupsCompleted != counts.size()) {
        return false;
    }
    springs.erase(0, endOfLastGroup + 1);
    while (!springs.empty() && springs[0] == '.') {
        springs.erase(0, 1);
    }
    counts.erase(counts.begin(), counts.begin() + groupsCompleted);
    return true;
}

// Fills in ? characters in springs in place as needed to fulfill at least
// first count in counts
// Returns: true if filling is successful, false ow
// Requires: springs are in reduced form wrt counts, springs contains a ?,
// counts is non-empty
bool fillInFirstCount(string &springs, vector<int> &counts) {
    // length of current contiguous group of damaged strings
    int len = 0;
    // whether the first group matching the first count has been completed
    bool groupCompleted = false;
    for (int i = 0; i < springs.size(); i++) {
        char c = springs[i];
        if (c == '.') {
            if (len != 0) {
                if (len == counts[0]) {
                    return true;
                } else {
                    return false;
                }
            }
        } else if (c == '#') {
            len++;
            if (len > counts[0]) {
                return false;
            }
        } else if (c == '?') {
            if (len < counts[0]) {
                springs[i] = '#';
                len++;
            } else if (len == counts[0]) {
                springs[i] = '.';
                return true;
            }
        }
    }
    if (len == counts[0]) {
        return true;
    }
    return false;
}

map<pair<string,vector<int>>,long> cache;

// how many different arrangements of operational and broken springs fit the
// counts
long recurse(const string &springs, const vector<int> &counts) {
    auto p = make_pair(springs, counts);
    if (cache.contains(p)) {
        return cache[p];
    }
    auto index = springs.find('?');
    if (index != string::npos) {
        if (counts.empty()) {
            string springsCopy(springs);
            vector<int> countsCopy(counts);
            for (int i = index; i < springs.size(); i++) {
                if (springsCopy[i] == '?') {
                    springsCopy[i] = '.';
                }
            }
            long returnVal = reduce(springsCopy, countsCopy);
            cache[p] = returnVal;
            return returnVal;
        }
        long result = 0;
        bool unbrokenSpringUsed = false;
        if (!counts.empty()) {
            string springsCopy(springs);
            vector<int> countsCopy(counts);
            bool valid = fillInFirstCount(springsCopy, countsCopy);
            if (valid) {
                unbrokenSpringUsed = springsCopy[index] == '.';
                if (reduce(springsCopy, countsCopy)) {
                    result += recurse(springsCopy, countsCopy);
                }
            }
        }
        if (!unbrokenSpringUsed) {
            string springsCopy(springs);
            vector<int> countsCopy(counts);
            springsCopy[index] = '.';
            bool valid = reduce(springsCopy, countsCopy);
            if (valid) {
                result += recurse(springsCopy, countsCopy);
            }
        }
        cache[p] = result;
        return result;
    } else {
        string springsCopy(springs);
        vector<int> countsCopy(counts);
        bool doesMatch = reduce(springsCopy, countsCopy);
        cache[p] = doesMatch;
        return doesMatch;
    }
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day12.txt");
    string line;
    long result = 0;
    while (getline(strm, line)) {
        stringstream ss(line);
        string springs;
        ss >> springs;
        vector<int> counts;
        int d;
        while (ss >> d) {
            counts.push_back(d);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        // reduce problem to a sub-problem in place
        reduce(springs, counts);
        result += recurse(springs, counts);
    }
    cout << result << endl;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day12.txt");
    string line;
    long result = 0;
    while (getline(strm, line)) {
        stringstream ss(line);
        string springs;
        ss >> springs;
        vector<int> counts;
        int d;
        while (ss >> d) {
            counts.push_back(d);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        int countsSize = counts.size();
        string springsUnfolded(springs);
        for (int i = 0; i < 4; i++) {
            springsUnfolded += '?';
            springsUnfolded += springs;
            for (int j = 0; j < countsSize; j++) {
                counts.push_back(counts[j]);
            }
        }
        reduce(springsUnfolded, counts);
        result += recurse(springsUnfolded, counts);
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}