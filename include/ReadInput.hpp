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
    extern int NUM_NN;
    extern int dimension;
};

/*Parse cmd line arguments and read from cin any that are missing*/
void ParseArguments(int argc, char** argv);

std::map<std::string,double> ReadLexicon(std::ifstream &data);
std::vector<Tweet*> ReadDataset(std::ifstream &data);

std::ifstream OpenInFile(std::string &filename);
std::ofstream OpenOutFile(std::string &filename);


#endif
