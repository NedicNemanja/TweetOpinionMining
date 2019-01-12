#include <iostream>
#include <fstream>
#include <time.h>

#include "ReadInput.hpp"
#include "ErrorCodes.hpp"
#include "Tweet.hpp"
#include "User.hpp"

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

  TweetScores(Tweets,Lexicon);  //Calc Tweet scores
  UserMap usermap;              //maps users by userid
  //assign each tweet to its user
  vector<User*> Users = GroupTweetsByUser(usermap,Tweets);
  //Using tweet scores calculate crypto values for each user
  for(auto user=Users.begin(); user!=Users.end(); user++)
    (*user)->CalcCryptoValues(CryptoNameMap);

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
