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

// Get all numbers listed in the string
vector<long long> getNumbersFromString(string& line);

// Calculate the front history result and back history result from the given number sequence
void calculateHistory(vector<long long>& numbers, long long& frontResult, long long& backResult);

// calculate the new sequence from the old on
// the allZeroValue value is used to check if all-zero sequence has been found or not
vector<long long> findNewSequence(vector<long long>& oldSequence, bool& allZeroValue);

int main()
{
    // read file
    std::ifstream infile("day9.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // final result
    long long sumOfFrontHistory = 0;                
    long long sumOfBackHistory = 0;

    std::string line;
    while (std::getline(infile, line))
    {
        // get the number sequence in this line
        vector<long long> numbers = getNumbersFromString(line);
        // result of this line
        long long frontHistory = 0;
        long long backHistory = 0;
        calculateHistory(numbers, frontHistory, backHistory);

        // add to the final result
        sumOfFrontHistory += frontHistory;
        sumOfBackHistory += backHistory;
    }

    std::cout << "sumOfFrontHistory: " << sumOfFrontHistory << "    " << "sumOfBackHistory: " << sumOfBackHistory << std::endl;
    return 0;
}

vector<long long> getNumbersFromString(string& line)
{
    // get all numbers from the line
    vector<long long> numbers;

    std::stringstream theStream(line);
    string number;
    while (std::getline(theStream, number, spaceDelimiter))
    {
        numbers.push_back(stoll(number));
    }

    return numbers;
}


void calculateHistory(vector<long long>& numbers, long long& frontResult, long long& backResult)
{
    // init the result
    backResult = 0;
    frontResult = 0;

    // the calculated new sequence, it is initialized with the first number sequence
    vector<long long> newSequence = numbers;
    // the boolean to determine if new sequence should be calculated. the process should stop when the all-zero sequence has been found
    bool stopFindNewSequence = false;
    // the list used to calculate frontResult
    vector<long long> frontHistoryList;
    
    // The loop will stop when all-zero sequence has been found
    while (!stopFindNewSequence)    
    {
        // Add the last number to backResult;
        backResult += newSequence.back();
        // Store the first number to frontHistoryList
        frontHistoryList.push_back(newSequence.front());
        // Get the next sequence
        newSequence = findNewSequence(newSequence, stopFindNewSequence);
    }
    
    // calculate the value of frontHistory
    for (auto iter = frontHistoryList.rbegin(); iter != frontHistoryList.rend(); iter++)
    {
        frontResult = *iter - frontResult;
    }

}

vector<long long> findNewSequence(vector<long long>& oldSequence, bool& allZeroValue)
{
    // return value
    vector<long long> newSequence;

    // boolean to check if all-zero sequence has been found or not
    allZeroValue = true;
    for (auto iter = oldSequence.begin(); iter != oldSequence.end() - 1; iter++)
    {
        // calculate and store the new value
        long long diff = *(iter + 1) - *(iter);
        newSequence.push_back(diff);
        // check if the value is 0 or not
        if (allZeroValue && diff != 0)
        {
            allZeroValue = false;
        }
    }

    return newSequence;
}
