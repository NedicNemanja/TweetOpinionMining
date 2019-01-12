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

std::vector<Tweet*> User::getTweets(){
  return tweets;
}

vector<User*> GroupTweetsByUser(UserMap &usermap, std::vector<Tweet*> &Tweets){
  vector<User*> users;
  for(auto it=Tweets.begin(); it!=Tweets.end(); it++){
    string uid = (*it)->getUserId();
    if(usermap.find(uid) == usermap.end()){
      //user not in map
      User* user = new User(uid); //create user
      users.push_back(user);
      usermap[uid] = user;
    }
    usermap[uid]->addTweet(*it);
  }
  cout << "Found " << users.size() << " different users." << endl;
  return users;
}

/*take the score of every user's tweet and add it to the
cryptocurrency/ies the tweet mentions*/
void User::CalcCryptoValues(map<string,string> &CryptoNameMap){
  for(auto tweet=tweets.begin(); tweet!=tweets.end(); tweet++){

    vector<std::string> tokens = (*tweet)->getTokens();
    for(auto token=tokens.begin(); token!=tokens.end(); token++){

      map<string,string>::iterator element;
      if( (element=CryptoNameMap.find(*token)) != CryptoNameMap.end()){
        //token is a cryptocurrency
        string crypto = element->second;

        map<string,double>::iterator val;
        if((val=crypto_values.find(crypto)) == crypto_values.end()){
          //this crypto was previously unknown to the user
          crypto_values[crypto] = (*tweet)->getScore();
        }
        else{
          //this crypto is known to the user and already has a value
          crypto_values[crypto] += (*tweet)->getScore();
        }
      }
    }
  }
}
