#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include<algorithm>
using namespace std;

// define the move direction
enum MoveDirection 
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

// A map to store the position of visited tiles and the in/out direction to visit the tile
// <row, <col, <in_direction, out_direction>>>
map<int, map<int, pair<int, int>>> visitedPosition;

// get the longest distance along the loop from the start position
long findLongestDistance(vector<string>& inputPipes, std::pair<long, long> startPosition);

// a function to fill the map visitedPosition
void fillMap(int row, int col, bool isIn, char direction);

// get total tiles in the loop, including the visited border tiles and the enclosed tiles
long getTotalTiles();

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
        // add a border for the 2D input
        /*
            .........
            .       .
            .       .
            . input .
            .       .
            .       .
            .........
        */
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

    // add the top and buttom border for the 2D input
    string border = string(inputPipes.back().size() + 2, '.');
    inputPipes.front() = border;
    inputPipes.push_back(border);

    // get the longest distance from start position
    long longestDistance = findLongestDistance(inputPipes, startPosition);
    // get all tiles, including the visited border tiles and the enclosed tiles
    long totalTiles = getTotalTiles();
    // get enclosed tiles
    long enclosedTiles = totalTiles - longestDistance * 2;

    std::cout << "longestDistance: " << longestDistance << "    " << "enclosedTiles: " << enclosedTiles << std::endl;
    return 0;
}


long findLongestDistance(vector<string>& inputPipes, std::pair<long, long> startPosition)
{
    // initialize possible first steps
    map<std::pair<long, long>, enum MoveDirection> positionsAroundStartPosition;
    positionsAroundStartPosition[std::make_pair(startPosition.first - 1, startPosition.second)] = MoveDirection::NORTH;
    positionsAroundStartPosition[std::make_pair(startPosition.first + 1, startPosition.second)] = MoveDirection::SOUTH;
    positionsAroundStartPosition[std::make_pair(startPosition.first, startPosition.second - 1)] = MoveDirection::WEST;
    positionsAroundStartPosition[std::make_pair(startPosition.first, startPosition.second + 1)] = MoveDirection::EAST;

    // iterate possible first steps
    for (auto iter = positionsAroundStartPosition.begin(); iter != positionsAroundStartPosition.end(); iter++)
    {
        // reset visited position map
        visitedPosition.clear();
        // fill the out direction for 'S'
        fillMap(startPosition.first, startPosition.second, false, iter->second);

        // get the position an in direction of the first step
        long row = iter->first.first;
        long column = iter->first.second;
        enum MoveDirection direction = iter->second;
        // count steps form start position
        long steps = 1;
        // get the pipe type at the position
        char pipe = inputPipes.at(row)[column];
        // fill the in direction for the first step
        fillMap(row, column, true, direction);

        // stop further searching, e.g. if it walks back to the 'S' (finish the loop) or it cannot go further (i.e. not in the loop)
        bool stopSearching = false;
        // indicate if it walks back to 'S'
        bool returnToStartPosition = false;
        
        // walking
        while (!stopSearching)
        {
            // if the pipe is reached with reasonable move direction
            // update the position(row/column) to next pipe
            if (pipe == '|' && direction == MoveDirection::NORTH)
            {
                fillMap(row, column, false, direction);
                row--;
                fillMap(row, column, true, direction);
            }
            else if (pipe == '|' && direction == MoveDirection::SOUTH)
            {
                fillMap(row, column, false, direction);
                row++;
                fillMap(row, column, true, direction);
            }
            else if (pipe == '-' && direction == MoveDirection::WEST)
            {
                fillMap(row, column, false, direction);
                column--;
                fillMap(row, column, true, direction);
            }
            else if (pipe == '-' && direction == MoveDirection::EAST)
            {
                fillMap(row, column, false, direction);
                column++;
                fillMap(row, column, true, direction);
            }
            else if (pipe == 'L' && direction == MoveDirection::SOUTH)
            {
                direction = MoveDirection::EAST;
                fillMap(row, column, false, direction);
                column++;
                fillMap(row, column, true, direction);
            }
            else if (pipe == 'L' && direction == MoveDirection::WEST)
            {
                direction = MoveDirection::NORTH;
                fillMap(row, column, false, direction);
                row--;
                fillMap(row, column, true, direction);
            }
            else if (pipe == 'J' && direction == MoveDirection::SOUTH)
            {
                direction = MoveDirection::WEST;
                fillMap(row, column, false, direction);
                column--;
                fillMap(row, column, true, direction);
            }
            else if (pipe == 'J' && direction == MoveDirection::EAST)
            {
                direction = MoveDirection::NORTH;
                fillMap(row, column, false, direction);
                row--;
                fillMap(row, column, true, direction);
            }
            else if (pipe == '7' && direction == MoveDirection::NORTH)
            {
                direction = MoveDirection::WEST;
                fillMap(row, column, false, direction);
                column--;
                fillMap(row, column, true, direction);
            }
            else if (pipe == '7' && direction == MoveDirection::EAST)
            {
                direction = MoveDirection::SOUTH;
                fillMap(row, column, false, direction);
                row++;
                fillMap(row, column, true, direction);
            }
            else if (pipe == 'F' && direction == MoveDirection::NORTH)
            {
                direction = MoveDirection::EAST;
                fillMap(row, column, false, direction);
                column++;
                fillMap(row, column, true, direction);
            }
            else if (pipe == 'F' && direction == MoveDirection::WEST)
            {
                direction = MoveDirection::SOUTH;
                fillMap(row, column, false, direction);
                row++;
                fillMap(row, column, true, direction);
            }
            // if the pipe is '.', it is not in a loop, stop further walking
            else if (pipe == '.')
            {
                stopSearching = true;
            }
            // it walks back to the start position, stop walking
            else if (pipe == 'S')
            {
                stopSearching = true;
                returnToStartPosition = true;
            } 
            // something is wrong, this should never happen
            else
            {
                stopSearching = true;
            }

            // if further search is possible
            if (!stopSearching)
            {
                // go to next position
                steps++;
                // get the pipe type of next position 
                pipe = inputPipes.at(row)[column];
            }   
        }

        // if it walks back to the start position, return the longest distance from start position
        if (returnToStartPosition)
        {
            return steps / 2;
        }
    }

    // this should never happen
    return 0;
}

