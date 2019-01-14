#ifndef TWEET_HPP
#define TWEET_HPP

#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "ParsingCSV.hpp"

//for tweet score normalisation
#define ALPHA 15

class Tweet{
    std::string userid;
    std::string tweetid;
    std::vector<std::string> tokens;
    double score;
  public:
    Tweet();
    Tweet(std::string,std::string);
    ~Tweet();
    void add_token(std::string token);
    void print();
    //sum of all tokens sentiment values from lexicon, but normalised
    void Score(std::map<std::string,double> &lexicon);
    double getScore();
    std::string getUserId();
    std::string getTweetId();
    std::vector<std::string> getTokens();
};

//read a tweet from stream and return true for success, else false
Tweet* GetTweet(std::ifstream &data);
//cals tweet scores
void TweetScores(std::vector<Tweet*>&, std::map<std::string,double>&);

#endif
