#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <deque>

using namespace std;

// visited position of the hiking map
set<pair<long, long>> visitedPosition;
// the end position
pair<long, long> endPosition;

// get the longest distance from current position (row, col)
set<pair<long, long>> getTheLongestDistance(long row, long col, set<pair<long, long>> visitedPosition, vector<string>& hikingMap);
// get allowed next position
vector<pair<long, long>> getAllowedNextPosition(long row, long col, set<pair<long, long>>& visitedPosition, vector<string>& hikingMap);


int main()
{
    // init file
    ifstream infile("day23.txt");
    if (!infile.is_open())
    {
        cout << "Open file failed!" << endl;
    }

    // read file
    vector<string> hikingMap;
    string line;
    while (getline(infile, line))
    {
        if (!line.empty())
        {
            hikingMap.push_back(line);
        }
    }

    // fill end position
    endPosition = make_pair(hikingMap.size() - 1, hikingMap.back().size() - 2);

    // add the start position to the visited position
    visitedPosition.insert(make_pair(0, 1));
    // start to find the longest distance from (1,1)
    set<pair<long, long>> allVisitedPositions = getTheLongestDistance(1, 1, visitedPosition, hikingMap);

    std::cout << "step: " << allVisitedPositions.size() - 1 << endl;

    return 0;
}

set<pair<long, long>> getTheLongestDistance(long row, long col, set<pair<long, long>> visitedPosition, vector<string>& hikingMap)
{
    // make pair for the next position
    auto next = make_pair(row, col);
    // add it to the visited position
    visitedPosition.insert(next);
    // get the possible next positions
    vector<pair<long, long>> nextPositions = getAllowedNextPosition(next.first, next.second, visitedPosition, hikingMap);

    bool stopLoop = false;
    while (!stopLoop)
    {
        // cannot go further
        if (nextPositions.size() == 0)
        {
            visitedPosition.clear();
            stopLoop = true;           
        }
        // only one possible position
         else if (nextPositions.size() == 1)
        {
            next = nextPositions.front();
            visitedPosition.insert(next);
            // reach the end of the map
            if (next == endPosition)
            {
                stopLoop = true;
            }
            // not reach the end of the map
            else
            {
                nextPositions = getAllowedNextPosition(next.first, next.second, visitedPosition, hikingMap);
            }      
        }
        // multiple positions possible
        else
        {    
            set<pair<long, long>> maxVisitedPosition = visitedPosition;
            set<pair<long, long>> totalVisitedPosition;
            for (auto iter = nextPositions.begin(); iter != nextPositions.end(); iter++)
            {                             
                totalVisitedPosition = getTheLongestDistance(iter->first, iter->second, visitedPosition, hikingMap); 
                if (totalVisitedPosition.size() > maxVisitedPosition.size())
                {
                    maxVisitedPosition = totalVisitedPosition;
                }
            }

            // If there is no way to the end position, also clear the current visited position
            if (maxVisitedPosition.size() == visitedPosition.size())
            {
                visitedPosition.clear();
            }
            // Otherwise, update the visited position
            else
            {
                visitedPosition = maxVisitedPosition;
            }
            stopLoop = true;
        }
    }

    return visitedPosition;
}

vector<pair<long, long>> getAllowedNextPosition(long row, long col, set<pair<long, long>>& visitedPosition, vector<string>& hikingMap)
{
    // return value, the possible positions of next tile(s)
    vector<pair<long, long>> ret;
    // the tile type of current tile
    char currentTile = hikingMap.at(row).at(col);
    // next positions
    // north
    auto north = make_pair(row - 1, col);
    // south
    auto south = make_pair(row + 1, col);
    // west
    auto west = make_pair(row, col - 1);
    // east
    auto east = make_pair(row, col + 1);

    // if it is on slopes
    
    if (currentTile == '>' && visitedPosition.find(east) == visitedPosition.end() && hikingMap.at(east.first).at(east.second) != '#')
    {
        ret.push_back(east);
        return ret;
    }
    else if (currentTile == '>')
    {
        return ret;
    }

    if (currentTile == '<' && visitedPosition.find(west) == visitedPosition.end() && hikingMap.at(west.first).at(west.second) != '#')
    {
        ret.push_back(west);
        return ret;
    }
    else if (currentTile == '<')
    {
        return ret;
    }

    if (currentTile == '^' && visitedPosition.find(north) == visitedPosition.end() && hikingMap.at(north.first).at(north.second) != '#')
    {
        ret.push_back(north);
        return ret;
    }
    else if (currentTile == '^')
    {
        return ret;
    }

    if (currentTile == 'v' && visitedPosition.find(south) == visitedPosition.end() && hikingMap.at(south.first).at(south.second) != '#')
    {
        ret.push_back(south);
        return ret;
    }
    else if (currentTile == 'v')
    {
        return ret;
    }
    
    // if it is on path
    // north
    if (visitedPosition.find(north) == visitedPosition.end() && hikingMap.at(north.first).at(north.second) != 'v' && hikingMap.at(north.first).at(north.second) != '#')
    {
        ret.push_back(north);
    }

    // south
    if (visitedPosition.find(south) == visitedPosition.end() && hikingMap.at(south.first).at(south.second) != '^' && hikingMap.at(south.first).at(south.second) != '#')
    {
        ret.push_back(south);
    }

    // west
    if (visitedPosition.find(west) == visitedPosition.end() && hikingMap.at(west.first).at(west.second) != '>' && hikingMap.at(west.first).at(west.second) != '#')
    {
        ret.push_back(west);
    }

    // east
    if (visitedPosition.find(east) == visitedPosition.end() && hikingMap.at(east.first).at(east.second) != '<' && hikingMap.at(east.first).at(east.second) != '#')
    {
        ret.push_back(east);
    }
    

    return ret;
}