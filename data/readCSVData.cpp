#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

#include "readCSVData.hpp"

std::vector<std::string> Read::readVidIds(std::string filePath)
{
    std::string line;
    std::vector<std::string> vidIds;
    //std::vector<std::string> vidNames;

    std::ifstream file(filePath);

    getline(file,line); // read in header

    while( getline(file,line) )
        vidIds.push_back(line.substr(0, line.find_first_of(','))); //stores video Ids from csv
        //vidNames.push_back(line.substr(line.find_first_of(',')+1, line.length())); //stores video names from csv
    file.close();

    return vidIds;
}

std::vector<std::string> Read::readVidNames(std::string filePath)
{
    std::string line;
    std::vector<std::string> vidNames;
    //std::vector<std::string> vidIds;

    std::ifstream file(filePath);

    getline(file,line); // read in header

    while( getline(file,line) )
        vidNames.push_back(line.substr(line.find_first_of(',')+1, line.length())); //stores video names from csv
        //vidIds.push_back(line.substr(0, line.find_first_of(','))); //stores video Ids from csv
    file.close();

    return vidNames;
}

std::unordered_map<std::string, std::vector<int>> Read::readProfileRatings(std::string filePath)
{
    std::string line;
    std::unordered_map<std::string, std::vector<int>> profileRatings;

    std::fstream file(filePath, std::ios::in);

    getline(file,line); // read in header

    while( getline(file, line) )
    { 
        std::string userName = line.substr(0, line.find_first_of(','));
        line.erase(0, line.find_first_of(',') + 1);
        
        for( size_t i = 0; i < line.length(); ++i )
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

    //std::cout << profileRatings["Tony"].size() << "\n";
    return profileRatings;
}