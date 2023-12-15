#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include <set>
#include<algorithm>
using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';
static const char verticalDelimiter = '|';

//part one
// get the note of one pattern
long long getNote(vector<string>& mirrors);
// check if two adjacent rows or colomns are the same or not
void checkAdjacentRowAndColumn(std::vector<std::string>& mirrors, std::set<std::pair<long, long>>& possibleVerticalLines, std::set<std::pair<long, long>>& possibleHorizontalLines);
// initialization
void initialize(std::vector<std::string>& mirrors, std::set<std::pair<long, long>>& possibleVerticalLines, std::set<std::pair<long, long>>& possibleHorizontalLines);

//part two
// get the note of one pattern
long long getNewNote(vector<string>& mirrors);
// check if two adjacent rows or colomns are the same or not
void newCheckAdjacentRowAndColumn(std::vector<std::string>& mirrors, std::map<std::pair<long, long>, long>& possibleVerticalLines, std::map<std::pair<long, long>, long>& possibleHorizontalLines);
// initialization
void newInitialize(std::vector<std::string>& mirrors, std::map<std::pair<long, long>, long>& possibleVerticalLines, std::map<std::pair<long, long>, long>& possibleHorizontalLines);


int main()
{
    // read file
    std::ifstream infile("day13.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    long long totalNotes = 0;
    // read all lines
    vector<string> Mirrors;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            Mirrors.push_back(line);
        }
        else
        {
            //totalNotes += getNote(Mirrors);
            totalNotes += getNewNote(Mirrors);
            Mirrors.clear();
        }
    }
    
    std::cout << "totalNotes: " << totalNotes;// << "    " << "sumOfBackHistory: " << sumOfBackHistory << std::endl;
    return 0;
}

long long getNote(vector<string>& mirrors)
{
    // initialize sets for check
    set<std::pair<long, long>> possibleVerticalLines;
    set<std::pair<long, long>> possibleHorizontalLines;
    initialize(mirrors, possibleVerticalLines, possibleHorizontalLines);

    // check if two adjacent rows or colomns are the same or not
    checkAdjacentRowAndColumn(mirrors, possibleVerticalLines, possibleHorizontalLines);

    /*verify result*/
    //verify possible the pair rows
    for (auto iter = possibleHorizontalLines.begin(); iter != possibleHorizontalLines.end(); iter++)
    {
        auto up = mirrors.rend() - iter->first - 1;
        auto down = mirrors.begin() + iter->second;
        bool found = true;

        do 
        {
            if (up->compare(*down) != 0)
            {
                found = false;
                break;
            }
            up++;
            down++;
        } while (found && (down != mirrors.end()) && (up != mirrors.rend()));

        if(found)
        {
            return (iter->first + 1) * 100;
        }
    }

    // verify possible the pair columns
    for (auto iter = possibleVerticalLines.begin(); iter != possibleVerticalLines.end(); iter++)
    {
        bool found = true;
        for (auto iterMirror = mirrors.begin(); found && iterMirror != mirrors.end(); iterMirror++)
        {
            auto left = iterMirror->rend() - iter->first - 1;
            auto right = iterMirror->begin() + iter->second;
            do
            {
                if (*left != *right)
                {
                    found = false;
                    break;
                }
                left++;
                right++;
            } while (found && right != iterMirror->end() && (left != iterMirror->rend()));
        }

        if (found)
        {
            return iter->first + 1;
        }
    }
    
    // this should never happen
    std::cout << "reflection not found!" << endl;
    return 0;
}


void checkAdjacentRowAndColumn(std::vector<std::string>& mirrors, std::set<std::pair<long, long>>& possibleVerticalLines, std::set<std::pair<long, long>>& possibleHorizontalLines)
{
    auto lastRow = mirrors.begin();
    // iterate rows
    for (auto iterRow = mirrors.begin(); iterRow != mirrors.end(); iterRow++)
    {
        // store the adjacent columns that are not the same
        vector<std::pair<long, long>> toBeRemoved;
        for (auto iterVer = possibleVerticalLines.begin(); iterVer != possibleVerticalLines.end(); iterVer++)
        {
            // check if adjacent columns are the same
            if (iterRow->at(iterVer->first) != iterRow->at(iterVer->second))
            {
                toBeRemoved.push_back(*iterVer);
            }
        }

        // remove the adjacent columns that are not the same
        for (auto iter = toBeRemoved.begin(); iter != toBeRemoved.end(); iter++)
        {
            possibleVerticalLines.erase(*iter);
        }

        // remove the adjacent rows that are not the same
        if (iterRow->compare(*lastRow) != 0)
        {
            possibleHorizontalLines.erase(std::make_pair(lastRow - mirrors.begin(), iterRow - mirrors.begin()));
        }
        // update lastRow with current row
        lastRow = iterRow;
    }
}

void initialize(std::vector<std::string>& mirrors, std::set<std::pair<long, long>>& possibleVerticalLines, std::set<std::pair<long, long>>& possibleHorizontalLines)
{
    long long numOfColumns = mirrors.front().size();
    long long numOfRows = mirrors.size();

    // initialize the two sets for check
    for (size_t i = 0; i < numOfColumns - 1; i++)
    {
        possibleVerticalLines.insert(std::make_pair(i, i + 1));
    }
    for (size_t i = 0; i < numOfRows - 1; i++)
    {
        possibleHorizontalLines.insert(std::make_pair(i, i + 1));
    }
}


