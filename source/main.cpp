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

using namespace std;

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
  /*Using tweet scores calculate crypto values for each user*/
  for(auto user=Users.begin(); user!=Users.end(); user++)
    (*user)->CalcCryptoValues(CryptoNameMap);
  /*Turning crypto_values into myvector type for each user*/
  vector<myvector*> UserVectors = VectorizeUsers(Users,CryptoSet);

  vector<HashTable*> LSH_Hashtables(CmdArgs::L);
  /**********Rate unknown cryptos using Nearest Neighbors with LSH*************/
  cout << endl << "Rate unknown cryptos using Nearest Neighbors with LSH...";
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
  /***********Rate unknown cryptos using Clusters******************************/
  //Initialize Hashtables
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables[i]=new HashTable(VectorizedTweets,"euclidean",
                                    CmdArgs::tweet_dimension,"lsh");
    //LSH_Hashtables[i]->PrintBuckets();
  }
  //Create Tweet Clusters
  cout << endl << "Clustering Vectorized Tweets...";
  ClusterSpace S(VectorizedTweets,"Random Init","RangeSearchLSH","K-means");
  S.RunClusteringAlgorithms(VectorizedTweets,LSH_Hashtables,NULL);
  cout << "Done." << endl;
  //corelate vectorizedtweet-tweet by id
  map<string,Tweet*> tweet_id_map= MapTweetsById(Tweets);

  //now use tweet clusters to create virualusers with cj vectors whose crypto_values are sum of tweet sentiment towards cryptos
  vector<User*> VirtualUsers = CreateVirtualUsers(S.getClusters(),tweet_id_map);

  //cleanup
  for(auto it=LSH_Hashtables.begin(); it!=LSH_Hashtables.end(); it++) delete (*it);
  for(auto it=VirtualUsers.begin(); it!=VirtualUsers.end(); it++) delete (*it);


  //Cleanup
  for(auto it=Tweets.begin(); it!=Tweets.end(); it++) delete (*it);
  for(auto it=Users.begin(); it!=Users.end(); it++) delete (*it);

  return OK;
/*
  //Initialize Hashtables
  vector<HashTable*> LSH_Hashtables(CmdArgs::L);
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables[i]=new HashTable(AllVectors,CmdArgs::Metric,CmdArgs::dimension,"lsh");
    //LSH_Hashtables[i]->PrintBuckets();
  }
  HashTable HypercubeHashtable(AllVectors,CmdArgs::Metric,CmdArgs::dimension,"hypercube");
  //HypercubeHashtable.PrintBuckets();
  //open outfile
  if(CmdArgs::OutFile.empty()){
    cout << endl << "Provide outfile path:" << endl;
    cin >> CmdArgs::OutFile;
  }
  ofstream outfile = OpenOutFile(CmdArgs::OutFile);
  /****************CLUSTERING**************************************************/
/*  for(int i=0; i<12; i++){
    //if(i%2 == 1) continue; //skip PAM
    string init,assign,update;
    int code = SetAlgorithmChoices(i,init,assign,update);
    double clustering_time;
    struct timespec start,end;
    clock_gettime(CLOCK_MONOTONIC,&start);
    //Initialize
    ClusterSpace S(AllVectors,init,assign,update);
    //Assign and Update
    S.RunClusteringAlgorithms(AllVectors,LSH_Hashtables,&HypercubeHashtable);
    //calculate time and write results to outfile
    clock_gettime(CLOCK_MONOTONIC,&end);
    double t=(end.tv_sec-start.tv_sec + (end.tv_nsec-start.tv_nsec)/1000000000.0);
    cout << "\t" << "clustering_time:" << t << "sec" << endl;
    WriteResult(outfile, code, S, t);
  }
  //cleanup
  for(int i=0; i<CmdArgs::L; i++){
    delete LSH_Hashtables[i];
  }
  outfile.close();
*/
}
