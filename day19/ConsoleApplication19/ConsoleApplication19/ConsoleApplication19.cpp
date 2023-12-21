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

// The workflows from the input
vector<string> workflows;
// The parts from the input
vector<string> parts;
// A map to store the rules in a workflow <workflow_name, rules>
map<string, vector<string>> ruleMap;

// fill the ruleMap with given workflows
void getRulesFromWorkflow();
// get rating number of a part. It is 0 if the part is rejected, otherwise the sum of rating of each category
long long getRatingNumberOfPart(string& part);
// get the result according to rules
long long getValueFromRules(map<char, long long>& categoryMap, string& workflowName);

int main()
{
    // read file
    std::ifstream infile("day19.txt");
    if (!infile.is_open())
    {
        std::cout << "Open file failed!" << std::endl;
    }

    // read all lines
    bool isWorkflow = true;
    std::string line;
    while (std::getline(infile, line))
    {
        if (!line.empty())
        {
            if (isWorkflow)
            {
                workflows.push_back(line);
            }
            else
            {
                parts.push_back(line);
            }
        }
        else
        {
            isWorkflow = false;
        }
    }

    // analyze the workflow string and store the data into a map
    getRulesFromWorkflow();
    long long totalRating = 0;
    for (auto iter = parts.begin(); iter != parts.end(); iter++)
    {
        // get the rating number of a part
        totalRating += getRatingNumberOfPart(*iter);
    }

    std::cout << "totalRating: " << totalRating;// << "    " << "sumOfBackHistory: " << sumOfBackHistory << std::endl;
    return 0;
}

void getRulesFromWorkflow()
{
    for (auto iter = workflows.begin(); iter != workflows.end(); iter++)
    {
        string name = iter->substr(0, iter->find('{', 0));
        string ruleString = iter->substr(iter->find('{', 0) + 1);
        ruleString.pop_back();

        stringstream ruleStream(ruleString);
        string rule;
        vector<string> rules;
        while (getline(ruleStream, rule, commaDelimiter))
        {
            rules.push_back(rule);
        }

        ruleMap[name] = rules;
    }
}

long long getRatingNumberOfPart(string& part)
{
    part = part.substr(1, part.size() - 2);

    stringstream partStream(part);
    string categoryString;
    map<char, long long> categoryMap;
    
    // store the rating of each category into a map
    while (getline(partStream, categoryString, commaDelimiter))
    { 
       categoryMap[categoryString[0]] = stoll(categoryString.substr(2));
    }

    // starting from the workflow "in", get the result according to rules
    string workflowName = "in";
    long long ret = getValueFromRules(categoryMap, workflowName);

    return ret;
}

long long getValueFromRules(map<char, long long>& categoryMap, string& workflowName)
{
    // Accepted, get the sum of values
    if (!workflowName.compare("A"))
    {
        long long ret = 0;
        for (auto iter = categoryMap.begin(); iter != categoryMap.end(); iter++)
        {
            ret += iter->second;
        }
        return ret;
    }
    // Rejected, return 0
    else if (!workflowName.compare("R"))
    {
        return 0;
    }
    // It is the name of a workflow, need to check rules
    else
    {
        vector<string> rules = ruleMap[workflowName];
        for (auto iter = rules.begin(); iter != rules.end(); iter++)
        {
            // the rule is true without conditions
            if (iter->find(':', 0) == string::npos)
            {
                return getValueFromRules(categoryMap, *iter);
            }
            // there is a rule for the condition
            else
            {
                char category = iter->at(0);
                char operation = iter->at(1);
                long long ruleNumber = stoll(iter->substr(2));

                bool fulfilled = false;
                if (
                    (operation == '>' && categoryMap[category] > ruleNumber)
                    ||
                    (operation == '<' && categoryMap[category] < ruleNumber)
                   )
                {
                    fulfilled = true;
                }

                if (fulfilled)
                {
                    string nextWorkflow = iter->substr(iter->find(':', 0) + 1);
                    return getValueFromRules(categoryMap, nextWorkflow);
                }
                
            }
        }
    }

    return 0;
}