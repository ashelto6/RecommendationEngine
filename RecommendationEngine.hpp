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
    RecommendationEngine(std::vector<std::string>& videos, std::map<std::string, std::vector<int>>& profiles) 
    : videos_(videos), profiles_(profiles) {}

    vecOfStrToDoubPairs getSimilarUsers(const std::string& targetUser);
    
    vecOfStrToDoubPairs getRecommendations(const std::string& user, vecOfStrToDoubPairs simUsers = vecOfStrToDoubPairs());
    
private:
    std::vector<std::string> videos_;
    std::map<std::string, std::vector<int>> profiles_;

    double getSimilarityIndex(const std::string& user_a, const std::string& user_b);
};

#endif //_RECOMMENDATIONENGINE_HPP_

