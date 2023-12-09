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

std::map<string, long> bidMap;

std::vector<string> fiveOfAKindv;
std::vector<string> fourOfAKindv;
std::vector<string> fullHousev;
std::vector<string> threeOfAKindv;
std::vector<string> twoPairv;
std::vector<string> onePairv;
std::vector<string> highCardv;

enum cardType
{
    fiveOfAKind,
    fourOfAKind,
    fullHouse,
    threeOfAKind,
    twoPair,
    onePair,
    highCard
};

enum cardType getCardType(string& cardString, bool isPartOne);
void replaceString(string& cardString, bool isPartOne);
long long getScore(std::vector<string>& cards, long long& initRank);

int main()
{
    // read file
    std::ifstream infile("day7.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    std::string line;
    while (std::getline(infile, line))
    {
        std::stringstream lineStream(line);
        std::string cardString;
        std::string bidString;
        std::getline(lineStream, cardString, spaceDelimiter);
        std::getline(lineStream, bidString, spaceDelimiter);  // "79 14 55 13"
        // true for part one, false for part two
        replaceString(cardString, false);
        switch (getCardType(cardString, false))
        {
        case cardType::fiveOfAKind:
            fiveOfAKindv.push_back(cardString);
            break;
        case cardType::fourOfAKind:
            fourOfAKindv.push_back(cardString);
            break;
        case cardType::fullHouse:
            fullHousev.push_back(cardString);
            break;
        case cardType::threeOfAKind:
            threeOfAKindv.push_back(cardString);
            break;
        case cardType::twoPair:
            twoPairv.push_back(cardString);
            break;
        case cardType::onePair:
            onePairv.push_back(cardString);
            break;
        case cardType::highCard:
            highCardv.push_back(cardString);
            break;
        }

        bidMap[cardString] = stol(bidString);
    }

    std::sort(fiveOfAKindv.begin(), fiveOfAKindv.end());
    std::sort(fourOfAKindv.begin(), fourOfAKindv.end());
    std::sort(fullHousev.begin(), fullHousev.end());
    std::sort(threeOfAKindv.begin(), threeOfAKindv.end());
    std::sort(twoPairv.begin(), twoPairv.end());
    std::sort(onePairv.begin(), onePairv.end());
    std::sort(highCardv.begin(), highCardv.end());
   
    long long totalScore = 0;
    long long initRank = bidMap.size();

    totalScore += getScore(fiveOfAKindv, initRank);
    totalScore += getScore(fourOfAKindv, initRank);
    totalScore += getScore(fullHousev, initRank);
    totalScore += getScore(threeOfAKindv, initRank);
    totalScore += getScore(twoPairv, initRank);
    totalScore += getScore(onePairv, initRank);
    totalScore += getScore(highCardv, initRank);
    
    std::cout << "totalScore: " << totalScore << std::endl;
    return 0;
}

long long getScore(std::vector<string>& cards, long long& initRank)
{
    long long ret = 0;
    for (auto iter = cards.rbegin(); iter != cards.rend(); iter++)
    {
        ret += bidMap[*iter] * initRank;
        initRank--;
    }

    return ret;
}

void replaceString(string& cardString, bool isPartOne)
{
    while (cardString.find("T") != string::npos)
    {
        cardString.replace(cardString.find("T"), 1, "a");
    }

    if (isPartOne)
    {
        while (cardString.find("J") != string::npos)
        {
            cardString.replace(cardString.find("J"), 1, "b");
        }
    }
    else
    {
        while (cardString.find("J") != string::npos)
        {
            cardString.replace(cardString.find("J"), 1, "1");
        }
    }
    

    while (cardString.find("Q") != string::npos)
    {
        cardString.replace(cardString.find("Q"), 1, "c");
    }

    while (cardString.find("K") != string::npos)
    {
        cardString.replace(cardString.find("K"), 1, "d");
    }

    while (cardString.find("A") != string::npos)
    {
        cardString.replace(cardString.find("A"), 1, "e");
    }
}


enum cardType getCardType(string& cardString, bool isPartOne)
{
    enum cardType ret;
    map<char, int> counter;
    int maxTime = 1;
    for (auto iter = cardString.begin(); iter != cardString.end(); iter++)
    {
        if (counter.find(*iter) != counter.end())
        {
            counter[*iter]++;
            if (isPartOne && counter[*iter] > maxTime)
            {
                maxTime = counter[*iter];
            }
            else if (!isPartOne && (*iter != '1') && counter[*iter] > maxTime)
            {
                maxTime = counter[*iter];
            }
        }
        else
        {
            counter[*iter] = 1;
        }
    }

    size_t counterSize = counter.size();

    // This part is only needed for part two and there are 'J's in the card string (note the 'J' is replaced with '1')
    if (!isPartOne && counter.find('1') != counter.end())
    {
        if (counter['1'] == 5)
        {
            return cardType::fiveOfAKind;
        }
        else
        {
            counterSize--;
            maxTime += counter['1'];
        }    
    }

    if (counterSize == 1)
    {
        return cardType::fiveOfAKind;
    }
    else if (counterSize == 2 && maxTime == 4)
    {
        return cardType::fourOfAKind;
    }
    else if (counterSize == 2 && maxTime == 3)
    {
        return cardType::fullHouse;
    }
    else if (counterSize == 3 && maxTime == 3)
    {
        return cardType::threeOfAKind;
    }
    else if (counterSize == 3 && maxTime == 2)
    {
        return cardType::twoPair;
    }
    else if (counterSize == 4)
    {
        return cardType::onePair;
    }
    else if (counterSize == 5)
    {
        return cardType::highCard;
    }
}