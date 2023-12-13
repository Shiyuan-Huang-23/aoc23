#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// checks if the record of strings, s, matches the contiguous group counts
// Requires: s only has '#' and '.'
bool matches(const string &s, const vector<int> &counts) {
    // length of current contiguous group of damaged strings
    int len = 0;
    // how many groups have been completed so far
    int groupNum = 0;
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '.') {
            if (len != 0) {
                if (groupNum >= counts.size()) {
                    return false;
                }
                // end current group
                if (len != counts[groupNum]) {
                    return false;
                }
                len = 0;
                groupNum++;
            }
        } else {
            len++;
        }
    }
    // make sure to count last group if needed
    if (len != 0) {
        if (groupNum >= counts.size()) {
            return false;
        }
        if (len == counts[groupNum]) {
            groupNum++;
        } else {
            return false;
        }
    }
    // make sure all groups have been accounted for
    return groupNum == counts.size();
}

struct prefixinfo {
    // number of groups that have been completed in the prefix
    int groupsCompleted;
    // index of the end of the last completed group, that is, the '.' after
    // the last completed group
    int endOfLastGroup;
};

// make sure s[0, size) matches a prefix of counts, and returns number of
// groups completed in counts
prefixinfo getPrefixInfo(const string &s, int size, const vector<int> &counts) {
    // length of current contiguous group of damaged strings
    int len = 0;
    // how many groups have been completed so far
    int groupsCompleted = 0;
    int endOfLastGroup = -1;
    for (int i = 0; i < size; i++) {
        char c = s[i];
        if (c == '.') {
            if (len != 0) {
                if (groupsCompleted >= counts.size()) {
                    return {-1, -1};
                }
                // end current group
                if (len != counts[groupsCompleted]) {
                    return {-1, -1};
                }
                len = 0;
                groupsCompleted++;
                endOfLastGroup = i;
            }
        } else {
            len++;
        }
    }
    // don't count group as completed unless we've see a '.' after it
    return {groupsCompleted, endOfLastGroup};
}

long calcArrangements(const string &s, const vector<int> counts) {
    auto index = s.find('?');
    if (index != string::npos) {
        // explore both possible options for '?'
        string sBroken(s);
        sBroken[index] = '#';
        long result = 0;
        prefixinfo p = getPrefixInfo(sBroken, index + 1, counts);
        if (p.groupsCompleted != -1) {
            vector<int> subCounts = {counts.begin() + p.groupsCompleted,
                                     counts.end()};
            string newS = p.endOfLastGroup == -1
                              ? sBroken
                              : sBroken.substr(p.endOfLastGroup + 1);
            result += calcArrangements(newS, subCounts);
        }
        string sWorking(s);
        sWorking[index] = '.';
        p = getPrefixInfo(sWorking, index + 1, counts);
        if (p.groupsCompleted != -1) {
            vector<int> subCounts = {counts.begin() + p.groupsCompleted,
                                     counts.end()};
            string newS = p.endOfLastGroup == -1
                              ? sWorking
                              : sWorking.substr(p.endOfLastGroup + 1);
            result += calcArrangements(newS, subCounts);
        }
        return result;
    } else {
        // no missing records, check if the record matches the group counts
        return matches(s, counts);
    }
}

void part1() {
    ifstream strm;
    strm.open("../../inputs/day12.txt");
    string line;
    int result = 0;
    while (getline(strm, line)) {
        stringstream ss(line);
        string s;
        ss >> s;
        vector<int> counts;
        int d;
        while (ss >> d) {
            counts.push_back(d);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        result += calcArrangements(s, counts);
    }
    cout << result << endl;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day12.txt");
    string line;
    long result = 0;
    int num = 0;
    while (getline(strm, line)) {
        stringstream ss(line);
        string s;
        ss >> s;
        vector<int> counts;
        int d;
        while (ss >> d) {
            counts.push_back(d);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
        int countsSize = counts.size();
        string sUnfolded(s);
        for (int i = 0; i < 4; i++) {
            sUnfolded += '?';
            sUnfolded += s;
            for (int j = 0; j < countsSize; j++) {
                counts.push_back(counts[j]);
            }
        }
        long temp = calcArrangements(sUnfolded, counts);
        result += temp;
        num++;
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}