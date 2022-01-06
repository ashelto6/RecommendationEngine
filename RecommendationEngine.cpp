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
#include <unordered_map>

#include "RecommendationEngine.hpp"
#include "data/readCSVData.hpp"

vecOfStrToDoubPairs RecommendationEngine::getSimilarUsers(const std::string& targetUser, int k)
{
    vecOfStrToDoubPairs similarUsers;

    std::for_each(profiles_->begin(), profiles_->end(), [&](auto& profile){
        if( profile.first != targetUser )
            similarUsers.push_back({profile.first, getSimilarityIndex(targetUser, profile.first)});
    });

    std::sort(similarUsers.begin(), similarUsers.end(), [](auto& user1, auto& user2){
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

vecOfStrToDoubPairs RecommendationEngine::getRecommendations(const std::string& user, int k, vecOfStrToDoubPairs simUsers /*= vecOfStrToDoubPairs()*/)
{
    vecOfStrToDoubPairs vids, similarUsers;
    
    similarUsers = ( !simUsers.size() ) ? getSimilarUsers(user) : simUsers;

    //std::cout << "Video Scores:\n";
    std::cout << unseen_[user].size() << "\n" << videos_->size();
    for( size_t i = 0; i < unseen_[user].size(); ++i )
    {
        //std::cout << (*profiles_)[user][unseen_[user][i]] << "\n";
        double vidScore = 0.0;
        int numOfScores = 0;
        std::for_each( similarUsers.begin(), similarUsers.end(), [&](auto& profile){
            if( (*profiles_)[profile.first][unseen_[user][i]] != 0 )
            {      
                vidScore += (profile.second * (*profiles_)[profile.first][unseen_[user][i]]);
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
        //std::cout << "video" << i+1 << " score: " <<  vidScore << "\n";
        vids.push_back({(*videos_)[unseen_[user][i]], vidScore});
    }

    std::sort(vids.begin(), vids.end(), [](auto& video1, auto& video2){ 
        return video1.second > video2.second; 
        });
    vids.resize(k);
    //std::cout << "\n";

    return vids;
}

double RecommendationEngine::getSimilarityIndex(const std::string& user_a, const std::string& user_b)
{
    double simIndex = 0;
    int numOfScores = 0;

    if( unseen_.find(user_a) == unseen_.end() )
    {
        for( size_t i = 0; i < videos_->size(); ++i )
        {
            if( (*profiles_)[user_a][i] != 0 && (*profiles_)[user_b][i] != 0 )
            {
                if( (*profiles_)[user_a][i] == (*profiles_)[user_b][i] )
                    simIndex += 1.0;
                else
                    simIndex += -1.0;
                ++numOfScores;
            }
            else
            {
                if((*profiles_)[user_a][i] == 0)
                    unseen_[user_a].push_back(i);
            }
        }
    }
    else
    {
        for( size_t i = 0; i < videos_->size(); ++i )
        {
            if( (*profiles_)[user_a][i] != 0 && (*profiles_)[user_b][i] != 0 )
            {
                if( (*profiles_)[user_a][i] == (*profiles_)[user_b][i] )
                    simIndex += 1.0;
                else
                    simIndex += -1.0;
                ++numOfScores;
            }
        }
    }

    if( numOfScores > 0 )
        simIndex /= numOfScores;

    return simIndex;
}

int main()
{
    //std::cout << VIDEOS.size() << "\n";
    //std::cout << PROFILES["Tony"].size() << "\n";
    Read ReadVidData;
    std::unordered_map<std::string, std::vector<int>> profiles = ReadVidData.readProfileRatings("data/profileRatings.csv");               
    std::vector<std::string> videos = ReadVidData.readVidNames("data/youtube_trending_videos_data.csv");
    /*
    std::map<std::string, std::vector<int>> profiles = { 
                                                    {"Tony",    {0, 1, 1, 0, 0, -1, -1, 1, 0, 1, 0, 1}}, 
                                                    {"Jackson", {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, -1}},
                                                    {"Rodger",  {0, 1, 0, 1, 0, -1, -1, 0, 0, 0, 1, 1}},
                                                    {"Joe",     {-1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1}},
                                                    {"Charles", {0, -1, 1, 0, 0, 1, 1, 1, 0, 1, 0, -1}},
                                                    {"David",   {1, 0, 0, -1, 0, -1, 1, 0, 0, -1, 0, -1}} 
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
    std::vector<std::string> userRecommendations = Engine.getRecommendations("Tony");
        for( auto& recommended : userRecommendations )
            std::cout << recommended << "\n";
    */
}
