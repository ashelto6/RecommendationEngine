#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>

#include "readCSVData.hpp"

std::vector<std::string> Read::readVidIds(std::string filePath)
{
    std::string line;
    std::vector<std::string> vidIds;
    //std::vector<std::string> vidNames;

    std::ifstream file(filePath);

    while( getline(file,line) )
        vidIds.push_back(line.substr(0, line.find_first_of(','))); //stores video Ids from csv
        //vidNames.push_back(line.substr(line.find_first_of(',')+1, line.length())); //stores video names from csv
    file.close();

    return vidIds;
}

#include <chrono>
#include <thread>
std::vector<std::string> Read::readVidNames(std::string filePath)
{
    std::string line;
    std::vector<std::string> vidNames;
    //std::vector<std::string> vidIds;

    std::ifstream file(filePath);

    /*
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    */

    while( getline(file,line) )
        vidNames.push_back(line.substr(line.find_first_of(',')+1, line.length())); //stores video names from csv
        //vidIds.push_back(line.substr(0, line.find_first_of(','))); //stores video Ids from csv
    file.close();

    /*
    auto t2 = high_resolution_clock::now();

    // Getting number of milliseconds as an integer. //
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    // Getting number of milliseconds as a double. //
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    */

    return vidNames;
}

std::unordered_map<std::string, std::vector<int>> Read::readProfileRatings(std::string filePath)
{
    std::string line;
    std::unordered_map<std::string, std::vector<int>> profileRatings;

    std::fstream file(filePath, std::ios::in);

    /*    
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    */   

    while( getline(file, line) )
    { 
        std::string userName = line.substr(0, line.find_first_of(','));
        line.erase(0, line.find_first_of(',') + 1);
        std::string delimiter = ",";

        size_t pos = 0;
        std::string token;
        
        for( int i = 0; i < line.length(); ++i )
        {
            if( line[i] != ',' && line[i] == '-' )
            {   
                profileRatings[userName].push_back(-1);
                ++i;
            }
            else if( line[i] != ',' )
                profileRatings[userName].push_back(line[i]-'0');
        }
    }
    file.close(); 

    /*  
    auto t2 = high_resolution_clock::now();

    // Getting number of milliseconds as an integer. //
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    // Getting number of milliseconds as a double. //
    duration<double, std::milli> ms_double = t2 - t1;

    std::cout << ms_int.count() << "ms\n";
    std::cout << ms_double.count() << "ms\n";
    */

    //std::cout << profileRatings["Tony"].size() << "\n";
    return profileRatings;
}