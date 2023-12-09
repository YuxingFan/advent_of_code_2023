#include <fstream>
#include <sstream>
#include <string>
#include <iostream>¡¡
#include <vector>
#include <map>
#include <cmath>
using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';
static const char verticalDelimiter = '|';

int main()
{
    init();
    // read file
    std::ifstream infile("day6.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    std::string time;
    std::string distance;
    std::getline(infile, time);     //get the seed line line "seeds: 79 14 55 13"
    std::getline(infile, distance);     //get the seed line line "seeds: 79 14 55 13"

    // "seeds: 79 14 55 13"
    std::stringstream timeString(time);
    std::string timeNumber;
    std::stringstream distanceString(distance);
    std::string distanceNumber;

    std::getline(timeString, timeNumber, delimiter);
    std::getline(timeString, timeNumber, delimiter);  

    std::getline(distanceString, distanceNumber, delimiter);
    std::getline(distanceString, distanceNumber, delimiter);  

    //part one
    /*  
    std::stringstream timestream(timeNumber);
    string number;
    std::vector<long long> times;
    // get numbers 
    while (getline(timestream, number, spaceDelimiter))
    {
        if (!number.empty())
        {
            long long integer = stoll(number);
            times.push_back(integer);
        }
    }

    std::stringstream distancestream(distanceNumber);
    std::vector<long long> distances;
    // get numbers 
    while (getline(distancestream, number, spaceDelimiter))
    {
        if (!number.empty())
        {
            long long integer = stoll(number);
            distances.push_back(integer);
        }
    }
    */

    // part two
    std::vector<long long> times;
    while (timeNumber.find(" ") != string::npos)
    {
        timeNumber.replace(timeNumber.find(" "), 1, "");
    }
    times.push_back(stoll(timeNumber));

    std::vector<long long> distances;
    while (distanceNumber.find(" ") != string::npos)
    {
        distanceNumber.replace(distanceNumber.find(" "), 1, "");
    }
    distances.push_back(stoll(distanceNumber));


    // calculate the result
    long long result = 1;
    for (size_t index = 0; index < times.size(); index++)
    {
        long long a = 1.0;
        long long b = -1.0 * times.at(index);
        long long c = 1.0 * distances.at(index);

        double x1 = (-1.0 * b - sqrt(b * b - 4.0 * a * c)) / (2.0 * a);
        double x2 = (-1.0 * b + sqrt(b * b - 4.0 * a * c)) / (2.0 * a);

        long x1_int = ceil(x1);
        long x2_int = floor(x2);

        long number = x2_int - x1_int + 1;

        if ((x1_int * times.at(index) - x1_int * x1_int) == distances.at(index))
        {
            number--;
        }

        if ((x2_int * times.at(index) - x2_int * x2_int) == distances.at(index))
        {
            number--;
        }

        result = result * number;
    }

    std::cout << "result: " << result << std::endl;
    return 0;
}


