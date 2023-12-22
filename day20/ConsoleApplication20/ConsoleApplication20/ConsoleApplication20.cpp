#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include <algorithm>
#include <set>
using namespace std;

set<pair<long, long>> reachableTiles;
set<pair<long, long>> tilesToSearch;

static const int desiaredSteps = 64;

int main()
{
    // read file
    std::ifstream infile("day20.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // <row, column>
    pair<long, long> startPosition;
    // read input
    vector<string> gardenMap;
    std::string line = "";
    gardenMap.push_back(line);
    while (getline(infile, line))
    {
        if (!line.empty())
        {
            string extendedLine = "#";
            extendedLine.append(line);
            extendedLine.push_back('#');

            if (extendedLine.find("S") != string::npos)
            {
                startPosition = std::make_pair(gardenMap.size(), extendedLine.find("S"));
            }
            gardenMap.push_back(extendedLine);
        }
    }

    string rocks('#', gardenMap.back().size());
    gardenMap.front() = rocks;
    gardenMap.push_back(rocks);

    // search the map
    reachableTiles.insert(startPosition);
    int currentStep = 0;
    
    while (currentStep < desiaredSteps)
    {
        set<pair<long, long>> tilesToSearchInCurrentStep = reachableTiles;
        reachableTiles.clear();

        for (auto iter = tilesToSearchInCurrentStep.begin(); iter != tilesToSearchInCurrentStep.end(); iter++)
        {
            if (gardenMap.at(iter->first - 1).at(iter->second) != '#')
            {
                reachableTiles.insert(make_pair(iter->first - 1, iter->second));
            }

            if (gardenMap.at(iter->first + 1).at(iter->second) != '#')
            {
                reachableTiles.insert(make_pair(iter->first + 1, iter->second));
            }

            if (gardenMap.at(iter->first).at(iter->second - 1) != '#')
            {
                reachableTiles.insert(make_pair(iter->first, iter->second - 1));
            }

            if (gardenMap.at(iter->first).at(iter->second + 1) != '#')
            {
                reachableTiles.insert(make_pair(iter->first, iter->second + 1));
            }         
            
        }

        currentStep++;
    }

   
    std::cout << "reachableTiles: " << reachableTiles.size();// << "    " << "power: " << power << std::endl;
    return 0;
}
