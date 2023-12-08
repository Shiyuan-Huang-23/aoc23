#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum class HandType {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
};

HandType getHandTypeFromCounts(int counts[5]) {
    using enum HandType;
    if (counts[4] == 1) {
        return FIVE_OF_A_KIND;
    } else if (counts[3] == 1) {
        return FOUR_OF_A_KIND;
    } else if (counts[2] == 1 && counts[1] == 1) {
        return FULL_HOUSE;
    } else if (counts[2] == 1 && counts[0] == 2) {
        return THREE_OF_A_KIND;
    } else if (counts[1] == 2 && counts[0] == 1) {
        return TWO_PAIR;
    } else if (counts[1] == 1 && counts[0] == 3) {
        return ONE_PAIR;
    } else {
        return HIGH_CARD;
    }
}

HandType getHandType(const string& hand) {
    map<char, int> counter;
    int counts[5] = {};
    for (auto& c : hand) {
        counter[c]++;
    }
    for (const auto& elem : counter) {
        counts[elem.second - 1]++;
    }
    return getHandTypeFromCounts(counts);
}

string ranking = "23456789TJQKA";

void parseInput(map<string, int>& bids) {
    ifstream strm;
    strm.open("../../inputs/day7.txt");
    string line;
    while (getline(strm, line)) {
        stringstream ss(line);
        string hand;
        int bid;
        ss >> hand;
        ss >> bid;
        bids.insert({hand, bid});
    }
}

void part1() {
    map<string, int> bids;
    parseInput(bids);
    map<HandType, vector<string>> rankedHands;
    // roughly sort hands by their hand type
    for (const auto& elem : bids) {
        string hand = elem.first;
        HandType handType = getHandType(hand);
        rankedHands[handType].push_back(hand);
    }
    // fully sort the hands of each type
    for (auto& elem : rankedHands) {
        auto& hands = elem.second;
        sort(hands.begin(), hands.end(), [](const auto& a, const auto& b) {
            for (int i = 0; i < a.size(); i++) {
                if (a[i] != b[i]) {
                    return ranking.find(a[i]) < ranking.find(b[i]);
                }
            }
            return false;  // should not be reachable
        });
    }
    // calculate ranking and total winnings
    long result = 0;
    int ranking = 1;
    for (int i = 0; i <= int(HandType::FIVE_OF_A_KIND); i++) {
        HandType handType = static_cast<HandType>(i);
        for (const auto& hand : rankedHands[handType]) {
            result += bids[hand] * ranking;
            ranking++;
        }
    }
    cout << result << endl;
}

HandType getJokerHandType(const string& hand) {
    map<char, int> counter;
    int counts[5] = {};
    int numJokers = 0;
    for (auto& c : hand) {
        if (c != 'J') {
            counter[c]++;
        } else {
            numJokers++;
        }
    }
    vector<int> values;
    for (const auto& elem : counter) {
        values.push_back(elem.second);
    }
    if (!values.empty()) {
        sort(values.begin(), values.end());
        values[values.size() - 1] += numJokers;
        for (const auto& count : values) {
            counts[count - 1]++;
        }
        return getHandTypeFromCounts(counts);
    } else {
        return HandType::FIVE_OF_A_KIND;
    }
}

string jokerRanking = "J23456789TQKA";

void part2() {
    map<string, int> bids;
    parseInput(bids);
    map<HandType, vector<string>> rankedHands;
    for (const auto& elem : bids) {
        string hand = elem.first;
        HandType handType = getJokerHandType(hand);
        rankedHands[handType].push_back(hand);
    }
    // fully sort the hands of each type
    for (auto& elem : rankedHands) {
        auto& hands = elem.second;
        sort(hands.begin(), hands.end(), [](const auto& a, const auto& b) {
            for (int i = 0; i < a.size(); i++) {
                if (a[i] != b[i]) {
                    return jokerRanking.find(a[i]) < jokerRanking.find(b[i]);
                }
            }
            return false;  // should not be reachable
        });
    }
    // calculate ranking and total winnings
    long result = 0;
    int ranking = 1;
    for (int i = 0; i <= int(HandType::FIVE_OF_A_KIND); i++) {
        HandType handType = static_cast<HandType>(i);
        for (const auto& hand : rankedHands[handType]) {
            result += bids[hand] * ranking;
            ranking++;
        }
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}