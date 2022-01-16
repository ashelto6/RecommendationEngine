/*
* Author: Tony S-M
* Date: 12/10/2021
*
* A RecommendationEngine class definition
*
* Using collaborative filtering
*/

//#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <array>
#include <iterator>
#include <cmath>
#include <thread>
#include <memory>
#include <unordered_map>

#include "RecommendationEngine.hpp"
#include "data/readCSVData.hpp"

vecOfStrToDoubPairs RecommendationEngine::getSimilarUsers(const std::string& targetUser, int k /*= 3*/)
{
    if( k < 1 )
        k = 1;
    else if( k >= profiles_->size() )
        k = profiles_->size()-1;
    
    vecOfStrToDoubPairs similarUsers;

    std::for_each(profiles_->begin(), profiles_->end(), [&](auto& profile)
    {
        if( profile.first != targetUser )
            similarUsers.push_back({profile.first, similarityIndex(targetUser, profile.first)});
    });

    std::partial_sort(similarUsers.begin(), similarUsers.begin()+k, similarUsers.end(), [](auto& user1, auto& user2)
    {
        return user1.second > user2.second; 
    });

    similarUsers.resize(k);
    
    /*
    std::cout << "\nSimilar Users for " << targetUser << ": \n";
    for( auto& user : similarUsers )
        std::cout << user.first << ": " << user.second << "\n";
    std::cout << "\n";
    */

    return similarUsers;
}

vecOfStrToDoubPairs RecommendationEngine::getRecommendations(const std::string& targetUser, int k, vecOfStrToDoubPairs simUsers /*= vecOfStrToDoubPairs()*/)
{
    if( k < 1 )
        k = 1;
    else if( k > unseen_[targetUser].size() )
        k = unseen_[targetUser].size();
    
    vecOfStrToDoubPairs vids, similarUsers;

    similarUsers = ( !simUsers.size() ) ? getSimilarUsers(targetUser) : simUsers;
    /*
    std::cout << "Video Scores:\n";
    std::cout << unseen_[targetUser].size() << "\n" << videos_->size();
    */
    for( size_t i = 0; i < unseen_[targetUser].size(); ++i )
    {
        double vidScore = 0.0;
        int numOfScores = 0;

        std::for_each( similarUsers.begin(), similarUsers.end(), [&](auto& profile)
        {
            if( isZero((*profiles_)[profile.first][unseen_[targetUser][i]]) == false && isZero(profile.second) == false )
            {      
                vidScore += (profile.second * (*profiles_)[profile.first][unseen_[targetUser][i]]);
                ++numOfScores;
            }
        });

        if( numOfScores > 0 )
        {
            /*
            std::cout << "raw score: " <<  vidScore 
                        << "\nsize of rawvidscores list: " << numOfScores << "\n";
            */
            vidScore /= numOfScores;
        }
        //std::cout << "video" << unseen_[targetUser][i]+1 << " score: " <<  vidScore << "\n";
        vids.push_back({(*videos_)[unseen_[targetUser][i]], vidScore});
    }

    std::partial_sort(vids.begin(), vids.begin()+k, vids.end(), [](auto& video1, auto& video2)
    { 
        return video1.second > video2.second; 
    });

    vids.resize(k);
    //std::cout << "\n";

    return vids;
}

double RecommendationEngine::similarityIndex(const std::string& targetUser, const std::string& user_b)
{
    double simIndex = 0.0;
    int numOfScores = 0;

    simIndex = simNScores_[targetUser][user_b][0];
    numOfScores = int(simNScores_[targetUser][user_b][1]);
    
    if( numOfScores > 0 )
        simIndex /= numOfScores;
    
    return simIndex;
}

// SOMEWHAT SLOW
void RecommendationEngine::seenUnseen(const std::string& targetUser, const std::string& user_b)
{
    // caching seen and unseen videos
    if( unseen_.find(user_b) == unseen_.end() ) // if other user is also not in unseen vector
    {
        for( size_t i = 0; i < videos_->size(); ++i )
        {
            if( isZero((*profiles_)[targetUser][i]) == false && isZero((*profiles_)[user_b][i]) == false )
            {
                seen_[targetUser].push_back(i);
                seen_[user_b].push_back(i);
            }
            else
            {
                if( isZero((*profiles_)[targetUser][i]) == true )
                    unseen_[targetUser].push_back(i);
                else
                    seen_[targetUser].push_back(i);

                if( isZero((*profiles_)[user_b][i]) == true )
                    unseen_[user_b].push_back(i);
                else
                    seen_[user_b].push_back(i);
            }
        }
    }
    else // if target user is not in unseen vector but the other user is
    {
        for( size_t i = 0; i < videos_->size(); ++i )
        {
            if( isZero((*profiles_)[targetUser][i]) == false )
                seen_[targetUser].push_back(i);

            else
                unseen_[targetUser].push_back(i);
        }    
    }
}

