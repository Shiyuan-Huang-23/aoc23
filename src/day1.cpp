#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void part1()
{
    ifstream strm;
    strm.open("../../inputs/day1.txt");
    string line;
    int result = 0;
    while (getline(strm, line))
    {
        int n = 0;
        for (int i = 0; i < line.length(); i++)
        {
            if (isdigit(line[i]))
            {
                n = (line[i] - '0') * 10;
                break;
            }
        }
        for (int i = line.length() - 1; i >= 0; i--)
        {
            if (isdigit(line[i]))
            {
                n += (line[i] - '0');
                break;
            }
        }
        result += n;
    }
    cout << result << endl;
    strm.close();
}

int NUM_DIGITS = 9;
string digitStrs[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

int findEarliestDigit(const string &line)
{
    int result = 0;
    int earliestPos = line.length();
    for (int i = 0; i < line.length(); i++)
    {
        if (isdigit(line[i]))
        {
            earliestPos = i;
            result = line[i] - '0';
            break;
        }
    }
    for (int i = 0; i < NUM_DIGITS; i++)
    {
        auto index = line.find(digitStrs[i]);
        if (index != std::string::npos && index < earliestPos)
        {
            earliestPos = index;
            result = i + 1;
        }
    }
    return result;
}

int findLatestDigit(const string &line)
{
    int result = 0;
    int latestPos = -1;
    for (int i = line.length() - 1; i >= 0; i--)
    {
        if (isdigit(line[i]))
        {
            latestPos = i;
            result = line[i] - '0';
            break;
        }
    }
    latestPos = max(latestPos, 0);
    for (int i = 0; i < NUM_DIGITS; i++)
    {
        auto index = line.find(digitStrs[i], latestPos);
        while (index != std::string::npos)
        {
            latestPos = index;
            result = i + 1;
            index = line.find(digitStrs[i], latestPos + 1); 
        }
    }
    return result;
}

void part2()
{
    ifstream strm;
    strm.open("../../inputs/day1.txt");
    string line;
    int result = 0;
    while (getline(strm, line))
    {
        result += findEarliestDigit(line) * 10 + findLatestDigit(line);
    }
    cout << result << endl;
    strm.close();
}

int main()
{
    part1();
    part2();
}