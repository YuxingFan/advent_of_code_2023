// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
using namespace std;

void replaceNumber(string& theString, vector<string>& stringWithNumber, string& first, string& last);
map<string, string> toMap;

int main()
{
    std::ifstream infile("day1.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }
    std::string line;
    unsigned long long total = 0;

    vector<string> numberStrings;       // string to find and replace
    numberStrings.push_back("one");
    numberStrings.push_back("two");
    numberStrings.push_back("three");
    numberStrings.push_back("four");
    numberStrings.push_back("five");
    numberStrings.push_back("six");
    numberStrings.push_back("seven");
    numberStrings.push_back("eight");
    numberStrings.push_back("nine");
    numberStrings.push_back("1");
    numberStrings.push_back("2");
    numberStrings.push_back("3");
    numberStrings.push_back("4");
    numberStrings.push_back("5");
    numberStrings.push_back("6");
    numberStrings.push_back("7");
    numberStrings.push_back("8");
    numberStrings.push_back("9");

    toMap["one"] = "1";
    toMap["two"] = "2";
    toMap["three"] = "3";
    toMap["four"] = "4";
    toMap["five"] = "5";
    toMap["six"] = "6";
    toMap["seven"] = "7";
    toMap["eight"] = "8";
    toMap["nine"] = "9";
    toMap["1"] = "1";
    toMap["2"] = "2";
    toMap["3"] = "3";
    toMap["4"] = "4";
    toMap["5"] = "5";
    toMap["6"] = "6";
    toMap["7"] = "7";
    toMap["8"] = "8";
    toMap["9"] = "9";


    while (std::getline(infile, line))
    {
        string first, last;
        replaceNumber(line, numberStrings, first, last);
        // handle a line;
        /*
        bool foundDigit = false;
        char firstDigit = '0';
        char secondDigit = '0';
        for (size_t index = 0; index < line.size(); index++)
        {
            if (line[index] >= '0' && line[index] <= '9')
            {   
                if (!foundDigit)
                {
                    firstDigit = line[index];
                    secondDigit = line[index];
                    foundDigit = true;
                }
                else
                {
                    secondDigit = line[index];
                }
            }
        }
        */
        total += (first[0] - '0') * 10 + (last[0] - '0');
    }

    std::cout << total << std::endl;
    return 0;
}

void replaceNumber(string& theString, vector<string>& stringWithNumber, string& first, string& last)
{
    
    size_t firstPos = theString.size();
    string firstString = "";
    for (auto iter = stringWithNumber.begin(); iter != stringWithNumber.end(); iter++)
    {
        string toFind = *iter;
        size_t pos = theString.find(toFind);
        if (pos != string::npos && pos < firstPos)
        {
            firstPos = pos;
            firstString = toFind;
        }
    }

    
    size_t lastPos = 0;
    string lastString = "";
    for (auto iter = stringWithNumber.begin(); iter != stringWithNumber.end(); iter++)
    {
        string toFind = *iter;
        size_t pos = theString.rfind(toFind);
        if (pos != string::npos && pos >= lastPos)
        {
            lastPos = pos;
            lastString = toFind;
        }
    }

    first = toMap[firstString];
    last = toMap[lastString];

}