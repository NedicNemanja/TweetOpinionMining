#ifndef WRITEOUTPUT_HPP
#define WRITEOUTPUT_HPP

#include <fstream>
#include <list>

#include "myvector.hpp"
#include "ClusterSpace.hpp"
#include "User.hpp"

void WriteOutput(std::ofstream&,
                std::vector<User*>& UsersU, //users with score from other users
                std::vector<User*>& UsersC,  //users with score from clusters
                std::map<std::string,User*>& usermap,
                std::set<std::string>& CryptoSet);

#endif