void RecommendationEngine::cacheData()
{
    std::cout << "\nCalculating and Caching Data..."
              << "\nLooks like this may take a few hours!\n\n";

    std::ostream& os = std::clog;
    std::string message = "Progress: #";
    char symbol = '#';
    int percentage = 1; 
    int pct = 0;
    const int progress_level = 1;

    os << "\r [" << std::setw(3) << percentage << "%] " << message << std::flush;
    
    cacheSeenUnseen();

    double remains = 0.0; 
    if( profiles_->size() >= 100 )
    {   
        remains = profiles_->size() / 100;
        if( profiles_->size() % 100 != 0 )
            remains += (profiles_->size() % 100) * .01;
    }

    for( auto& targetUser : *profiles_ )
    {
        ++pct;
           
        cacheSimNScores(targetUser.first);

        if( profiles_->size() >= 100 )
        {     
            if( (pct % int(std::ceil(remains))) == 0 )
            { 
                percentage += progress_level; 
                message += symbol;
                os << "\r [" << std::setw(3) << percentage << "%] " << message << std::flush;
            }
        }
        else
        {
            percentage += progress_level; 
            message += symbol;
            os << "\r [" << std::setw(3) << percentage << "%] " << message << std::flush;
        }
    }

    os << "\r [" << std::setw(3) << 100 << "%] Progress: ####################################################################################################" << std::flush;
    std::cout << "\nCalculations and Caching completed!\n";    
}

void RecommendationEngine::cacheSeenUnseen()
{
    auto targetUser = profiles_->begin();
    if( unseen_.find(targetUser->first) == unseen_.end() )
    {
        for( auto& user_b : *profiles_ )
        {
            if( user_b.first != targetUser->first )
            {
                if( unseen_.find(user_b.first) == unseen_.end() )
                    seenUnseen(user_b.first, targetUser->first);
            }
        }
    }
}

// SLOW
void RecommendationEngine::cacheSimNScores(const std::string& targetUser)
{
    std::vector<std::unordered_map<std::string, std::array<double,2>>> splitMaps(4, std::unordered_map<std::string, std::array<double,2>>());
    
    const int splits = profiles_->size()/4;

    //profiles_->begin() to endQuad1
    auto endQuad1 = profiles_->begin();
    std::advance(endQuad1, splits);
    //endQuad1 to endQuad2
    auto endQuad2 = endQuad1;
    std::advance(endQuad2, splits);
    //endQuad2 to endQuad3
    auto endQuad3 = endQuad2;
    std::advance(endQuad3, splits);
    //endQuad3 to profiles_->end()
    
    if( simNScores_.find(targetUser) == simNScores_.end() )
    {
        for( size_t i = 0; i < seen_[targetUser].size(); ++i )
        {
            int vidIndex = seen_[targetUser][i];

            std::thread t1(&RecommendationEngine::simNScoresForThreads, this, std::ref(splitMaps[0]), profiles_->begin(), endQuad1, vidIndex, targetUser);
            std::thread t2(&RecommendationEngine::simNScoresForThreads, this, std::ref(splitMaps[1]), endQuad1, endQuad2, vidIndex, targetUser);
            std::thread t3(&RecommendationEngine::simNScoresForThreads, this, std::ref(splitMaps[2]), endQuad2, endQuad3, vidIndex, targetUser);
            std::thread t4(&RecommendationEngine::simNScoresForThreads, this, std::ref(splitMaps[3]), endQuad3, profiles_->end(), vidIndex, targetUser);

            t1.join();
            t2.join();
            t3.join();
            t4.join();
        }
    }
    
    std::unordered_map<std::string, std::array<double,2>> finishedMap;

    for( auto& map2Merge : splitMaps )
        finishedMap.merge(map2Merge);

    simNScores_[targetUser] = finishedMap;
}

void RecommendationEngine::simNScoresForThreads(std::unordered_map<std::string, std::array<double,2>>& map2Merge, 
                          std::unordered_map<std::string, std::vector<int>>::iterator begin, 
                          std::unordered_map<std::string, std::vector<int>>::iterator end, const int vidIndex, const std::string targetUser)
{
    double simIndex = 0.0;
    std::for_each(begin, end, [&](auto& profile)
    {
        if( profile.first != targetUser )
        {
            if( isZero((*profiles_)[profile.first][vidIndex]) == false )
            {
                simIndex = ( isEqual((*profiles_)[targetUser][vidIndex], (*profiles_)[profile.first][vidIndex]) ) ? 1.0 : -1.0;
                map2Merge[profile.first][1] += 1;
            }
            map2Merge[profile.first][0] += simIndex;
            map2Merge[profile.first][1] += 0;
        }
    });
}

