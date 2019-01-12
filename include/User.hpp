#ifndef USER_HPP
#define USER_HPP

#include "Tweet.hpp"

#include <vector>
#include <string>
#include <map>


class User{
    std::string userid;
    std::vector<Tweet*> tweets;
  public:
    User();
    User(std::string &uid);
    ~User();
    void addTweet(Tweet* tweet);
    void print();
};

typedef std::map<std::string,User*> UserMap; //maps users by userid

//assign each tweet to its user and return all users (creates Users)
std::vector<User*> GroupTweetsByUser(UserMap &usermap, std::vector<Tweet*> &Tweets);

#endif
