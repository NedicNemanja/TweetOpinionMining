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

using namespace std;

int main(int argc, char** argv){
  /*****************PREPROCESSING*************************************/
  ParseArguments(argc,argv);
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

  /************Vectorization of user sentiment towards CryptoCurrencies********/
  TweetScores(Tweets,Lexicon);  //Calc Tweet scores
  UserMap usermap;              //maps users by userid
  /*Assign each tweet to its user*/
  vector<User*> Users = GroupTweetsByUser(usermap,Tweets);
  /*Using tweet scores calculate crypto values for each user*/
  for(auto user=Users.begin(); user!=Users.end(); user++)
    (*user)->CalcCryptoValues(CryptoNameMap);
  /*Turning crypto_values into myvector type for each user*/
  vector<myvector> Vectors;
  for(auto it=Users.begin(); it!=Users.end(); it++){
    myvector vector = (*it)->Vectorize(CryptoSet);
    if(vector.size() != 0){
      //ignore vectors that didn't mention any crypto
      Vectors.push_back(vector);
    }
  }
  cout << Vectors.size() << " Vectors refering to cryptocurrencies." << endl;

  /**********Finding Nearest Neighbors using LSH*******************************/
  //Initialize Hashtables
  vector<HashTable*> LSH_Hashtables(CmdArgs::L);
  for(int i=0; i<CmdArgs::L; i++){
    LSH_Hashtables[i]=new HashTable(Vectors,"cosine",CmdArgs::dimension,"lsh");
    //LSH_Hashtables[i]->PrintBuckets();
  }
  

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
  return OK;
}
