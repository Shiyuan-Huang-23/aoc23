#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void part1() {
    ifstream strm;
    strm.open("../../inputs/day4.txt");
    string line;
    int result = 0;
    while (getline(strm, line)) {
        auto index = line.find(':');
        line = line.substr(index + 2);
        index = line.find('|');
        string winnersStr = line.substr(0, index - 1);
        stringstream ss1(winnersStr);
        int temp;
        set<int> winners;
        while (ss1 >> temp) {
            winners.insert(temp);
        }
        stringstream ss2(line.substr(index + 2));
        int points = 0;
        while (ss2 >> temp) {
            if (winners.count(temp) == 1) {
                if (points == 0) {
                    points = 1;
                } else {
                    points *= 2;
                }
            }
        }
        result += points;
    }
    cout << result << endl;
}

void part2() {
    ifstream strm;
    strm.open("../../inputs/day4.txt");
    string line;
    vector<int> cardsWon;
    vector<int> cardCounts;
    while (getline(strm, line)) {
        auto index = line.find(':');
        line = line.substr(index + 2);
        index = line.find('|');
        string winnersStr = line.substr(0, index - 1);
        stringstream ss1(winnersStr);
        int temp;
        set<int> winners;
        while (ss1 >> temp) {
            winners.insert(temp);
        }
        stringstream ss2(line.substr(index + 2));
        int numWinners = 0;
        while (ss2 >> temp) {
            if (winners.count(temp) == 1) {
                numWinners++;
            }
        }
        cardsWon.push_back(numWinners);
        cardCounts.push_back(1);
    }
    int result = 0;
    for (int i = 0; i < cardsWon.size(); i++) {
        for (int j = 1; j <= cardsWon[i] && i + j < cardsWon.size(); j++) {
            cardCounts[i + j] += cardCounts[i];
        }
        result += cardCounts[i];
    }
    cout << result << endl;
}

int main() {
    part1();
    part2();
}