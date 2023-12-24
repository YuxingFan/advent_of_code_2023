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

// allowed range of test area
static const double positionRangeMin = 200000000000000; //7.0;
static const double positionRangeMax = 400000000000000; // 27.0;

// constant to determine if two float values are the same or not
static const double epsilon = 1e-5;

// suppose y = ax + b
// <a, b, input_value>  input_value = px, py, pz, vx, vy, vz
vector<tuple<double, double, vector<long long>>> trajectories;

// add a trajectory according to the input
void addTrajectory(string& line);
// get the total number of cross within test area in the future
long long getNumOfCrossWithinTestArea();
// check if the cross is within allowed test area or not
bool isWithinTestArea(double x, double y);
// check if the cross is in the future or not
bool isInTheFuture(double x_origin, double x_end, double x_change);

int main()
{
    // init file
    ifstream infile("day24.txt");
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
            // create a trajectory according to the input
            addTrajectory(line);
        }
    }

    long long result = getNumOfCrossWithinTestArea();  
    std::cout << "result: " << result<< endl;

    return 0;
}

void addTrajectory(string& line)
{
   size_t delimiterPosition = line.find('@');
   line.replace(delimiterPosition - 1, 2, ",");
   
   // get input values and store them into a vector
   // px, py, pz, vx, vy, vz
   vector<long long> values;
   stringstream valueStream(line);
   string value;
   while (getline(valueStream, value, ' '))
   {
       values.push_back(stoll(value));
   }

   // (y-py) = k(x-px)
   // k = vy/vx
   
   // y = k*x - px*k + py
   // y = ax + b
   // a = k
   // b = - px*k + py
   
   double k = values.at(4) * 1.0 / values.at(3);
   double a = k;
   double b = -1.0 * values.at(0) * k + values.at(1);

   trajectories.push_back(make_tuple(a, b, values));
}


long long getNumOfCrossWithinTestArea()
{
    // return value
    long long ret = 0;

    // iterate trajectories
    for (auto iter = trajectories.begin(); iter != trajectories.end(); iter++)
    {
        auto firstTrajectory = *iter;
        for (auto iterSecond = iter + 1; iterSecond != trajectories.end(); iterSecond++)
        {
            auto secondTrajectory = *iterSecond;
            
            // if two trajectories are parallel, ignore them
            if (fabs(std::get<0>(firstTrajectory) - std::get<0>(secondTrajectory)) < epsilon)
            {
                continue;
            }
            // otherwise, calculate the cross coorinate
            else
            {
                // calculate the cross point
                double a1 = std::get<0>(firstTrajectory);
                double b1 = std::get<1>(firstTrajectory);

                double a2 = std::get<0>(secondTrajectory);
                double b2 = std::get<1>(secondTrajectory);

                double x_cross = (b2 - b1) / (a1 - a2);
                double y_cross = a1 * x_cross + b1;

                // get the start position and change rate for x, this is used to check if the cross is in the past or future
                double x1_start = std::get<2>(firstTrajectory).front();
                double x1_change = std::get<2>(firstTrajectory).at(3);

                double x2_start = std::get<2>(secondTrajectory).front();
                double x2_change = std::get<2>(secondTrajectory).at(3);
                
                if (isInTheFuture(x1_start, x_cross, x1_change) && isInTheFuture(x2_start, x_cross, x2_change) && isWithinTestArea(x_cross, y_cross))
                {
                    ret++;
                }
                
            }
        }
    }
    return ret;
}

bool isWithinTestArea(double x, double y)
{
    return (x > positionRangeMin || fabs(x - positionRangeMin) < epsilon)
        &&
        (x < positionRangeMax || fabs(x - positionRangeMax) < epsilon)
        &&
        (y > positionRangeMin || fabs(y - positionRangeMin) < epsilon)
        &&
        (y < positionRangeMax || fabs(y - positionRangeMax) < epsilon);
}

bool isInTheFuture(double x_origin, double x_end, double x_change)
{
    return (x_end - x_origin) / x_change > 0;
}