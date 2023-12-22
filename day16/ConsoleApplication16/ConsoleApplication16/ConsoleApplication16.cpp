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

// The input grid
vector<string> grid;

// The minimum/maximum allowed row/col index
long minRowIndex = 0;
long maxRowIndex = 0;
long minColIndex = 0;
long maxColIndex = 0;

// record if a tile has been visited or not. The directions to visit the tile have also been recorded
// <<row, col>, directions> 
// directions can be N/W/S/E
std::map<std::pair<long, long>, set<char>> visitedPositionAndDirection;

// the possitions where beam starts
vector<tuple<long, long, char>> possibleStartPositions;

// handle the visit of a tile
vector<tuple<long, long, char>> handleVisit(tuple<long, long, char>& currentTileToVisit);

// get all possible start positions
void getAllPossibleStartPositions(bool isPartOne);

int main()
{
    // read file
    std::ifstream infile("day16.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // read all lines
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            grid.push_back(line);
        }
    }
    
    // set max row index and max col index
    maxRowIndex = grid.size() - 1;
    maxColIndex = grid.front().size() - 1;

    // get all possible start positions, it is different between part one and part two
    // true => part one, false => part two
    getAllPossibleStartPositions(false);
    // the maximum visited tiles when start from different positions
    long long maxVisitedTiles = 0;
    // debug output
    long long visitedStartPosition = 0;

    // iterate the possible start positions
    for (auto iterStartPosition = possibleStartPositions.begin(); iterStartPosition != possibleStartPositions.end(); iterStartPosition++)
    {
        /* debug info */
        visitedStartPosition++;
        std::cout << "visited start position" << visitedStartPosition << " " << std::get<0>(*iterStartPosition) << " " << std::get<1>(*iterStartPosition) << endl;
        /***************/
        // reset the map for different start position
        visitedPositionAndDirection.clear();
        // tiles to be visited
        deque<tuple<long, long, char>> tilesToVisit;
        // initialize with the start position
        tilesToVisit.push_back(*iterStartPosition);

        // if there are still tiles to visit
        while (!tilesToVisit.empty())
        {
            // get the first tile from the queue
            tuple<long, long, char> currentTileToVisit = tilesToVisit.front();
            // handle the visit
            auto ret = handleVisit(currentTileToVisit);
            // if there will be new tiles to visit, add them to the queue
            if (!ret.empty())
            {
                tilesToVisit.insert(tilesToVisit.end(), ret.begin(), ret.end());
            }
            // remove the visited tile
            tilesToVisit.pop_front();
        }

        // all possible tiles have been visited, check how many tiles can be visited from this start position
        if (visitedPositionAndDirection.size() > maxVisitedTiles)
        {
            maxVisitedTiles = visitedPositionAndDirection.size();
        }
    }

    
    std::cout << "max visited tiles: " << maxVisitedTiles << endl;
    return 0;
}

