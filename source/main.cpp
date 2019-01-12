#include <iostream>
#include <fstream>
#include <time.h>
#include <map>

#include "ReadInput.hpp"
#include "ErrorCodes.hpp"

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
return 0;
  //open and read input file
  ifstream data = OpenInFile(CmdArgs::InputFile);
  AllVectors = ReadDataset(data);
  cout << "Read input set of " << AllVectors.size() << "vectors" << endl;
  data.close();

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
