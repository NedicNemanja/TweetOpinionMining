#ifndef READINPUT_HPP
#define READINPUT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include "myvector.hpp"

namespace CmdArgs{
    extern std::string InputFile;
    extern std::string OutFile;
    extern int validate;
    extern int NUM_NN;
    extern int dimension;
};

/*Parse cmd line arguments and read from cin any that are missing*/
void ParseArguments(int argc, char** argv);

std::map<std::string,double> ReadLexicon(std::ifstream &data);
MyVectorContainer ReadDataset(std::ifstream &data);

//read coordinates+id of a myvector and return true for success, else false
bool GetVector(std::ifstream&,std::vector<double>&,std::string&,std::string&);

std::ifstream OpenInFile(std::string &filename);
std::ofstream OpenOutFile(std::string &filename);


#endif