bool RecommendationEngine::isZero(const double simScore)
{
    if( simScore == 0.0 )
        return true;
    return false;
}

bool RecommendationEngine::isZero(const int rating)
{
    if ( rating == 0 )
        return true;
    return false;    
}








int main()
{
    Read ReadVidData;
    std::cout << "\nReading Profile and Ratings data from csv...\n";
    std::unordered_map<std::string, std::vector<int>> profiles = ReadVidData.readProfileRatings("data/profileRatings_sm.csv");
    std::cout << "Reading of Profile and Ratings data from csv done!\n"  
              << "\nReading video names data from csv...\n";
    std::vector<std::string> videos = ReadVidData.readVidNames("data/youtube_trending_videos_data_sm.csv");
    std::cout << "Reading of video names data from csv done!\n";

    /*
    std::unordered_map<std::string, std::vector<int>> profiles = { 
                                                    {"Tony",    {0, 1, 1, 0, 0, -1, -1, 1, 0, 1, 0, 1}}, 
                                                    {"Jackson", {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, -1}},
                                                    {"Rodger",  {0, 1, 0, 1, 0, -1, -1, 0, 0, 0, 1, 1}},
                                                    {"Joe",     {-1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1}},
                                                    {"Charles", {0, -1, 1, 0, 0, 1, 1, 1, 0, 1, 0, -1}},
                                                    {"David",   {1, 0, 0, -1, 0, -1, 1, 0, 0, -1, 0, -1}}, 
                                                    {"Jason",   {-1, 0, 1, -1, 0, 1, 0, -1, 0, -1, 1, -1}},
                                                    {"Amy",     {0, 0, 0, -1, -1, 0, 0, -1, 1, -1, -1, 1}} 
                                                };
                                          


    std::vector<std::string> videos = {
                                    "1) Zach King's Harry Potter illusion - 2.2 billion views", 
                                    "2) James Charles' Christmas wonderland transition - 1.7 billion views", 
                                    "3) Addison Rae's WAP challenge - 302.9 million views", 
                                    "4) Daeox's video of a smiling baby - 375.2 million views", 
                                    "5) Billie Eilish's face warp challenge - 325.8 million views", 
                                    "6) Bella Poarch's 'M To The B' video - 610.8 million views",
                                    "7) Lorem ipsum dolor sit amet, consectetur adipiscing elit.", 
                                    "8) Lorem ipsum dolor sit amet, consectetur adipiscing elit. sit ipsum dolor", 
                                    "9) Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut sollicitudin.", 
                                    "10) Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque viverra.", 
                                    "11) Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla varius.", 
                                    "12) Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque posuere."
                                    };
    */
    
    // caching data
    RecommendationEngine Engine(videos, profiles);
    
    for( auto& user : profiles )
    {
        std::cout << "\n#########################################################################################################\n";

        std::vector<std::pair<std::string, double>> similarUsers = Engine.getSimilarUsers(user.first);
        
        std::vector<std::pair<std::string, double>> videoRecommendations = Engine.getRecommendations(user.first, 3, similarUsers);

        std::cout << "\n" << user.first << ", here are your recommended videos:\n";
        for( auto& recommended : videoRecommendations )
            std::cout << "\t" << recommended.first << "\n";
        
        std::cout << "\nhere are your top " << similarUsers.size() << " most similar users:\n";
        for( auto& simUser : similarUsers )
            std::cout << "\t" << simUser.first << "\n";
            
        std::cout << "#########################################################################################################\n";
    }
    
    /*
    // client interaction begins
    std::string name;
    std::cout << "\nEnter a username: ";
    std::cin >> name;

    while( name != "" )
    {
        while( profiles.find(name) == profiles.end() )
        {
            std::cout << "That username is not valid. Please Try Again\n"
                      << "\nEnter a username: ";
            std::cin >> name;
        }
        std::cout << "Fetching Recommendations...\n";
     
        std::vector<std::pair<std::string, double>> userRecommendations = Engine.getRecommendations(name,3);
        std::cout << "\n" << name << ", here are your recommended videos:\n";
        for( auto& recommended : userRecommendations )
            std::cout << recommended.first << "\n";

        std::cout << "\nEnter a username: ";
        std::cin >> name;
    }
    */
}