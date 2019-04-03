#include <iostream>
#include <fstream>
#include <time.h>
#include <set>
 
#include "ReadInput.hpp"
#include "ErrorCodes.hpp"
#include "Tweet.hpp"
#include "User.hpp"
#include "myvector.hpp"
#include "HashTable.hpp"
#include "utility.hpp"
#include "WriteOutput.hpp"

using namespace std;

#define K 10 //number of clusters for 2A

vector<string> CryptoOrder;  //how cryptos are ordered in myvectors

void SetCryptoOrder(set<string>& CryptoSet){
  for(auto it=CryptoSet.begin(); it!=CryptoSet.end(); it++)
    CryptoOrder.push_back(*it);
}

int main(int argc, char** argv){
  /*****************PREPROCESSING**********************************************/
  ParseArguments(argc,argv);
  //open outfile
  if(CmdArgs::OutFile.empty()){
    cout << endl << "Provide outfile path:" << endl;
    cin >> CmdArgs::OutFile;
  }
  ofstream outfile = OpenOutFile(CmdArgs::OutFile);

  //open and read Lexicon
  string lexicon_path;
  cout << endl << "Provide Lexicon path:" << endl;
  cin >> lexicon_path;
  ifstream lexicon = OpenInFile(lexicon_path);
  map<string,double> Lexicon = ReadLexicon(lexicon);
  lexicon.close();

  //open and read input file
  ifstream data = OpenInFile(CmdArgs::InputFile);
  vector<Tweet*> Tweets = ReadDataset(data);
  cout << "Read input set of " << Tweets.size() << "tweets" << endl;
  data.close();

  //open and read cryptocurrrencies file
  string crypto_path;
  cout << endl << "Provide CryptoCurrencies path:" << endl;
  cin >> crypto_path;
  ifstream crypto = OpenInFile(crypto_path);
  map<string,string> CryptoNameMap = ReadCryptos(crypto);
  crypto.close();
  set<string> CryptoSet;  //set of all cryptos by their real name
  for(auto it=CryptoNameMap.begin(); it!=CryptoNameMap.end(); it++)
    CryptoSet.insert(it->second);
  SetCryptoOrder(CryptoSet);

  //open and read configuration file
  ReadConfigurationFile();

  //open and read vectorized tweets file
  string vtweets_path;
  cout << endl << "Provide vectorized tweets file path:" << endl;
  cin >> vtweets_path;
  ifstream vtweets = OpenInFile(vtweets_path);
  vector<myvector> VectorizedTweets = ReadVectorizedTweets(vtweets);
  vtweets.close();

  /**Vectorization of User sentiment by scoring their tweets and normalizing***/
  TweetScores(Tweets,Lexicon);  //Calc Tweet scores
  UserMap usermap;              //maps users by userid
  /*Assign each tweet to its user*/
  vector<User*> Users = GroupTweetsByUser(usermap,Tweets);


  //Using tweet scores calculate crypto values for each user
  for(auto user=Users.begin(); user!=Users.end(); user++)
    (*user)->CalcCryptoValues(CryptoNameMap);
  //Turning crypto_values into myvector type for each user
  vector<myvector*> UserVectors = VectorizeUsers(Users,CryptoSet);
  //keep a copy for each algorithm before tampering
  vector<User*> Users1B = CopyUsers(Users);
  vector<User*> Users2A = CopyUsers(Users);

  vector<HashTable*> LSH_Hashtables(CmdArgs::L);
  /***********************1.A***************************************************
  ***&Rate unknown cryptos using Cosine-LSH based on other users uj ************
  *****************************************************************************/
  cout<<endl<<"Rate unknown cryptos using Cosine-LSH based on  Nearest Neighbors...";
  //Initialize Hashtables
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables[i]=new HashTable(UserVectors,"cosine",CmdArgs::crypto_dimension,"lsh");
    //LSH_Hashtables[i]->PrintBuckets();
  }
  //Rate unknown crypto values by NN CosineSimilarity
  for(auto it=Users.begin(); it!=Users.end(); it++){
    if(*it == NULL) continue;
    (*it)->RateByNNSimilarity(LSH_Hashtables,CryptoSet);
  }
  cout << "Done." << endl;
  //cleanup
  for(auto it=LSH_Hashtables.begin(); it!=LSH_Hashtables.end(); it++) delete (*it);

  /**********************1.B****************************************************
  *********Rate unknown cryptos using Cosine-LSH based on tweet Clusters(cj)****
  *****************************************************************************/
  cout<<endl<<"Rate unknown cryptos using Cosine-LSH based on tweet Clusters..." << endl;
  //Initialize Hashtables for tweets
  vector<HashTable*> LSH_Hashtables_tweets(CmdArgs::L);
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables_tweets[i]
      =new HashTable(VectorizedTweets,"cosine",CmdArgs::tweet_dimension,"lsh");
    //LSH_Hashtables_tweets[i]->PrintBuckets();
  }
  //Create VectorizedTweet Clusters
  cout << endl << "\tClustering Vectorized Tweets...";
  ClusterSpace S(VectorizedTweets,"Random Init","RangeSearchLSH","K-means");
  S.RunClusteringAlgorithms(VectorizedTweets,LSH_Hashtables_tweets,NULL);
  cout << "\tClustering Done." << endl;
  //corelate vectorizedtweets to tweets by id
  map<string,Tweet*> tweet_id_map= MapTweetsById(Tweets);
  //now use tweet clusters to create virual users and calc their crypto values
  vector<User*> VirtualUsers = CreateVirtualUsers(S.getClusters(),tweet_id_map,CryptoNameMap);
  cout << VirtualUsers.size() << " VirtualUsers." << endl;
  //turning crypto_values into myvector type for each virtual user
  vector<myvector*> VirtualUserVectors = VectorizeUsers(VirtualUsers,CryptoSet);

  //Initialize Hashtables for VirtualUsers
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables[i]
    =new HashTable(VirtualUserVectors,"cosine",CmdArgs::crypto_dimension,"lsh");
    //LSH_Hashtables[i]->PrintBuckets();
  }
  //Rate unknown crypto values by NN CosineSimilarity
  for(auto it=Users1B.begin(); it!=Users1B.end(); it++){
    if(*it == NULL) continue;
    (*it)->RateByNNSimilarity(LSH_Hashtables,CryptoSet);
  }
  cout << "Done." << endl;
  WriteOutput(outfile,Users,Users1B,usermap,CryptoSet);

  //cleanup
  for(auto it=LSH_Hashtables_tweets.begin();it!=LSH_Hashtables_tweets.end();it++)
    delete (*it);
  for(auto it=LSH_Hashtables.begin();it!=LSH_Hashtables.end();it++)delete (*it);
  for(auto it=Users.begin(); it!=Users.end(); it++) delete (*it);
  for(auto it=Users1B.begin(); it!=Users1B.end(); it++) delete (*it);
  for(auto it=VirtualUsers.begin(); it!=VirtualUsers.end(); it++) delete (*it);

  /**********************2.A****************************************************
  ***********Clustering Users and rating by EuclideanSimilarity in cluster******
  *****************************************************************************/
  cout<<endl<<"2A Clustering Users and rating by EuclideanSimilarity in cluster"<< endl;
  CmdArgs::number_of_clusters = K;
  //transform users to myvectors
  vector<myvector> myUsers2A = UserToMyvector(Users2A);
  UserMap usermap2A = MapUsersByID(Users2A);
  //Initialize Hashtables for users
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables[i]=new HashTable(myUsers2A,"euclidean",CmdArgs::crypto_dimension,"lsh");
    //LSH_Hashtables_tweets[i]->PrintBuckets();
  }
  //Create User Clusters
  cout << endl << "\tClustering Vectorized Tweets...";
  ClusterSpace CS_2A(myUsers2A,"Random Init","RangeSearchLSH","K-means");
/*  CS_2A.RunClusteringAlgorithms(myUsers2A,LSH_Hashtables,NULL);
  cout << "\tClustering Done." << endl;
  //extract user clusters from myvector cluster space
  vector<vector<User*>> UserClusters = UserCluster(CS_2A.getClusters(),usermap2A);
  //Rate unknown crypto values by EuclideanSimilarity in each cluster
  for(int i=0; i<UserClusters.size(); i++){
    for(auto it=UserClusters[i].begin(); it!=UserClusters[i].end(); i++){
      if(*it == NULL) continue;
      (*it)->RateByClusterSimilarity(CS_2A.getCluster(i),CryptoSet);
    }
  }
*/

  cout << "Done." << endl;
  //cleanup
  for(auto it=Users2A.begin(); it!=Users2A.end(); it++) delete (*it);
  for(auto it=LSH_Hashtables.begin();it!=LSH_Hashtables.end();it++)delete (*it);

  //Cleanup
  for(auto it=Tweets.begin(); it!=Tweets.end(); it++) delete (*it);
  outfile.close();
  return OK;
}
