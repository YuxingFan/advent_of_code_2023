#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <tuple>
#include <deque>

using namespace std;

static const char delimiter = ':';
static const char spaceDelimiter = ' ';
static const char semiDelimiter = ';';
static const char commaDelimiter = ',';
static const char verticalDelimiter = '|';

static const int buttonModulePress = 1000;
long long highPulse = 0;
long long lowPulse = 0;

// the module connection <module_name, destination_names>
map<string, vector<string>> destinationMap;
// destination of broadcast module
vector<string> destOfBroadcast;

// the state of the flipflop modules <module_name, high/low inpulse>
map<string, bool> flipFlopStateMap;
// the state of the conjunction modules <module_name, <input_module_name, high/low inpulse>>
map<string, map<string, bool>> conjunctionStateMap;

// analyze the line of configuration
void analyze(string& configuration);
// find input modules for conjunction modules
void findInputsForConjunctionModules();
// press the button module once
void pressButton();
// handle a single pulse
vector<tuple<string, bool, string>> handlePulse(tuple<string, bool, string>& currentPulse);

int main()
{
    // init file
    ifstream infile("day21.txt");
    if (!infile.is_open())
    {
        cout << "Open file failed!" << endl;
    }

    // read file
    string line;
    while (getline(infile, line))
    {
        if (!line.empty())
        {
            analyze(line);
        }
    }

    // find input modules for each conjunction module
    findInputsForConjunctionModules();
    for (int index = 0; index < buttonModulePress; index++)
    {
        pressButton();
    }

    std::cout << "highPulse * lowPulse = " << highPulse * lowPulse << endl;
}

void analyze(string& configuration)
{
    // "%a", "&b" or "broadcast"
    string moduleName = configuration.substr(0, configuration.find("->") - 1);
    // " a, b, c"
    string destinations = configuration.substr(configuration.find("->") + 2);

    // get all destination strings
    stringstream destStream(destinations);
    string dest;
    vector<string> dests;
    while (getline(destStream, dest, commaDelimiter))
    {
        dests.push_back(dest.substr(1));
    }

    /* initialization */
    // flip flop
    if (moduleName[0] == '%')
    {
        string module = moduleName.substr(1);
        destinationMap[module] = dests;
        //flipFlopModules.insert(module);
        flipFlopStateMap[module] = false;
    }
    // conjunction
    else if (moduleName[0] == '&')
    {
        string module = moduleName.substr(1);
        destinationMap[module] = dests;
        //conjunctionModules.insert(module);
        conjunctionStateMap[module] = map<string, bool>();
    }
    else
    {
        destOfBroadcast = dests;
    }
}

void findInputsForConjunctionModules()
{
    // iterate all flipflop and conjunction modules
    for (auto iter = destinationMap.begin(); iter != destinationMap.end(); iter++)
    {
        // get module name
        auto inputModuleName = iter->first;
        // get destinations according to the configuration
        auto& destinations = iter->second;
        // iterate destinations
        for (auto iterdest = destinations.begin(); iterdest != destinations.end(); iterdest++)
        {
            // if the destination is a conjunction module, the current module is an input of the conjunction module
            if (conjunctionStateMap.find(*iterdest) != conjunctionStateMap.end())
            {
                conjunctionStateMap[*iterdest][inputModuleName] = false;
            }
        }
    }
}

void pressButton()
{
    // count the number of pulse
    lowPulse++;
    // the pulses to be handled
    // <source, inpulse, destination>
    deque<tuple<string, bool, string>> pulses;
    // initialize the pulses
    for (auto iter = destOfBroadcast.begin(); iter != destOfBroadcast.end(); iter++)
    {
        tuple<string, bool, string> pulse = make_tuple("broadcast", false, *iter);
        pulses.push_back(pulse);
    }

    // if have pulses not handled
    while (!pulses.empty())
    {
        // get the first pulse
        tuple<string, bool, string> currentPulse = pulses.front();
        // handle the first pulse
        vector<tuple<string, bool, string>> newPulses = handlePulse(currentPulse);
        // if new pulses created, add to the queue
        if (!newPulses.empty())
        {
            pulses.insert(pulses.end(), newPulses.begin(), newPulses.end());
        }
        // remove the handled pulse
        pulses.pop_front();
    }
}


vector<tuple<string, bool, string>> handlePulse(tuple<string, bool, string>& currentPulse)
{
    // return value;
    vector<tuple<string, bool, string>> ret;

    // get the information from current pulse
    string sourceModuleName;
    bool pulse;
    string destModuleName;
    tie(sourceModuleName, pulse, destModuleName) = currentPulse;

    // count the pulse
    if (pulse)
    {
        highPulse++;
    }
    else
    {
        lowPulse++;
    }

    // if the destination module is flip flop module
    if (flipFlopStateMap.find(destModuleName) != flipFlopStateMap.end())
    {
        // if it is high pulse, do nothing
        if (pulse)
        {
            return ret;
        }
        else
        {
            // reverse the state type
            flipFlopStateMap[destModuleName] = !flipFlopStateMap[destModuleName];
            // send pulses to all destination modules
            for (auto iterDest = destinationMap[destModuleName].begin(); iterDest != destinationMap[destModuleName].end(); iterDest++)
            {
                ret.push_back(make_tuple(destModuleName, flipFlopStateMap[destModuleName], *iterDest));
            }
            return ret;
        }
    }
    // if the destination module is conjunction module
    else if (conjunctionStateMap.find(destModuleName) != conjunctionStateMap.end())
    {
        // update the memory for the input module
        conjunctionStateMap[destModuleName][sourceModuleName] = pulse;

        // determine which kind of pulse should be sent
        bool isAllHighPulse = true;
        for (auto iterInputState = conjunctionStateMap[destModuleName].begin(); iterInputState != conjunctionStateMap[destModuleName].end(); iterInputState++)
        {
            if (!iterInputState->second)
            {
                isAllHighPulse = false;
                break;
            }
        }
        bool pulseToSend = !isAllHighPulse;

        // send pulses to all destination modules
        for (auto iterDest = destinationMap[destModuleName].begin(); iterDest != destinationMap[destModuleName].end(); iterDest++)
        {
            ret.push_back(make_tuple(destModuleName, pulseToSend, *iterDest));
        }
        return ret;

    }

    // if there is no information about the type of the module
    //cout << "Unknown module type! " << "Module name: " << destModuleName << endl;
    return ret;
}