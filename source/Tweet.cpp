#include "Tweet.hpp"

using namespace std;

Tweet::Tweet()
:userid(),tweetid(),tokens(){}

Tweet::Tweet(std::string uid,std::string tid)
:userid(uid),tweetid(tid),tokens(){}

Tweet::~Tweet(){}

void Tweet::add_token(std::string token){
  tokens.push_back(token);
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

void Tweet::print(){
  for(auto it=tokens.begin(); it!=tokens.end(); it++){
    cout << "(" << *it << ")";
  }
  cout << endl;
}
