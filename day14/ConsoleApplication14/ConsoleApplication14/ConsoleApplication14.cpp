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

int main()
{
    // read file
    std::ifstream infile("day14.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // read all lines
    vector<string> rocks;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            rocks.push_back(line);
        }
    }

    long long numOfRows = rocks.size();
    long long numOfColumns = rocks.front().size();
    long long maxLoad = numOfRows;

    // the vector used to store the total load of each column
    vector<long long> totalLoadOfColumn(numOfColumns, 0);
    // the load for next 'O'
    vector<long long> nextLoadOfColumn(numOfColumns, maxLoad);

    // iterate rows
    for (auto iter = rocks.begin(); iter != rocks.end(); iter++)
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
                nextLoadOfColumn.at(index) = maxLoad - (iter - rocks.begin()) - 1;
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

