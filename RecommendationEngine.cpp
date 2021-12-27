/*
* Author: Tony S-M
* Date: 12/10/2021

* A RecommendationEngine class definition
*
* Using collaborative filtering
*/

#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>
#include <map>

#include "RecommendationEngine.hpp"
#include "data/readCSVData.hpp"

vecOfStrToDoubPairs RecommendationEngine::getSimilarUsers(const std::string& targetUser)
{
    vecOfStrToDoubPairs similarUsers;

    std::for_each(profiles_.begin(), profiles_.end(), [&](auto& profile){
        if( profile.first != targetUser )
            similarUsers.push_back({profile.first, getSimilarityIndex(targetUser, profile.first)});
    });

    std::sort(similarUsers.begin(), similarUsers.end(), [](auto& user1, auto& user2){
        return user1.second > user2.second; 
    });
    similarUsers.resize(3);

    /*
    std::cout << "\nSimilar Users for " << targetUser << ": \n";
    for( auto& user : similarUsers )
        std::cout << user.first << ": " << user.second << "\n";
    std::cout << "\n";
    */

    return similarUsers;
}

vecOfStrToDoubPairs RecommendationEngine::getRecommendations(const std::string& user, vecOfStrToDoubPairs simUsers /*= vecOfStrToDoubPairs()*/)
{
    vecOfStrToDoubPairs vids, similarUsers;
    
    similarUsers = ( !simUsers.size() ) ? getSimilarUsers(user) : simUsers;

    std::vector<std::string> recommendations;
    double vidScore = 0.0;

    for( size_t i = 0; i < videos_.size(); ++i )
        if( profiles_[user][i] == 0 )
        {
            double rawVidScoreSum = 0.0;
            std::vector<double> rawVidScores;

            std::for_each( similarUsers.begin(), similarUsers.end(), [&](auto& profile){
                if( profiles_[profile.first][i] != 0 )      
                    rawVidScores.push_back(profile.second * profiles_[profile.first][i]);
            });

            if( rawVidScores.size() > 0 )
            {
                rawVidScoreSum = std::accumulate(rawVidScores.begin(), rawVidScores.end(), 0.0);
                //std::cout << "raw score: " <<  rawVidScoreSum 
                            //<< "\nsize of rawvidscores list: " << rawVidScores.size() << "\n";
                vidScore = rawVidScoreSum/rawVidScores.size();
            }
            else
                vidScore = 0.0;

            //std::cout << "video" << i+1 << " score: " <<  vidScore << "\n";
            vids.push_back({videos_[i], vidScore});
        }

    std::sort(vids.begin(), vids.end(), [](auto& video1, auto& video2){ 
        return video1.second > video2.second; 
        });

    vids.resize(3);
    //std::cout << "\n";

    return vids;
}

double RecommendationEngine::getSimilarityIndex(const std::string& user_a, const std::string& user_b)
{
    std::vector<double> simIndexList;
    double simIndex;

    for( size_t i = 0; i < videos_.size(); ++i )
    {
        if( profiles_[user_a][i] != 0 && profiles_[user_b][i] != 0 )
        {
            if( profiles_[user_a][i] == profiles_[user_b][i] )
                simIndexList.push_back(1.0);
            else
                simIndexList.push_back(-1.0);
        }
    }

    double rawSum = 0.0;
    if( simIndexList.size() > 0 )
    {
        rawSum = std::accumulate(simIndexList.begin(), simIndexList.end(), 0.0);
        simIndex = rawSum/simIndexList.size();
    }
    else
        simIndex = 0.0;

    return simIndex;
}

std::map<std::string, std::vector<int>> PROFILES = { 
                                                    {"Tony",    {0, 1, 1, 0, 0, -1, -1, 1, 0, 1, 0, 1}}, 
                                                    {"Jackson", {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, -1}},
                                                    {"Rodger",  {0, 1, 0, 1, 0, -1, -1, 0, 0, 0, 1, 1}},
                                                    {"Joe",     {-1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1}},
                                                    {"Charles", {0, -1, 1, 0, 0, 1, 1, 1, 0, 1, 0, -1}},
                                                    {"David",   {1, 0, 0, -1, 0, -1, 1, 0, 0, -1, 0, -1}} 
                                                };

std::vector<std::string> VIDEOS = {
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

//Read ReadVidData;                  
//std::vector<std::string> VIDEOS = ReadVidData.readVidIds("data/youtube_trending_videos_data.cpp");

int main()
{
    RecommendationEngine Engine(VIDEOS, PROFILES);

    for( auto& user : PROFILES )
    {
        std::cout << "\n#########################################################################################################\n";

        std::vector<std::pair<std::string, double>> similarUsers = Engine.getSimilarUsers(user.first);
        
        //Labeled example
        std::vector<std::pair<std::string, double>> videoRecommendations = Engine.getRecommendations(user.first, similarUsers);

        std::cout << user.first << ", here are your recommended videos:\n";
        for( auto& recommended : videoRecommendations )
            std::cout << "\t" << recommended.first << "\n";
        
        std::cout << "\nhere are your top 3 most similar users:\n";
        for( auto& simUser : similarUsers )
            std::cout << "\t" << simUser.first << "\n";
            
        std::cout << "#########################################################################################################\n";
    }
    /*
    std::vector<std::string> userRecommendations = Engine.getRecommendations("Tony");
        for( auto& recommended : userRecommendations )
            std::cout << recommended << "\n";
    */
}
