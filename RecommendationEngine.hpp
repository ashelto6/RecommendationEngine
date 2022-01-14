/*
* Author: Tony S-M
* Date: 12/10/2021
*
* A RecommendationEngine class
*
* Using collaborative filtering
*/

#ifndef _RECOMMENDATIONENGINE_HPP_
#define _RECOMMENDATIONENGINE_HPP_

typedef std::vector<std::pair<std::string, double>> vecOfStrToDoubPairs;

class RecommendationEngine{
public:
    RecommendationEngine(std::vector<std::string>& videos, std::unordered_map<std::string, std::vector<int>>& profiles) 
    : videos_(std::make_unique<std::vector<std::string>>(videos)), profiles_(std::make_unique<std::unordered_map<std::string, std::vector<int>>>(profiles)) { cacheData(); }

    vecOfStrToDoubPairs getSimilarUsers(const std::string& targetUser, int k = 3);
    vecOfStrToDoubPairs getRecommendations(const std::string& targetUser, int k, vecOfStrToDoubPairs simUsers = vecOfStrToDoubPairs());
    
private:
    void simNScoresForThreads(std::unordered_map<std::string, std::array<double,2>>& map2Merge, 
                              std::unordered_map<std::string, std::vector<int>>::iterator begin, 
                              std::unordered_map<std::string, std::vector<int>>::iterator end, const int vidIndex, const std::string targetUser);

    double similarityIndex(const std::string& targetUser, const std::string& user_b);
    void seenUnseen(const std::string& targetUser, const std::string& user_b);

    void cacheData();
    void cacheSeenUnseen();
    void cacheSimNScores(const std::string& targetUser);

    bool isEqual(const int left, const int right) { return left == right; }

    // overloaded methods
    bool isZero(const int rating);
    bool isZero(const double simScore);

    std::unique_ptr<std::vector<std::string>> videos_; // points to a vector of video titles
    std::unique_ptr<std::unordered_map<std::string, std::vector<int>>> profiles_; // points to an unordered map keyed to usernames with values being their ratings
    
    std::unordered_map<std::string, std::vector<int>> unseen_; // cache for indices of unseen videos for each user
    std::unordered_map<std::string, std::vector<int>> seen_; // cache for indices of seen videos for each user
    std::unordered_map<std::string, std::unordered_map<std::string, std::array<double,2>>> simNScores_; // cache for a map of data for each user
};

#endif //_RECOMMENDATIONENGINE_HPP_