#include "ReadInput.hpp"
#include "ErrorCodes.hpp"
#include "myvector.hpp"
#include "ParsingCSV.hpp"

#include <stdlib.h> //atoi
#include <string.h> //strcmp
#include <sstream>
#include <utility>  //std::move()
#include <ctype.h> //tolower()

using namespace std;

namespace CmdArgs{
    std::string InputFile;
    std::string OutFile;
    int validate;
    int NUM_NN; //num of nearest neighbors (default 20)
    //from project2
    std::string Metric;
    std::string ConfFile;
    int number_of_clusters,L,K;
    int RANGESEARCH_ITERATIONS;
    int HYPERCUBE_PROBES;
    int crypto_dimension;
    int tweet_dimension;
    double center_convergence_tolerance;
    int max_iterations;
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

vector<Tweet*> ReadDataset(ifstream &data){
  cout << endl << "Reading dataset from disk..." << endl;
  //read number of NearestNeighbors (P)
  string p;
  data >> p;
  if(p == "P:"){
    data >> CmdArgs::NUM_NN;
    string str;
    getline(data,str);
  }
  else
    CmdArgs::NUM_NN = 20;

  //read tweets
  std::vector<Tweet*> Tweets;
  Tweet* tweet;
  while((tweet=GetTweet(data)) != NULL){
    Tweets.push_back(tweet);
  }
  return Tweets;
}

MyVectorContainer ReadVectorizedTweets(ifstream &data){
  char prev_delimeter = DELIMETER;
  DELIMETER = ',';
  CmdArgs::tweet_dimension = FindDimension(data);
  cout << endl << "Reading dataset from disk..." << endl;
  //read coords from input and initialize vectors
  string id;
  vector<coord> coords(CmdArgs::tweet_dimension);
  vector<myvector> vectors;
  while(GetVector(data, coords, id, CmdArgs::tweet_dimension)){
      vectors.push_back(myvector(coords,id));
  }
  DELIMETER = prev_delimeter;
  return vectors;
}

//read coordinates of a vector and return true for success, else false
bool GetVector(ifstream &data,vector<coord> &coords, string &id, int dim){
  //read id
  CSVRow row;
  int row_index=0;
  data >> row;
  if(data.eof()) return false;
  id = row[row_index++];
  //read coords
  for(int i=0; i<dim; i++){
    coords.at(i) = stod(row[row_index++]);
  }
  return true;
}

//Check the first vector and find its dimension
int FindDimension(ifstream &data){
  int dimension=0;
  streampos oldpos = data.tellg();  // stores the stream position
  string line;
  getline(data,line);               //get the whole vector
  istringstream is(line);         //treat line like a stream
  string id;                      //skip first string, that's the id
  is >> id;
  coord n;
  while( is >> n ) {                //count coords in line
    dimension++;
  }
  data.seekg (oldpos);   // get back to the position
  return dimension;
}

/*Returns a map that maps all cryptocurrency names/slangs to the
subsquent crypto name. In the file each line concerns a different cryptocurrency
and the 5th column contains its full/real name, if the 5th column is absent the
name present in the 1st column is used.*/
map<string,string> ReadCryptos(ifstream &data){
  int num_crypto=0;
  map<string,string> CryptoNameMap;
  //read row by row
  CSVRow row;
  data >> row;
  while(!data.eof()){
    num_crypto++;
    //find cryptos real name
    string real_name;
    if(row.size() < 5)
      real_name = row[0];
    else
      real_name = row[4];
    //for every name
    for(int i=0; i<row.size(); i++){
      CryptoNameMap.insert(pair<string,string>(row[i],real_name));
      //cout << row[i] << '\t'<< real_name << endl;
    }
    data >> row;
  }
  cout << "Found " << num_crypto << " different cryptocurrrencies." << endl;
  CmdArgs::crypto_dimension = num_crypto;
  return CryptoNameMap;
}


void ReadConfigurationFile(){
  string filename;
  cout << endl << "Provide .conf path: " << endl;
  cin >> filename;

  int clusters_flag=0, hash_func_flag=0, hashtables_flag=0, dimension_flag=0,
  hypercube_probes_flag=0, rangesearch_iterations_flag=0,cc_tolerance_flag=0,
  max_iterations_flag=0;
  ifstream file = OpenInFile(filename);
  string field_name;
  while(!file.eof()){
    file >> field_name;
    if(field_name == "number_of_clusters:"){
      if(!file.eof()){
        file >> CmdArgs::number_of_clusters;
        clusters_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(field_name == "number_of_hash_functions:"){
      if(!file.eof()){
        file >> CmdArgs::K;
        hash_func_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(field_name == "number_of_hashtables:"){
      if(!file.eof()){
        file >> CmdArgs::L;
        hashtables_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(field_name == "rangesearch_iterations:"){
      if(!file.eof()){
        file >> CmdArgs::RANGESEARCH_ITERATIONS;
        rangesearch_iterations_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(field_name == "hypercube_probes:"){
      if(!file.eof()){
        file >> CmdArgs::HYPERCUBE_PROBES;
        hypercube_probes_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(field_name == "center_convergence_tolerance:"){
      if(!file.eof()){
        file >> CmdArgs::center_convergence_tolerance;
        cc_tolerance_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(field_name == "max_iterations:"){
      if(!file.eof()){
        file >> CmdArgs::max_iterations;
        max_iterations_flag=1;
        field_name.clear();
        continue;
      }
    }
    if(!file.eof()){
      cerr << "Unknown argument in .conf: -" << field_name <<"-" << endl;
      exit(UNKNOWN_CMDARGUMENT);
    }
  }
  file.close();

  //check that all is set, if not use default values
  if(clusters_flag == 0){
    cout << "Default number_of_clusters: 3" << endl;
    CmdArgs::number_of_clusters = 3;
  }
  else
    cout << "number_of_clusters: " << CmdArgs::number_of_clusters << endl;

  if(hash_func_flag == 0 ){
    cout << "Default number_of_hash_functions (K): 4" << endl;
    CmdArgs::K=4;
  }
  else
    cout << "number_of_hash_functions (K): " << CmdArgs::K << endl;

  if(hashtables_flag == 0){
    cout << "Default number_of_hashtables (L): 5" << endl;
    CmdArgs::L=5;
  }
  else
    cout << "number_of_hashtables (L): " << CmdArgs::L << endl;


  if(hypercube_probes_flag==0 || max_iterations_flag==0 ||
    rangesearch_iterations_flag==0 || cc_tolerance_flag==0){
      cout << "Badness in Config file" << endl;
      exit(BAD_CONFIG_FILE);
  }
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