long long getNewNote(vector<string>& mirrors)
{
    // initialize sets for check
    map<std::pair<long, long>, long> possibleVerticalLines;
    map<std::pair<long, long>, long> possibleHorizontalLines;
    newInitialize(mirrors, possibleVerticalLines, possibleHorizontalLines);

    newCheckAdjacentRowAndColumn(mirrors, possibleVerticalLines, possibleHorizontalLines);

    /*verify result*/
    //verify possible the pair rows
    for (auto iter = possibleHorizontalLines.begin(); iter != possibleHorizontalLines.end(); iter++)
    {
        auto up = mirrors.rend() - iter->first.first - 1;
        auto down = mirrors.begin() + iter->first.second;

        long long numOfDifferent = 0;
        do
        {
            if (up->compare(*down) != 0)
            {
                for (size_t index = 0; index < up->size(); index++)
                {
                    // get the number of different chars
                    if (up->at(index) != down->at(index))
                    {
                        numOfDifferent++;
                    }
                }
            }
            up++;
            down++;
        } while ((down != mirrors.end()) && (up != mirrors.rend()));

        long long numOfCol = mirrors.front().size();
        bool found = (numOfDifferent == 1);
        if (found)
        {
            return (iter->first.first + 1) * 100;
        }
    }

    // verify possible the pair columns
    for (auto iter = possibleVerticalLines.begin(); iter != possibleVerticalLines.end(); iter++)
    {
        long long numOfDifferent = 0;
        for (auto iterMirror = mirrors.begin(); iterMirror != mirrors.end(); iterMirror++)
        {
            auto left = iterMirror->rend() - iter->first.first - 1;
            auto right = iterMirror->begin() + iter->first.second;
            
            do
            {
                // get the number of different chars
                if (*left != *right)
                {
                    numOfDifferent++;
                }
                left++;
                right++;
            } while (right != iterMirror->end() && (left != iterMirror->rend()));
        }

        long long numOfRow = mirrors.size();
        bool found = (numOfDifferent == 1);
        if (found)
        {
            return iter->first.first + 1;
        }
    }

    // this should never happen
    std::cout << "reflection not found!" << endl;

    return 0;
}

void newInitialize(std::vector<std::string>& mirrors, std::map<std::pair<long, long>, long>& possibleVerticalLines, std::map<std::pair<long, long>, long>& possibleHorizontalLines)
{
    long long numOfColumns = mirrors.front().size();
    long long numOfRows = mirrors.size();

    // initialize the two sets for check
    for (size_t i = 0; i < numOfColumns - 1; i++)
    {
        possibleVerticalLines[std::make_pair(i, i + 1)] = 0;
    }
    for (size_t i = 0; i < numOfRows - 1; i++)
    {
        possibleHorizontalLines[std::make_pair(i, i + 1)] = 0;
    }
}

void newCheckAdjacentRowAndColumn(std::vector<std::string>& mirrors, std::map<std::pair<long, long>, long>& possibleVerticalLines, std::map<std::pair<long, long>, long>& possibleHorizontalLines)
{
    long long numOfRows = mirrors.size();
    long long numOfCols = mirrors.front().size();
    // iterate rows
    for (auto iterRow = mirrors.begin(); iterRow != mirrors.end(); iterRow++)
    {
        for (auto iterVer = iterRow->begin(); iterVer != iterRow->end() - 1; iterVer++)
        {
            // check if adjacent columns are the same
            if (*iterVer == *(iterVer + 1))
            {
                possibleVerticalLines[make_pair(iterVer - iterRow->begin(), iterVer - iterRow->begin() + 1)]++;
            }
        }
    }

    // iterate rows
    for (auto iterRow = mirrors.begin(); iterRow != mirrors.end() - 1; iterRow++)
    {
        for (size_t index = 0; index < iterRow->size(); index++)
        {
            // check if adjacent columns are the same
            if (iterRow->at(index) == (iterRow + 1)->at(index))
            {
                possibleHorizontalLines[make_pair(iterRow - mirrors.begin(), iterRow - mirrors.begin() + 1)]++;
            }
        }

    }

    // only need the pair that matchs or only has one dismatch
    vector<std::pair<long, long>> toBeRemoved;
    for (auto iter = possibleVerticalLines.begin(); iter != possibleVerticalLines.end(); iter++)
    {
        if (!(iter->second == numOfRows - 1 || iter->second == numOfRows))
        {
            toBeRemoved.push_back(iter->first);
        }
    }
    for (auto iter = toBeRemoved.begin(); iter != toBeRemoved.end(); iter++)
    {
        possibleVerticalLines.erase(*iter);
    }
    toBeRemoved.clear();

    for (auto iter = possibleHorizontalLines.begin(); iter != possibleHorizontalLines.end(); iter++)
    {
        if (!(iter->second == numOfCols - 1 || iter->second == numOfCols))
        {
            toBeRemoved.push_back(iter->first);
        }
    }
    for (auto iter = toBeRemoved.begin(); iter != toBeRemoved.end(); iter++)
    {
        possibleHorizontalLines.erase(*iter);
    }
}