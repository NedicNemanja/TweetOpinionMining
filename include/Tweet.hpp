#ifndef TWEET_HPP
#define TWEET_HPP

#include <string>
#include <vector>
#include <fstream>

#include "ParsingCSV.hpp"

class Tweet{
    std::string userid;
    std::string tweetid;
    std::vector<std::string> tokens;
  public:
    Tweet();
    Tweet(std::string,std::string);
    ~Tweet();
    void add_token(std::string token);
    void print();
};

//read a tweet from stream and return true for success, else false
Tweet* GetTweet(std::ifstream &data);
#endif
