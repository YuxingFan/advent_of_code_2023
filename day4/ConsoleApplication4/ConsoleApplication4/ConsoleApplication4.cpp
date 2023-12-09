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
static const char verticalDelimiter = '|';

std::map<int, int> numberOfMatches;                 // the number of matched numbers in each card
std::map<int, int> numberOfCards;                   // the number of each card

void init();
void calculateMatchedNumberInLine(std::string& numberString, int round);
unsigned long long calculateTotalNumberOfCards();
unsigned long long calculateTotalNumberOfPoints();

int main()
{
    init();
    // read file
    std::ifstream infile("day4.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    std::string line;
    int cardRound = 1;

    // get each line of the file
    while (std::getline(infile, line))
    {
        // a line is like "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53"
        std::stringstream stringString(line);
        std::string roundString;                            // to calculate round => "Game 1"
        std::string numberString;                             // to check cube => " 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
        std::getline(stringString, roundString, delimiter);
        std::getline(stringString, numberString, delimiter);
        
        calculateMatchedNumberInLine(numberString, cardRound);
        cardRound++;
 
    }
    unsigned long long totalPoint = calculateTotalNumberOfPoints();                          // part one final result
    unsigned long long totalCards = calculateTotalNumberOfCards();   // part two final result
    std::cout << "totalPoing: " << totalPoint << std::endl << "totalCards: " << totalCards << std::endl;
    return 0;
}

void init()
{
    // do nothing
}

unsigned long long calculateTotalNumberOfCards()
{
    // At the beginning, each card has only one piece
    for(auto iter = numberOfMatches.begin(); iter != numberOfMatches.end(); iter++)
    {
        numberOfCards[iter->first] = 1;
    }

    // Increase the number of cards according to the matched number
    for (auto iter = numberOfMatches.begin(); iter != numberOfMatches.end(); iter++)
    {
        int roundNumber = iter->first;
        int numberOfMatched = iter->second;
        for (int index = roundNumber + 1; index <= roundNumber + numberOfMatched; index++)
        {
            numberOfCards[index] += numberOfCards[roundNumber];
        }
    }

    unsigned long long totalNumberOfCard = 0;
    for (auto iter = numberOfCards.begin(); iter != numberOfCards.end(); iter++)
    {
        totalNumberOfCard += iter->second;
    }

    return totalNumberOfCard;
}


unsigned long long calculateTotalNumberOfPoints()
{
    unsigned long long points = 0;
    for (auto iter = numberOfMatches.begin(); iter != numberOfMatches.end(); iter++)
    {
        if (iter->second != 0)
        {
            points += pow(2, iter->second - 1);
        }
    }

    return points;
}


void calculateMatchedNumberInLine(std::string& numberString, int round)
{
   
    // cube string is like " 41 48 83 86 17 | 83 86  6 31 17  9 48 53"
    std::stringstream ss(numberString);
    string winnerNumberList;
    string myNumberList;
    getline(ss, winnerNumberList, verticalDelimiter); //" 41 48 83 86 17 "
    getline(ss, myNumberList, verticalDelimiter);    // " 83 86  6 31 17  9 48 53"

    std::stringstream winnerNumberStream(winnerNumberList);
    std::stringstream myNumberSteam(myNumberList);
    std::string number;
    
    // get numbers from winner list
    map<int, int> winnerNumbers;
    while (getline(winnerNumberStream, number, spaceDelimiter))
    {
        if (!number.empty())
        {
            int keyValue = stoi(number);
            winnerNumbers[keyValue] = keyValue;
        }
    }

    // get numbers from candidate list
    map<int, int> myNumbers;
    while (getline(myNumberSteam, number, spaceDelimiter))
    {
        if (!number.empty())
        {
            int keyValue = stoi(number);
            myNumbers[keyValue] = keyValue;
        }    
    }
    
    // calculate number of matched numbers
    unsigned int numberOfMatched = 0;
    for (auto iter = myNumbers.begin(); iter != myNumbers.end(); iter++)
    {
        if (winnerNumbers.find(iter->first) != winnerNumbers.end())
        {
            numberOfMatched++;
        }
    }

    // store the number
    numberOfMatches[round] = numberOfMatched; 
}
