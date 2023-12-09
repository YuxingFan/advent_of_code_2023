#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';

bool isDigit(char c);
void getNumberFromFirst(string& currentString, size_t index, vector<string>& container);
void getNumberFromTail(string& currentString, size_t index, vector<string>& container);
void getNumberFromMiddle(string& currentString, size_t index, vector<string>& container);

int main()
{
    // read file
    std::ifstream infile("day3.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }
    
    vector<string> strings;
    strings.push_back(string());

    std::string line;
    // get each line of the file
    while (std::getline(infile, line))
    {
        string theString(".");
        theString.append(line);
        theString.append(".");
        strings.push_back(theString);
    }

    size_t stringSize = strings.back().size();
    string border = string(stringSize, '.');
    strings.at(0) = border;
    strings.push_back(border);

    unsigned long long totalPart = 0;
    // check each data line except the first and last line
    for (auto iter = strings.begin() + 1; iter != strings.end() - 1; iter++)
    {
        string lastString = *(iter - 1);
        string currentString = *iter;
        string nextString = *(iter + 1);

        // part two
        //************************************
        for (size_t index = 0; index < currentString.size(); index++)
        {
            char currentChar = currentString[index];
            if (currentChar == '*')
            {
                vector<string> allNumbers;
                getNumberFromFirst(currentString, index + 1, allNumbers);
                getNumberFromTail(currentString, index - 1, allNumbers);
                if (isDigit(lastString[index]))
                {
                    getNumberFromMiddle(lastString, index, allNumbers);
                }
                else
                {
                    getNumberFromFirst(lastString, index + 1, allNumbers);
                    getNumberFromTail(lastString, index - 1, allNumbers);
                }

                if (isDigit(nextString[index]))
                {
                    getNumberFromMiddle(nextString, index, allNumbers);
                }
                else
                {
                    getNumberFromFirst(nextString, index + 1, allNumbers);
                    getNumberFromTail(nextString, index - 1, allNumbers);
                }
                
                if (allNumbers.size() == 2)
                {
                    totalPart += stoll(allNumbers.front())* stoll(allNumbers.back());
                }
            }
        }
        //************************************
        
        // part one
        //************************************
        /*
        string numberString = "";
        size_t startPos = 0;
        for (size_t index = 0; index < currentString.size(); index++)
        {
            char currentChar = currentString[index];
            // if it is digit, push it to the string
            if (currentChar >= '0' && currentChar <= '9')
            {
                numberString.push_back(currentChar);
            }
            // otherwise, process the numberString
            else
            {
                // If there is a numberString, get the string for check
                if (!numberString.empty())
                {
                    // position of first digit;
                    size_t pos = currentString.find(numberString, startPos);
                    // get chars around the numberString
                    string stringToCheck;
                    stringToCheck.append(lastString.substr(pos - 1, numberString.size() + 2));
                    stringToCheck.append(nextString.substr(pos - 1, numberString.size() + 2));
                    stringToCheck.append(1, currentString[pos - 1]);
                    stringToCheck.append(1, currentString[pos + numberString.size()]);

                    bool isValidNumber = false;
                    for (size_t i = 0; i < stringToCheck.size(); i++)
                    {
                        if (stringToCheck[i] >= '0' && stringToCheck[i] <= '9')
                        {
                            continue;
                            
                        }
                        else if (stringToCheck[i] != '.')
                        {
                            isValidNumber = true;
                            break;
                        }
              
                    }

                    if (isValidNumber)
                    {
                        totalPart += stol(numberString);
                    }

                    // update variables
                    startPos = pos + numberString.size();
                    numberString.clear();
                   
                }
            }
        }
        */
        //************************************
    }
   

    std::cout << "totalPart: " << totalPart << std::endl;
    
    return 0;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

void getNumberFromFirst(string& currentString, size_t index, vector<string>& container)
{
    string myString = "";
    for (size_t i = index; i < currentString.size(); i++)
    {
        if (isDigit(currentString[i]))
        {
            myString.push_back(currentString[i]);
        }
        else
        {
            break;
        }
    }

    if (!myString.empty())
    {
        container.push_back(myString);
    }
}

void getNumberFromTail(string& currentString, size_t index, vector<string>& container)
{
    string myString = "";
    for (int i = index; i >=0; i--)
    {
        if (isDigit(currentString[i]))
        {
            myString.push_back(currentString[i]);
        }
        else
        {
            break;
        }
    }

    if (!myString.empty())
    {
        std::reverse(myString.begin(), myString.end());
        container.push_back(myString);
    }
}

void getNumberFromMiddle(string& currentString, size_t index, vector<string>& container)
{
    vector<string> theString;
    getNumberFromTail(currentString, index - 1, theString);
    string s = string(1, currentString[index]);
    theString.push_back(s);
    getNumberFromFirst(currentString, index + 1, theString);

    string ret = "";
    for (auto iter = theString.begin(); iter != theString.end(); iter++)
    {
        ret.append(*iter);
    }

    if (!ret.empty())
    {
        container.push_back(ret);
    }
}