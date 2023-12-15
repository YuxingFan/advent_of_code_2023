#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include<algorithm>
using namespace std;


// This function calculate the HASH result given a string
long long getHashResult(std::string& step);

// This function manipulate the boxes according to the step
void manipulateBox(std::string& step);

// Remove len from box
void removeLen(long long boxNumber, std::string& label);

// Refresh lens in box
void refreshLen(long long boxNumber, std::string& label, long long focalLength);

// Calculate focusing power
long long getFocusingPower();

// <box number, list of <label, focal length>>
std::map<long long, std::vector<std::pair<std::string, long long>>> lensInBoxMap;

int main()
{
    // read file
    std::ifstream infile("day15.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // read input
    std::string sequence;               
    std::getline(infile, sequence);
    sequence.push_back(',');

    // initialize
    long long totalValue = 0;           // The result for part one
    std::string step = "";
    for (auto iter = sequence.begin(); iter != sequence.end(); iter++)
    {
        if (*iter == ',')
        {
            totalValue += getHashResult(step);
            manipulateBox(step);
            step.clear();
        }
        else
        {
            step.push_back(*iter);
        }
    }

    long long power = getFocusingPower();
    std::cout << "totalValue: " << totalValue << "    " << "power: " << power << std::endl;
    return 0;
}

long long getHashResult(std::string& step)
{
    long long currentValue = 0;
    for (auto iter = step.begin(); iter != step.end(); iter++)
    {
        int8_t value = *iter;
        currentValue += value;
        currentValue *= 17;
        currentValue %= 256;
    }
    return currentValue;
}

void manipulateBox(std::string& step)
{
    std::string label;
    bool remove;
    long long focalLength;
    // the '-' operation
    if (step.find('-') != string::npos)
    {
        label = step.substr(0, step.size() - 1);
        remove = true;
    }
    // the '=' operation
    else
    {
        label = step.substr(0, step.size() - 2);
        focalLength = stoi(step.substr(step.size() - 1, 1));
        remove = false;
    }

    // get box number
    long long boxNumber = getHashResult(label);

    remove ? removeLen(boxNumber, label) : refreshLen(boxNumber, label, focalLength);
}


void removeLen(long long boxNumber, std::string& label)
{
    // If the box has lens
    if (lensInBoxMap.find(boxNumber) != lensInBoxMap.end())
    {
        auto& lens = lensInBoxMap[boxNumber];
        for (auto iter = lens.begin(); iter != lens.end(); iter++)
        {
            if (iter->first.compare(label) == 0)
            {
                lens.erase(iter);
                break;
            }
        }
    }
}

void refreshLen(long long boxNumber, std::string& label, long long focalLength)
{
    // If the box has lens
    if (lensInBoxMap.find(boxNumber) != lensInBoxMap.end())
    {
        auto& lens = lensInBoxMap[boxNumber];
        bool replaced = false;
        for (auto iter = lens.begin(); iter != lens.end(); iter++)
        {
            if ((iter->first).compare(label) == 0)
            {
                iter->second = focalLength;
                replaced = true;
                break;
            }
        }

        if (!replaced)
        {
            lens.push_back(std::make_pair(label, focalLength));
        }
    }
    else
    {
        std::vector<std::pair<std::string, long long>> lens;
        lens.push_back(std::make_pair(label, focalLength));
        lensInBoxMap[boxNumber] = lens;
    }
}

long long getFocusingPower()
{
    long long totalPower = 0;
    for (auto iterBox = lensInBoxMap.begin(); iterBox != lensInBoxMap.end(); iterBox++)
    {
        long long boxNumberFactor = iterBox->first + 1;
        for (auto iterLen = iterBox->second.begin(); iterLen != iterBox->second.end(); iterLen++)
        {
            long long slot = iterLen - iterBox->second.begin() + 1;
            long long focalLength = iterLen->second;

            totalPower += boxNumberFactor * slot * focalLength;
        }
    }

    return totalPower;
}