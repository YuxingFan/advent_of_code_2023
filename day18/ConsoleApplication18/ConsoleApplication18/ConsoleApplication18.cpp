#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <tuple>
#include <deque>
using namespace std;

// <row, <col, <in, out>>>
map<int, map<int, pair<char, char>>> visitedPosition;
pair<int, int> currentPosition;

// handle a line of the plan
void handlePlan(string& plan);
// get offset according to the direction
void getOffset(int& row, int& col, char& direction);
// fill the visitedPosition map according to the operation
void fillMap(int row, int col, bool isIn, char direction);

int main()
{
    // read file
    std::ifstream infile("day18.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // set init position
    currentPosition = make_pair(0, 0);
    // read all lines
    string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            handlePlan(line);
        }
    }

    long totalVolumn = 0;
    // iterate each row
    for (auto iter = visitedPosition.begin(); iter != visitedPosition.end(); iter++)
    {
        // all visited columns
        auto& visitedColumns = iter->second;

        // the start position for 'U' or 'D'
        bool foundStartIndex = false;
        char startChar = '-';
        int startIndex = -1;
        
        // the end position for 'U' or 'D'
        bool foundEndIndex = false;
        char endChar = '-';
        int endIndex = -1;

        // check if trench available between end position and next start position
        bool hasTrenchBetweenEndAndStart = false;

        // iterate the column
        for (auto iterCol = visitedColumns.begin(); iterCol != visitedColumns.end(); iterCol++)
        {
            // get the column index and in/out direction
            auto colIndex = iterCol->first;
            auto direction = iterCol->second;
            // if the direction is 'U' or 'D'
            if (direction.first == 'U' || direction.first == 'D' || direction.second == 'U' || direction.second == 'D')
            {
                // get the 'U' or 'D'
                char desiredDirection;
                if (direction.first == 'U' || direction.first == 'D')
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
                    startChar = desiredDirection;
                    startIndex = colIndex;
                }
                // get the end position
                else if (foundStartIndex && !foundEndIndex && startChar != desiredDirection)
                {
                    foundEndIndex = true;
                    endChar = desiredDirection;
                    endIndex = colIndex;
                }
                // extend end position
                else if (foundStartIndex && foundEndIndex && endChar == desiredDirection)
                {
                    endIndex = colIndex;
                    hasTrenchBetweenEndAndStart = false;
                }
                // find the second start position
                else if (foundStartIndex && foundEndIndex && endChar != desiredDirection)
                {
                    totalVolumn += (endIndex - startIndex + 1);

                    // if we have trench between end position and the second start position, add them to total number
                    if (hasTrenchBetweenEndAndStart)
                    {
                        totalVolumn += (colIndex - endIndex - 1);
                        hasTrenchBetweenEndAndStart = false;
                    }

                    startChar = desiredDirection;
                    startIndex = colIndex;

                    foundEndIndex = false;
                    endChar = '-';
                    endIndex = -1;
                }
            }
            // if the direction is 'L' or 'R', and end position has been found
            else if (foundEndIndex)
            {
                hasTrenchBetweenEndAndStart = true;
            }
               
        }

        if (foundStartIndex && foundEndIndex)
        {
            totalVolumn += (endIndex - startIndex + 1);
        }
        else
        {
            std::cout << "something wrong" << endl;
        }
    }

    std::cout << "totalVolumn: " << totalVolumn << endl;
    return 0;
}

void handlePlan(string& plan)
{
    // get direction from the string
    char direction = plan[0];
    // get meters from the string
    int steps = stoi(plan.substr(2));

    // get offset for each step
    int rowOffset;
    int colOffset;
    getOffset(rowOffset, colOffset, direction);

    for (int step = 0; step < steps; step++)
    {
        fillMap(currentPosition.first, currentPosition.second, false, direction);
        currentPosition.first += rowOffset;
        currentPosition.second += colOffset;
        fillMap(currentPosition.first, currentPosition.second, true, direction);

       // cout << currentPosition.first << " " << currentPosition.second << endl;
    }

}

void getOffset(int& row, int& col, char& direction)
{
    switch (direction)
    {
    case 'U':
        row = -1;
        col = 0;
        break;
    case 'D':
        row = 1;
        col = 0;
        break;
    case 'L':
        row = 0;
        col = -1;
        break;
    case 'R':
        row = 0;
        col = 1;
        break;
    default:
        break;
    }
}

void fillMap(int row, int col, bool isIn, char direction)
{
    // if the row is not visited yet
    if (visitedPosition.find(row) == visitedPosition.end())
    {
        visitedPosition[row] = map<int, pair<char, char>>();
    }
    
    // if the col is not visited yet
    if (visitedPosition[row].find(col) == visitedPosition[row].end())
    {
        visitedPosition[row][col] = make_pair('-', '-');
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