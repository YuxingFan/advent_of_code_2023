#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';
static const char verticalDelimiter = '|';

std::set<std::pair<long, long>> visitedPosition;
std::map<std::pair<long, long>, int> passedPoints;
std::map<std::pair<long, long>, int> passedBackSlashs;
std::map<std::pair<long, long>, int> passedForwardSlashs;
std::map<std::pair<long, long>, int> passedBackVerSpliters;
std::map<std::pair<long, long>, int> passedBackHorSpliters;

int main()
{
    // read file
    std::ifstream infile("example.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // read all lines
    vector<string> grid;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            grid.push_back(line);
        }
    }

    //long long numOfRows = grid.size();
    //long long numOfColumns = grid.front().size();
    std::pair<long, long> currentPosition = std::make_pair(0, 0);
    char currentDirecction = 'E';
    std::pair<std::pair<long, long>, char> beamProperty = std::make_pair(currentPosition, currentDirecction);

    std::vector<std::pair<std::pair<long, long>, char>> totalBeams;
    totalBeams.push_back(beamProperty);

    while (!totalBeams.empty())
    {
        auto iter = totalBeams.begin();
        auto& currentBeam = *iter;

        auto currentPosition = currentBeam.first;
        auto currentDirection = currentBeam.second;

        // not visited yet
        if (visitedPosition.find(currentPosition) == visitedPosition.end())
        {
            char currentTile = grid.at(currentPosition.first).at(currentPosition.second);
            addVisitRecord(currentPosition, currentDirection, currentTile);
        }

        bool stop = stopFurtherSearch(currentBeam, grid);


        if (stop)
        {
            totalBeams.erase(iter);
        }
    }

    // the vector used to store the total load of each column
    vector<long long> totalLoadOfColumn(numOfColumns, 0);
    // the load for next 'O'
    vector<long long> nextLoadOfColumn(numOfColumns, maxLoad);

    // iterate rows
    for (auto iter = grid.begin(); iter != grid.end(); iter++)
    {
        // iterate columns
        for (size_t index = 0; index < numOfColumns; index++)
        {
            if ((*iter)[index] == '.')
            {
                // do nothing
            }
            else if ((*iter)[index] == '#')
            {
                // reset the load for current column
                nextLoadOfColumn.at(index) = maxLoad - (iter - grid.begin()) - 1;
            }
            else if ((*iter)[index] == 'O')
            {
                // add load for current column
                totalLoadOfColumn.at(index) += nextLoadOfColumn.at(index);
                // update load
                nextLoadOfColumn.at(index) -= 1;

            }
        }
    }

    // get result of partOne
    long long sumOfAll = 0;
    for (auto iter = totalLoadOfColumn.begin(); iter != totalLoadOfColumn.end(); iter++)
    {
        sumOfAll += *iter;
    }

    std::cout << "sumOfAll: " << sumOfAll;// << "    " << "sumOfBackHistory: " << sumOfBackHistory << std::endl;
    return 0;
}

void addVisitRecord(std::pair<long, long>& currentPosition, char currentDirection, char currentTile)
{
    if (currentTile == '.' && (currentDirection == 'E' || currentDirection == 'W'))
    {
        passedPoints
    }
}

void getNextPosition(std::pair<std::pair<long, long>, char>& currentProperty, std::pair<std::pair<long, long>, char>& nextProperty, vector<string>& grid)
{

}

bool willExceedBorder(std::pair<std::pair<long, long>, char>& beamProperty, vector<string>& grid)
{
    auto currentPosition = beamProperty.first;
    auto currentDirection = beamProperty.second;
    char currentTile = grid.at(currentPosition.first).at(currentPosition.second);

    //long long numOfRows = grid.size();
    //long long numOfColumns = grid.front().size();

}

bool stopFurtherSearch(std::pair<std::pair<long, long>, char>& currentBeam, vector<string>& grid)
{
    bool ret;
    auto currentPosition = currentBeam.first;
    auto currentDirection = currentBeam.second;
    char currentTile = grid.at(currentPosition.first).at(currentPosition.second);

    long long numOfRows = grid.size();
    long long numOfColumns = grid.front().size();

    if (currentTile == '.')
    {

    }
    else if (currentTile == '/')
    {

    }
    else if (currentTile == '\\')
    {

    }
    else if (currentTile == '-')
    {

    }
    else if (currentTile == '|')
    {

    }
    
}
