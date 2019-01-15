#ifndef READINPUT_HPP
#define READINPUT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "myvector.hpp"
#include "Tweet.hpp"

namespace CmdArgs{
    extern std::string InputFile;
    extern std::string OutFile;
    extern int validate;
    extern int NUM_NN;  //number of NearestNeighbors to look for
    //from project2
    extern std::string Metric;
    extern std::string ConfFile;
    extern int number_of_clusters,L,K;
    extern int RANGESEARCH_ITERATIONS;
    extern int HYPERCUBE_PROBES;
    extern int crypto_dimension;
    extern int tweet_dimension;
    extern double center_convergence_tolerance;
    extern int max_iterations;
};

/*Parse cmd line arguments and read from cin any that are missing*/
void ParseArguments(int argc, char** argv);

std::map<std::string,double> ReadLexicon(std::ifstream &data);

std::vector<Tweet*> ReadDataset(std::ifstream &data);

MyVectorContainer ReadVectorizedTweets(std::ifstream &data);

//read coordinates of a vector and return true for success, else false
bool GetVector(std::ifstream &data,std::vector<coord> &coords,std::string&,int);

//Check the first vector and find its dimension
int FindDimension(std::ifstream &data);

/*Returns a map that maps all cryptocurrency names/slangs to the
subsquent crypto name. In the file each line concerns a different cryptocurrency
and the 5th column contains its full/real name, if the 5th column is absent the
name present in the 1st column is used.*/
std::map<std::string,std::string> ReadCryptos(std::ifstream &data);

void ReadConfigurationFile();

std::ifstream OpenInFile(std::string &filename);
std::ofstream OpenOutFile(std::string &filename);


#endif
