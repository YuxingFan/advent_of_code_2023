#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';
static const char verticalDelimiter = '|';

std::vector<long long> seeds;
std::vector<std::pair<long long, long long>> seedPair;
std::map<string, vector<std::pair<std::pair<long long, long long>, long long>>> infoMap;

// part one
void getSeed(string& numberString);
std::pair<std::pair<long long, long long>, long long> getThreeNumbers(string& line);
long long getLocation(long long seed);
long long getDestination(long long source, vector<std::pair<std::pair<long long, long long>, long long>>& mapping);

// part two
void getSeedPair();
std::vector<std::pair<long long, long long>> getLocationPair();
std::vector<std::pair<long long, long long>> getDestinationPair(std::vector<std::pair<long long, long long>>& input, vector<std::pair<std::pair<long long, long long>, long long>>& theMap);

int main()
{
    // read file
    std::ifstream infile("day5.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    std::string line;
    std::getline(infile, line);     //get the seed line line "seeds: 79 14 55 13"

    // "seeds: 79 14 55 13"
    std::stringstream stringString(line);
    std::string numberString;                           
    std::getline(stringString, numberString, delimiter);
    std::getline(stringString, numberString, delimiter);  // "79 14 55 13"
    
    string mapName = "";
    vector<std::pair<std::pair<long long, long long>, long long>> dataOfMap;
    // get each line of the file
    while (std::getline(infile, line))
    {
        // If get the empty line
        if (line.empty())
        {
            // If have collected data for a map
            if (!mapName.empty())
            {
                // store the data to a map
                infoMap[mapName] = dataOfMap;
                // clear vector to collect new data
                dataOfMap.clear();
            }
            std::getline(infile, line);
            if (line.find("seed-to-soil") != string::npos)
            {
                mapName = "seedToSoil";
            }
            else if (line.find("soil-to-fertilizer") != string::npos)
            {
                mapName = "soilToFertilizer";
            }
            else if (line.find("fertilizer-to-water") != string::npos)
            {
                mapName = "fertilizerToWater";
            }
            else if (line.find("water-to-light") != string::npos)
            {
                mapName = "waterToLight";
            }
            else if (line.find("light-to-temperature") != string::npos)
            {
                mapName = "lightToTemperature";
            }
            else if (line.find("temperature-to-humidity") != string::npos)
            {
                mapName = "temperatureToHumidity";
            }
            else if (line.find("humidity-to-location") != string::npos)
            {
                mapName = "humidityToLocation";
            }
            continue;
        }
        else
        {
            std::pair<std::pair<long long, long long>, long long> thePairInLine = getThreeNumbers(line);
            dataOfMap.push_back(thePairInLine);
        }  
    }
   
    // store the last data to a map
    infoMap[mapName] = dataOfMap;
    getSeed(numberString);
    long long minLocation = std::numeric_limits<long long>::max();

    // part one
    /*
    auto iter = seeds.begin();
    while (iter != seeds.end())
    {
        long long start = *iter;
        iter++;
        long long end = *iter;
        iter++;
        for (long long index = 0; index < end; index++)
        {
            long long currentLocation = getLocation(start + index);
            if (currentLocation < minLocation)
            {
                minLocation = currentLocation;
            }
        }
    }
    */

    // part two
    getSeedPair();
    std::vector<std::pair<long long, long long>> result = getLocationPair();
    for (auto iter = result.begin(); iter != result.end(); iter++)
    {
        if (iter->second > 0 && iter->first < minLocation)
        {
            minLocation = iter->first;
        }
    }

    std::cout << "minLocation: " << minLocation << std::endl;// << "totalCards: " << totalCards << std::endl;
    return 0;
}


void getSeed(string& numberString)
{
    std::stringstream numberStream(numberString);
    std::string number;

    std::vector<long long> numbers;
    // get numbers 
    while (getline(numberStream, number, spaceDelimiter))
    {
        if (!number.empty())
        {
            long long integer = stoll(number);
            numbers.push_back(integer);
        }
    }
    
    seeds = numbers;
}

void getSeedPair()
{
    auto iter = seeds.begin();
    while (iter != seeds.end())
    {
        std::pair<long long, long long> aPair = make_pair(*iter, *(iter + 1));
        seedPair.push_back(aPair);
        iter = iter + 2;
    }
}


std::pair<std::pair<long long, long long>, long long> getThreeNumbers(string& line)
{
    std::stringstream numberStream(line);
    std::string number;
    vector<long long> numbers;
    // get numbers 
    while (getline(numberStream, number, spaceDelimiter))
    {
        if (!number.empty())
        {
            long long integer = stoll(number);
            numbers.push_back(integer);
        }
    }

    std::pair<long long, long long> firstPair = make_pair(numbers.at(1), numbers.at(2));
    std::pair<std::pair<long long, long long>, long long> ret = make_pair(firstPair, numbers.at(0));

    return ret;
}

long long getLocation(long long seed)
{
    long long destination = seed;
    destination = getDestination(destination, infoMap["seedToSoil"]);
    destination = getDestination(destination, infoMap["soilToFertilizer"]);
    destination = getDestination(destination, infoMap["fertilizerToWater"]);
    destination = getDestination(destination, infoMap["waterToLight"]);
    destination = getDestination(destination, infoMap["lightToTemperature"]);
    destination = getDestination(destination, infoMap["temperatureToHumidity"]);
    destination = getDestination(destination, infoMap["humidityToLocation"]);

    return destination;
}

long long getDestination(long long source, vector<std::pair<std::pair<long long, long long>, long long>>& mapping)
{
    long long destination = source;

    for (auto iter = mapping.begin(); iter != mapping.end(); iter++)
    {
        long long start = (iter->first).first;
        long long end = (iter->first).first + (iter->first).second;
        if (source >= start && source < end)
        {
            destination = source - start + iter->second;
            break;
        }
    }

    return destination;
}


std::vector<std::pair<long long, long long>> getLocationPair()
{
    std::vector<std::pair<long long, long long>> result = seedPair;

    result = getDestinationPair(result, infoMap["seedToSoil"]);
    result = getDestinationPair(result, infoMap["soilToFertilizer"]);
    result = getDestinationPair(result, infoMap["fertilizerToWater"]);
    result = getDestinationPair(result, infoMap["waterToLight"]);
    result = getDestinationPair(result, infoMap["lightToTemperature"]);
    result = getDestinationPair(result, infoMap["temperatureToHumidity"]);
    result = getDestinationPair(result, infoMap["humidityToLocation"]);

    return result;
}


std::vector<std::pair<long long, long long>> getDestinationPair(std::vector<std::pair<long long, long long>>& input, vector<std::pair<std::pair<long long, long long>, long long>>& theMap)
{
    std::vector<std::pair<long long, long long>> result;
    for (auto iter = input.begin(); iter != input.end(); iter++)
    {
        long long inputBegin = iter->first;
        long long inputRange = iter->second;
        vector<long long> numberList;
        for (auto iterMap = theMap.begin(); iterMap != theMap.end(); iterMap++)
        {
            auto sourceRange = iterMap->first;
            long long start = sourceRange.first;
            long long end = sourceRange.first + sourceRange.second;
            if (start > inputBegin && start < (inputBegin + inputRange))
            {
                numberList.push_back(start);
            }
            if (end > inputBegin && end < (inputBegin + inputRange))
            {
                numberList.push_back(end);
            }
        }

        numberList.push_back(inputBegin);
        numberList.push_back(inputBegin + inputRange);

        std::sort(numberList.begin(), numberList.end());

        for (auto iterNumber = numberList.begin(); iterNumber != numberList.end() - 1; iterNumber++)
        {
            long long destination = getDestination(*iterNumber, theMap);
            std::pair<long long, long long> resultPair = std::make_pair(destination, *(iterNumber + 1) - *iterNumber);
            result.push_back(resultPair);
        }
    }

    return result;
}