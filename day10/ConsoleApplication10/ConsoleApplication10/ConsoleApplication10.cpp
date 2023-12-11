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

enum MoveDirection 
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

long findLongestDistance(vector<string>& inputPipes, std::pair<long, long> startPosition);

int main()
{
    // read file
    std::ifstream infile("day10.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // the list to store data
    vector<string> inputPipes;
    // placeholder for the first additional line
    inputPipes.push_back("");
    // the position of start "S"
    std::pair<long, long> startPosition;

    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty())
        {
            continue;
        }
        string extendedLine = ".";
        extendedLine.append(line);
        extendedLine.append(".");

        if (extendedLine.find("S") != string::npos)
        {
            long rowPositionIndex = inputPipes.size();
            long columnPositionIndex = extendedLine.find("S");
            startPosition = make_pair(rowPositionIndex, columnPositionIndex);
        }

        inputPipes.push_back(extendedLine);
    }

    string border = string(inputPipes.back().size() + 2, '.');
    inputPipes.front() = border;
    inputPipes.push_back(border);

    long longestDistance = findLongestDistance(inputPipes, startPosition);

    std::cout << "longestDistance: " << longestDistance;// << "    " << "sumOfBackHistory: " << sumOfBackHistory << std::endl;
    return 0;
}


long findLongestDistance(vector<string>& inputPipes, std::pair<long, long> startPosition)
{
    map<std::pair<long, long>, enum MoveDirection> positionsAroundStartPosition;
    positionsAroundStartPosition[std::make_pair(startPosition.first - 1, startPosition.second)] = MoveDirection::NORTH;
    positionsAroundStartPosition[std::make_pair(startPosition.first + 1, startPosition.second)] = MoveDirection::SOUTH;
    positionsAroundStartPosition[std::make_pair(startPosition.first, startPosition.second - 1)] = MoveDirection::WEST;
    positionsAroundStartPosition[std::make_pair(startPosition.first, startPosition.second + 1)] = MoveDirection::EAST;

    for (auto iter = positionsAroundStartPosition.begin(); iter != positionsAroundStartPosition.end(); iter++)
    {
        long row = iter->first.first;
        long column = iter->first.second;
        enum MoveDirection direction = iter->second;
        long steps = 1;
        char pipe = inputPipes.at(row)[column];
        
        bool stopSearching = false;
        bool returnToStartPosition = false;
        
        while (!stopSearching)
        {
            if (pipe == '|' && direction == MoveDirection::NORTH)
            {
                row--;
            }
            else if (pipe == '|' && direction == MoveDirection::SOUTH)
            {
                row++;
            }
            else if (pipe == '-' && direction == MoveDirection::WEST)
            {
                column--;
            }
            else if (pipe == '-' && direction == MoveDirection::EAST)
            {
                column++;
            }
            else if (pipe == 'L' && direction == MoveDirection::SOUTH)
            {
                column++;
                direction = MoveDirection::EAST;
            }
            else if (pipe == 'L' && direction == MoveDirection::WEST)
            {
                row--;
                direction = MoveDirection::NORTH;
            }
            else if (pipe == 'J' && direction == MoveDirection::SOUTH)
            {
                column--;
                direction = MoveDirection::WEST;
            }
            else if (pipe == 'J' && direction == MoveDirection::EAST)
            {
                row--;
                direction = MoveDirection::NORTH;
            }
            else if (pipe == '7' && direction == MoveDirection::NORTH)
            {
                column--;
                direction = MoveDirection::WEST;
            }
            else if (pipe == '7' && direction == MoveDirection::EAST)
            {
                row++;
                direction = MoveDirection::SOUTH;
            }
            else if (pipe == 'F' && direction == MoveDirection::NORTH)
            {
                column++;
                direction = MoveDirection::EAST;
            }
            else if (pipe == 'F' && direction == MoveDirection::WEST)
            {
                row++;
                direction = MoveDirection::SOUTH;
            }
            else if (pipe == '.')
            {
                stopSearching = true;
            }
            else if (pipe == 'S')
            {
                stopSearching = true;
                returnToStartPosition = true;
            } 
            else
            {
                stopSearching = true;
            }

            if (!stopSearching)
            {
                steps++;
                pipe = inputPipes.at(row)[column];
            }   
        }

        if (returnToStartPosition)
        {
            return steps / 2;
        }
    }

    return 0;
}
