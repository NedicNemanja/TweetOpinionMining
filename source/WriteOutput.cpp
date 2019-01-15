#include "WriteOutput.hpp"
#include "ReadInput.hpp"

#include <iostream>

using namespace std;


void WriteOutput(std::ofstream & outfile,
                  vector<User*>& UsersU, //users with score from other users
                  vector<User*>& UsersC,  //users with score from clusters
                  map<string,User*>& usermap,
                  set<string>& CryptoSet
                )
{
  outfile << "Cosine LSH" << endl;
  for(auto it=UsersC.begin(); it!=UsersC.end(); it++){
    if(*it == NULL) continue;
    outfile << "<" << (*it)->getUserId() << ">";
    vector<string> top5 = usermap[(*it)->getUserId()]->getTopCryptos(5);
    vector<string> top2 = (*it)->getTopCryptos(2);
    for(int i=0; i<5; i++){
      if(i>=top5.size())
        outfile << "\t-";  //found less than 5
      else
        outfile << "\t" << top5[i];
    }
    for(int i=0; i<2; i++){
      if(i>=top2.size())
        outfile << "\t-";  //found less than 5
      else
        outfile << "\t" << top2[i];
    }
    outfile << endl;
  }
}
