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

//static const unsigned long long expansionFactor = 2;               // part one
static const unsigned long long expansionFactor = 1000000;           // part two

// indicate if the row has galaxy or not
vector<bool> rowWithGalaxy;
// indicate if the column has galaxy or not
vector<bool> columnWithGalaxy;
// store the location of galaxies (row, column)
vector<std::pair<long, long>> galaxyPosition;

int main()
{
    // read file
    std::ifstream infile("day11.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // read all lines
    vector<string> image;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            image.push_back(line);
        }
    }
    
    // initialize vectors that indicate if galaxy found in the row or column
    rowWithGalaxy.insert(rowWithGalaxy.end(), image.size(), false);
    columnWithGalaxy.insert(columnWithGalaxy.end(), image.front().size(), false);

    // analyze each row of image
    for (auto iter = image.begin(); iter != image.end(); iter++)
    {
        bool hasGalaxyInRow = false;
        size_t startPosition = 0;
        while (iter->find('#', startPosition) != string::npos)
        {
            hasGalaxyInRow = true;

            size_t pos = iter->find('#', startPosition);
            columnWithGalaxy.at(pos) = true;
            startPosition = pos + 1;

            std::pair<long, long> positionOfGalaxy = make_pair(iter - image.begin(), pos);
            galaxyPosition.push_back(positionOfGalaxy);
        }

        if (hasGalaxyInRow)
        {
            rowWithGalaxy.at(iter - image.begin()) = true;
        }
    }

    unsigned long long totalDistance = 0;
    // calculate distance
    for (auto iter = galaxyPosition.begin(); iter != galaxyPosition.end() - 1; iter++)
    {
        for (auto iterSecond = iter + 1; iterSecond != galaxyPosition.end(); iterSecond++)
        {
            // basic distance before expansion 
            long long minRow = min(iter->first, iterSecond->first);
            long long maxRow = max(iter->first, iterSecond->first);
            long long minColumn = min(iter->second, iterSecond->second);
            long long maxColumn = max(iter->second, iterSecond->second);
            unsigned long long basicDistance = (maxRow - minRow) + (maxColumn - minColumn);

            // get distance after expansion of rows
            for (size_t indexRow = minRow; indexRow != maxRow; indexRow++)
            {
                if (!rowWithGalaxy.at(indexRow))
                {
                    basicDistance += (expansionFactor - 1);
                }
            }

            // get distance after expansion of columns
            for (size_t indexColumn = minColumn; indexColumn != maxColumn; indexColumn++)
            {
                if (!columnWithGalaxy.at(indexColumn))
                {
                    basicDistance += (expansionFactor - 1);
                }
            }

            // std::cout << "totalDistance: " << basicDistance << endl;
            // add distance to total distance
            totalDistance += basicDistance;
            
        }
    }
    
    std::cout << "totalDistance: " << totalDistance;// << "    " << "sumOfBackHistory: " << sumOfBackHistory << std::endl;
    return 0;
}

