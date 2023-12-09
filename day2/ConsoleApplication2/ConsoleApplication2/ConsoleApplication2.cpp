#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';

vector<string> colorKey;
std::map<string, int> maximumAllowedCubes;

void init();
void getTotalIdAndPower(std::string& cubeString, bool& isPossible, unsigned long long& power);

int main()
{
    init();
    // read file
    std::ifstream infile("day2.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    std::string line;
    unsigned long long totalId = 0;             // part one final result
    unsigned long long totalPower = 0;          // part two final result

    // get each line of the file
    while (std::getline(infile, line))
    {
        // a line is like "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
        std::stringstream stringString(line);
        std::string roundString;                            // to calculate round => "Game 1"
        std::string cubeString;                             // to check cube => " 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
        std::getline(stringString, roundString, delimiter);
        std::getline(stringString, cubeString, delimiter);

        // two variables to get the result of each line
        bool isPossible = false;                // result for part one
        unsigned long long power = 0;           // result for part two
        getTotalIdAndPower(cubeString, isPossible, power);

        // it is possible for question one, add the id
        if (isPossible)
        {
            // get round number
            std::stringstream ssRound(roundString);
            string round;
            while (std::getline(ssRound, round, spaceDelimiter))
            {
                // do nothing, just get the number
            }
            int roundNumber = stoi(round);
            totalId += roundNumber;
        }

        // add the power for question two
        totalPower += power; 
    }

    std::cout << "totalId: " << totalId << std::endl << "totalPower: " << totalPower << std::endl;
    return 0;
}

void init()
{
    colorKey.push_back("red");
    colorKey.push_back("green");
    colorKey.push_back("blue");

    maximumAllowedCubes["red"] = 12;
    maximumAllowedCubes["green"] = 13;
    maximumAllowedCubes["blue"] = 14;
}

void getTotalIdAndPower(std::string& cubeString, bool& isPossible, unsigned long long& power)
{
    // variables to calculate power
    map<string, unsigned long long> maximumSeenCubes;
    // set maximumSeenCubes["red"] maximumSeenCubes["green"] maximumSeenCubes["blue"] to 0
    for (auto iterKey = colorKey.begin(); iterKey != colorKey.end(); iterKey++)
    {
        maximumSeenCubes[*iterKey] = 0;
    }

    // check if it is possible for question one
    bool possibleResult = true;

    // cube string is like " 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
    std::stringstream ss(cubeString);
    string result;                          // each result is like " 3 blue, 4 red", " 1 red, 2 green, 6 blue", " 2 green"
    vector<std::string> results;
    // get all results from the cubeString
    while (getline(ss, result, semiDelimiter))
    {
        results.push_back(result);
    }

    // analyze each result
    for (auto iter = results.begin();  iter != results.end(); iter++)
    {
        std::stringstream ssResult(*iter);
        string cubeResult;                      // each cubeResult is like " 3 blue", " 4 red"
        // analyze each cubeResult
        while (getline(ssResult, cubeResult, commaDelimiter))
        {
            // iterate colors to check which color is in the cubeResult
            for (auto iterKey = colorKey.begin(); iterKey != colorKey.end(); iterKey++)
            {
                if (cubeResult.find(*iterKey) != string::npos)
                {
                    // get Number
                    size_t pos = cubeResult.find(*iterKey);
                    string numberString = cubeResult.substr(0, pos);
                    int number = stoi(numberString);
                    // check number for question one
                    if (possibleResult && number > maximumAllowedCubes[*iterKey])
                    {
                        possibleResult = false;
                    }
                    // check number for question two
                    if (number > maximumSeenCubes[*iterKey])
                    {
                        maximumSeenCubes[*iterKey] = number;
                    }
                }
            }            
        }
    }

    isPossible = possibleResult;
    power = 1;
    for (auto iterKey = colorKey.begin(); iterKey != colorKey.end(); iterKey++)
    {
        power *= maximumSeenCubes[*iterKey];
    }
}
