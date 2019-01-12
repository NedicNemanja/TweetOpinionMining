#ifndef USER_HPP
#define USER_HPP

#include "Tweet.hpp"

#include <vector>
#include <string>
#include <map>


class User{
    std::string userid;
    std::vector<Tweet*> tweets;
    std::map<std::string,double> crypto_values;
  public:
    User();
    User(std::string &uid);
    ~User();
    void addTweet(Tweet* tweet);
    void print();
    std::vector<Tweet*> getTweets();
    /*take the score of every user's tweet and add it to the
    cryptocurrency/ies the tweet mentions*/
    void CalcCryptoValues(std::map<std::string,std::string> &CryptoNameMap);
};

typedef std::map<std::string,User*> UserMap; //maps users by userid

//assign each tweet to its user and return all users (creates Users)
std::vector<User*> GroupTweetsByUser(UserMap &usermap, std::vector<Tweet*> &);

#endif