vector<tuple<long, long, char>> handleVisit(tuple<long, long, char>& currentTileToVisit)
{
    // return value
    // used to store tiles to be visited next
    vector<tuple<long, long, char>> ret;

    // get information for current visit
    long long row;
    long long col;
    char direction;
    tie(row, col, direction) = currentTileToVisit;

    // if the row or col index is out of range, cannot visit and return
    if (row < minRowIndex || row > maxRowIndex || col < minColIndex || col > maxColIndex)
    {
        return ret;
    }
    
    // check if the current tile has already been visited
    auto tilePosition = make_pair(row, col);
    // not ever been visited
    if (visitedPositionAndDirection.find(tilePosition) == visitedPositionAndDirection.end())
    {
        visitedPositionAndDirection[tilePosition] = set<char>();
        visitedPositionAndDirection[tilePosition].insert(direction);
    }
    // already visited, but not in current direction
    else if (visitedPositionAndDirection[tilePosition].find(direction) == visitedPositionAndDirection[tilePosition].end())
    {
        visitedPositionAndDirection[tilePosition].insert(direction);
    }
    // already visited and in the same direction
    else
    {
        // there is no need to visit again, return
        return ret;
    }
  
    
    // get char of the current tile
    char tile = grid.at(row).at(col);

    // get the next tile(s) to visit according to current tile char and direction
    switch (direction)
    {
    case 'N':
        if (tile == '.')
        {
            ret.push_back(make_tuple(row - 1, col, 'N'));
        }
        else if (tile == '/')
        {
            ret.push_back(make_tuple(row, col + 1, 'E'));
        }
        else if (tile == '\\')
        {
            ret.push_back(make_tuple(row, col - 1, 'W'));
        }
        else if (tile == '-')
        {
            ret.push_back(make_tuple(row, col + 1, 'E'));
            ret.push_back(make_tuple(row, col - 1, 'W'));
        }
        else if (tile == '|')
        {
            ret.push_back(make_tuple(row - 1, col, 'N'));
        }
        else
        {
            cout << "something is wrong during visit handling" << endl;
        }
        break;
    case 'W':
        if (tile == '.')
        {
            ret.push_back(make_tuple(row, col - 1, 'W'));
        }
        else if (tile == '/')
        {
            ret.push_back(make_tuple(row + 1, col, 'S'));
        }
        else if (tile == '\\')
        {
            ret.push_back(make_tuple(row - 1, col, 'N'));
        }
        else if (tile == '-')
        {
            ret.push_back(make_tuple(row, col - 1, 'W'));
        }
        else if (tile == '|')
        {
            ret.push_back(make_tuple(row + 1, col, 'S'));
            ret.push_back(make_tuple(row - 1, col, 'N'));
        }
        else
        {
            cout << "something is wrong during visit handling" << endl;
        }
        break;
    case 'S':
        if (tile == '.')
        {
            ret.push_back(make_tuple(row + 1, col, 'S'));
        }
        else if (tile == '/')
        {
            ret.push_back(make_tuple(row, col - 1, 'W'));
        }
        else if (tile == '\\')
        {
            ret.push_back(make_tuple(row, col + 1, 'E'));
        }
        else if (tile == '-')
        {
            ret.push_back(make_tuple(row, col - 1, 'W'));
            ret.push_back(make_tuple(row, col + 1, 'E'));
        }
        else if (tile == '|')
        {
            ret.push_back(make_tuple(row + 1, col, 'S'));
        }
        else
        {
            cout << "something is wrong during visit handling" << endl;
        }
        break;
    case 'E':
        if (tile == '.')
        {
            ret.push_back(make_tuple(row, col + 1, 'E'));
        }
        else if (tile == '/')
        {
            ret.push_back(make_tuple(row - 1, col, 'N'));
        }
        else if (tile == '\\')
        {
            ret.push_back(make_tuple(row + 1, col, 'S'));
        }
        else if (tile == '-')
        {
            ret.push_back(make_tuple(row, col + 1, 'E'));
        }
        else if (tile == '|')
        {
            ret.push_back(make_tuple(row - 1, col, 'N'));
            ret.push_back(make_tuple(row + 1, col, 'S'));
        }
        else
        {
            cout << "something is wrong during visit handling" << endl;
        }
        break;
    default:
        break;
    }

    return ret;
}

void getAllPossibleStartPositions(bool isPartOne)
{
    // part one
    if (isPartOne)
    {
        possibleStartPositions.push_back(make_tuple(0, 0, 'E'));
    }
    // part two
    else
    {
        for (int rowIndex = minRowIndex; rowIndex <= maxRowIndex; rowIndex++)
        {
            possibleStartPositions.push_back(make_tuple(rowIndex, minColIndex, 'E'));
            possibleStartPositions.push_back(make_tuple(rowIndex, maxColIndex, 'W'));
        }

        for (int colIndex = minColIndex; colIndex <= maxColIndex; colIndex++)
        {
            possibleStartPositions.push_back(make_tuple(minRowIndex, colIndex, 'S'));
            possibleStartPositions.push_back(make_tuple(maxRowIndex, colIndex, 'N'));
        }
    }

}
