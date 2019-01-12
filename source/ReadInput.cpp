#include "ReadInput.hpp"
#include "ErrorCodes.hpp"
#include "myvector.hpp"
#include "ParsingCSV.hpp"

#include <stdlib.h> //atoi
#include <string.h> //strcmp
#include <sstream>
#include <utility>  //std::move()
#include <ctype.h> //tolower()
      #include <unistd.h>

using namespace std;

namespace CmdArgs{
    std::string InputFile;
    std::string OutFile;
    int validate;
    int NUM_NN; //num of nearest neighbors (default 20)
    int dimension;
};

/*Parse cmd line arguments and read from cin any that are missing*/
void ParseArguments(int argc, char** argv){
  int arg_index=1;
  int Input_flag=0, Out_flag=0;

  //scan all arguements to find identifiers
  while(arg_index < argc){
    //make sure there is an argument to check
    if(argv[arg_index] == NULL)
      break;

    if(strcmp(argv[arg_index],"-d")==0 && Input_flag==0 && argv[arg_index+1]!=NULL){
      CmdArgs::InputFile.assign(argv[++arg_index]);
      Input_flag = 1;
      arg_index++;
      continue;
    }
    if(strcmp(argv[arg_index],"-o")==0 && Out_flag==0 && argv[arg_index+1]!=NULL){
      CmdArgs::OutFile.assign(argv[++arg_index]);
      Out_flag = 1;
      arg_index++;
      continue;
    }
    if(strcmp(argv[arg_index],"-validate")==0 && argv[arg_index+1]!=NULL){
      CmdArgs::validate = 1;
      arg_index++;
      continue;
    }
    fprintf(stderr, "Unknown argument: %s\n", argv[arg_index]);
    exit(UNKNOWN_CMDARGUMENT);
  }

  //check that you got all the arguements
  if(Input_flag == 0){
    cout << "Provide dataset path: ";
    cin >> CmdArgs::InputFile;
  }
  if(Out_flag == 0){
    cout << "Provide outfile path: ";
    cin >> CmdArgs::OutFile;
  }
}

map<string,double> ReadLexicon(ifstream &data){
  int num_pairs=0;
  map<string,double> Lexicon;
  //read row by row, row0 is key, row1 is value of the map
  CSVRow row;
  data >> row;
  while(!data.eof()){
    Lexicon.insert(pair<string,double>(row[0],stod(row[1])));
    //cout << row[0] << '\t'<< row[1] << endl;
    num_pairs++;
    data >> row;
  }
  //cout << "num_pairs:" << num_pairs << endl;
  return Lexicon;
}

MyVectorContainer ReadDataset(ifstream &data){
  cout << endl << "Reading dataset from disk..." << endl;
  //read coords from input and initialize vectors
  string userid, tweetid;
  vector<double> values;  //temp vector that gets overwritten every loop
  vector<myvector> vectors;
  while(GetVector(data, values, userid, tweetid)){
      vectors.push_back(myvector(values,tweetid));
  }
  return vectors;
}

//read coordinates of a vector and return true for success, else false
bool GetVector(ifstream &data,vector<double> &values, string &userid, string &tweetid){
  //read id
  CSVRow row;
  int row_index=0;
  data >> row;  //read a whole row
  if(data.eof()) return false;
  userid = row[row_index++];
  tweetid = row[row_index++];
  //read coords
  for(int i=0; i<row.size(); i++){
    //values.push_back(SentimentValue(row[row_index++]));
  }
  return true;
}

ifstream OpenInFile(string &filename){
  ifstream file(filename);
  if(!file.is_open()){
    cerr << "Couldn't open " << filename << endl;
    exit(FILE_ACCESS);
  }
  return file;
}

ofstream OpenOutFile(string &filename){
  ofstream file(filename);
  if(!file.is_open()){
    cerr << "Couldn't open " << filename << endl;
    exit(FILE_ACCESS);
  }
  return file;
}
