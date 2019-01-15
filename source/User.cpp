#include "User.hpp"
#include "ReadInput.hpp"
#include "LSH.hpp"
#include "CosineSimilarity.hpp"
#include "utility.hpp"

#include <math.h>

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

std::string User::getUserId(){
  return userid;
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

    std::vector<std::string> tokens = (*tweet)->getTokens();
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

double User::AverageValue(){
  double sum=0;
  int num_cryptos=0;
  for(auto it=crypto_values.begin(); it!=crypto_values.end(); it++){
    sum += it->second;
    num_cryptos++;
  }
  return sum/num_cryptos;
}

/*Create myvector using crypto_values,ordered as in CryptoSet.
If there are unknown crypto_values use average value.
Values are normalised by subtracting average_value from each of them.*/
myvector* User::Vectorize(set<string> CryptoSet){
  double average_value = AverageValue();
  if(isnan(average_value))  //if zero vector
    return NULL;
  std::vector<double> myvalues(CryptoSet.size());
  int i=0;
  for(auto it=CryptoSet.begin(); it!=CryptoSet.end(); it++,i++){
    map<string,double>::iterator val;
    if((val=crypto_values.find(*it)) == crypto_values.end()){
      //crypto unknown to user
      myvalues[i] = average_value;
    }
    else{
      //crypto known to user and has a value
      myvalues[i] = val->second-average_value;
    }
  }
  vector = myvector(myvalues,userid);
  return &vector;
}

void User::RateByNNSimilarity(std::vector<HashTable*> &LSH_Hashtables,
  set<string> &CryptoSet){
  //find nearest neighbors of this user
  std::vector<myvector*> nns = NearestNeighbors(LSH_Hashtables,vector,CmdArgs::NUM_NN);
  //calculate similarities to nns
  std::vector<double> similarities(nns.size());
  double sim_sum=0;
  int i=0;
  for(auto it=nns.begin(); it!=nns.end(); it++,i++){
    similarities[i] = CosineVectorSimilarity(vector.begin(),vector.end(),(*it)->begin());
    sim_sum += similarities[i];
  }
  //approximate unknown crypto values using similarities
  int coord=0;
  for(auto crypto=CryptoSet.begin(); crypto!=CryptoSet.end(); crypto++,coord++){
    //rate only unknown crypto values
    if(crypto_values.find(*crypto) == crypto_values.end())
      continue;
    double nn_ratings=0;
    for(int i=0; i<nns.size(); i++)
      nn_ratings += (similarities[i]*((*nns[i])[coord]));
    if(sim_sum != 0)
      vector[coord] = nn_ratings/sim_sum;
  }
}

vector<myvector*> VectorizeUsers(vector<User*> &Users, set<string> &CryptoSet){
  vector<myvector*> UserVectors;
  for(auto it=Users.begin(); it!=Users.end(); it++){
    myvector* vector = (*it)->Vectorize(CryptoSet);
    if(vector==NULL){
      //remove users that didn't mention any crypto
      delete(*it);
      *it = NULL;
    }
    else if(isZeroVector(vector->begin(),vector->end())){
      //remove users that didn't mention any crypto
      delete(*it);
      *it = NULL;
    }
    else{
      UserVectors.push_back(vector);
    }
}
  return UserVectors;
}

vector<User*> CreateVirtualUsers(const std::vector<Cluster> &Clusters,
  map<string,Tweet*>&tweet_id_map){
  vector<User*> vusers;

  return vusers;
}
