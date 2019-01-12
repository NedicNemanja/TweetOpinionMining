#include "User.hpp"

using namespace std;

User::User(){}

User::User(string &uid)
:userid(uid){}

User::~User(){}

void User::addTweet(Tweet* tweet){
  tweets.push_back(tweet);
}

void User::print(){
  cout << userid << ":{";
  for(auto it=tweets.begin(); it!=tweets.end(); it++){
    cout << (*it)->getTweetId() << ",";
  }
  cout << "}\n";
}

vector<User*> GroupTweetsByUser(UserMap &usermap, std::vector<Tweet*> &Tweets){
  vector<User*> users;
  for(auto it=Tweets.begin(); it!=Tweets.end(); it++){
    string uid = (*it)->getUserId();
    if(usermap.find(uid) == usermap.end()){
      //user not in map
      User* user = new User(uid); //create user
      users.push_back(user);
      usermap.insert(pair<string,User*>(uid,user));
    }
    usermap[uid]->addTweet(*it);
  }
  cout << "Found " << users.size() << " different users." << endl;
  return users;
}
