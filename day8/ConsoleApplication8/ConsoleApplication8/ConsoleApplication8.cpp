#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include<algorithm>
using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';
static const char verticalDelimiter = '|';

std::vector<string> keyName;
std::map<string, string> mapLeft;
std::map<string, string> mapRight;

long long getLCM(long long first, long long second);
long long getGCD(long long first, long long second);

int main()
{
    // read file
    std::ifstream infile("day8.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // get instructions
    std::string instruction = "";
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            instruction.append(line);
        }
        else
        {
            break;
        }
    }

    while (std::getline(infile, line))
    {
        string key = line.substr(0, 3);
        string left = line.substr(line.find("(") + 1, 3);
        string right = line.substr(line.find(")") - 3, 3);

        keyName.push_back(key);
        mapLeft[key] = left;
        mapRight[key] = right;
    }

    // part one
    /*
    bool findExit = false;
    string currentKey = "AAA";
    long long steps = 0;
    size_t index = 0;
    size_t sizeOfInstructions = instruction.size();
    while (!findExit)
    {
        char currentInstruction = instruction[index % sizeOfInstructions];
        if (currentInstruction == 'L')
        {
            currentKey = mapLeft[currentKey];
        }
        else if (currentInstruction == 'R')
        {
            currentKey = mapRight[currentKey];
        }

        steps++;
        index++;

        if (currentKey.compare("ZZZ") == 0)
        {
            findExit = true;
        }
    }

    std::cout << "steps: " << steps << std::endl;

    */

    // part two
    // get all start point
    std::vector<string> startPosition;
    for (auto iter = keyName.begin(); iter != keyName.end(); iter++)
    {
        if (iter->back() == 'A')
        {
            startPosition.push_back(*iter);
        }
    }

    // the vector to store steps for each startPosition
    vector<long long> stepsOfAll;
    for (auto iter = startPosition.begin(); iter != startPosition.end(); iter++)
    {
        string currentKey = *iter;
        bool findExit = false;
        long long steps = 0;
        size_t index = 0;
        size_t sizeOfInstructions = instruction.size();
        while (!findExit)
        {
            char currentInstruction = instruction[index % sizeOfInstructions];
            if (currentInstruction == 'L')
            {
                currentKey = mapLeft[currentKey];
            }
            else if (currentInstruction == 'R')
            {
                currentKey = mapRight[currentKey];
            }

            steps++;
            index++;

            if (currentKey.back() == 'Z')
            {
                findExit = true;
                stepsOfAll.push_back(steps);
                cout << *iter << " " << currentKey <<endl;
            }

        }
    }
    
    // get the LCM of all steps
    long long partTwoResult = * stepsOfAll.begin();
    for (auto iter = stepsOfAll.begin() + 1; iter != stepsOfAll.end(); iter++)
    {
        partTwoResult = getLCM(partTwoResult, *iter);
    }
    
    std::cout << "partTwoResult: " << partTwoResult << std::endl;
    return 0;
}

long long getLCM(long long first, long long second)
{
    return first * second / getGCD(first, second);
}

long long getGCD(long long first, long long second)
{
    long long larger = max(first, second);
    long long smaller = min(first, second);
   
    while (larger % smaller)
    {
        long long q = larger % smaller;
        larger = smaller;
        smaller = q;
    }

    return smaller;
}