void fillMap(int row, int col, bool isIn, char direction)
{
    // if the row is not visited yet
    if (visitedPosition.find(row) == visitedPosition.end())
    {
        visitedPosition[row] = map<int, pair<int, int>>();
    }

    // if the col is not visited yet
    if (visitedPosition[row].find(col) == visitedPosition[row].end())
    {
        visitedPosition[row][col] = make_pair(-1, -1);
    }

    if (isIn)
    {
        visitedPosition[row][col].first = direction;
    }
    else
    {
        visitedPosition[row][col].second = direction;
    }
}

// this is the method used in day18, reuse it for day10
long getTotalTiles()
{
    long totalTiles = 0;
    // iterate each row
    for (auto iter = visitedPosition.begin(); iter != visitedPosition.end(); iter++)
    {
        // all visited columns
        auto& visitedColumns = iter->second;

        // the start position for 'North' or 'South'
        bool foundStartIndex = false;
        int startDirection = -1;
        int startIndex = -1;

        // the end position for 'North' or 'South'
        bool foundEndIndex = false;
        int endDirection = -1;
        int endIndex = -1;

        // check if trench available between end position and next start position
        bool hasTrenchBetweenEndAndStart = false;

        // iterate the column
        for (auto iterCol = visitedColumns.begin(); iterCol != visitedColumns.end(); iterCol++)
        {
            // get the column index and in/out direction
            auto colIndex = iterCol->first;
            auto direction = iterCol->second;
            // if the direction is 'North' or 'South'
            if (direction.first == MoveDirection::NORTH || direction.first == MoveDirection::SOUTH || direction.second == MoveDirection::NORTH || direction.second == MoveDirection::SOUTH)
            {
                // get the 'North' or 'South'
                char desiredDirection;
                if (direction.first == MoveDirection::NORTH || direction.first == MoveDirection::SOUTH)
                {
                    desiredDirection = direction.first;
                }
                else
                {
                    desiredDirection = direction.second;
                }

                // get the start position
                if (!foundStartIndex && !foundEndIndex)
                {
                    foundStartIndex = true;
                    startDirection = desiredDirection;
                    startIndex = colIndex;
                }
                // get the end position
                else if (foundStartIndex && !foundEndIndex && startDirection != desiredDirection)
                {
                    foundEndIndex = true;
                    endDirection = desiredDirection;
                    endIndex = colIndex;
                }
                // extend end position
                else if (foundStartIndex && foundEndIndex && endDirection == desiredDirection)
                {
                    endIndex = colIndex;
                    hasTrenchBetweenEndAndStart = false;
                }
                // find the second start position
                else if (foundStartIndex && foundEndIndex && endDirection != desiredDirection)
                {
                    totalTiles += (endIndex - startIndex + 1);

                    // if we have trench between end position and the second start position, add them to total number
                    if (hasTrenchBetweenEndAndStart)
                    {
                        totalTiles += (colIndex - endIndex - 1);
                        hasTrenchBetweenEndAndStart = false;
                    }

                    startDirection = desiredDirection;
                    startIndex = colIndex;

                    foundEndIndex = false;
                    endDirection = -1;
                    endIndex = -1;
                }
            }
            // if the direction is 'WEST' or 'EAST', and end position has been found
            else if (foundEndIndex)
            {
                hasTrenchBetweenEndAndStart = true;
            }

        }

        if (foundStartIndex && foundEndIndex)
        {
            totalTiles += (endIndex - startIndex + 1);
        }
        else
        {
            std::cout << "something wrong" << endl;
        }
    }

    return totalTiles;
}