/*
* Author: Tony S-M
* Date: 12/10/2021

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
    : videos_(&videos), profiles_(&profiles) {}

    vecOfStrToDoubPairs getSimilarUsers(const std::string& targetUser, int k = 3);
    
    vecOfStrToDoubPairs getRecommendations(const std::string& user, int k, vecOfStrToDoubPairs simUsers = vecOfStrToDoubPairs());
    
private:
    std::vector<std::string>* videos_;
    std::unordered_map<std::string, std::vector<int>>* profiles_;
    std::unordered_map<std::string, std::vector<int>> unseen_;

    double getSimilarityIndex(const std::string& user_a, const std::string& user_b);
};

#endif //_RECOMMENDATIONENGINE_HPP_

