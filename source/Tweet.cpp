#include "Tweet.hpp"
#include <cmath>

using namespace std;

Tweet::Tweet()
:userid(),tweetid(),tokens(){}

Tweet::Tweet(std::string uid,std::string tid)
:userid(uid),tweetid(tid),tokens(){}

Tweet::~Tweet(){}

void Tweet::add_token(std::string token){
  tokens.push_back(token);
}

void Tweet::print(){
  for(auto it=tokens.begin(); it!=tokens.end(); it++){
    cout << "(" << *it << ")";
  }
  cout << endl;
}

//sum of all tokens sentiment values from lexicon, but normalised
void Tweet::Score(std::map<std::string,double> &lexicon){
  double total_score=0;
  for(auto it=tokens.begin(); it!=tokens.end(); it++){
    total_score += lexicon[*it];
  }
  score = total_score/sqrt(total_score*total_score+alpha);
}

double Tweet::getScore(){
  return score;
}

string Tweet::getUserId(){
  return userid;
}

string Tweet::getTweetId(){
  return tweetid;
}

std::vector<std::string> Tweet::getTokens(){
  return tokens;
}



Tweet* GetTweet(std::ifstream &data){
  //read id
  CSVRow row;
  int row_index=0;
  data >> row;  //read a whole row
  if(data.eof()) return NULL;
  Tweet* tweet = new Tweet(row[row_index++],row[row_index++]);
  //read tokens
  for(;row_index<row.size(); row_index++){
    tweet->add_token(row[row_index]);
  }
  return tweet;
}

//cals tweet scores
void TweetScores(vector<Tweet*> &Tweets, map<string,double> &lexicon){
  for(auto it=Tweets.begin(); it!=Tweets.end(); it++){
    (*it)->Score(lexicon);
  }
}
