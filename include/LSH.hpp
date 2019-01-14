#ifndef LSH_HPP
#define LSH_HPP

#include <vector>

#include "myvector.hpp"
#include "HashTable.hpp"

std::vector<myvector*> NearestNeighbors(
                                            std::vector<HashTable*> &Hashtables,
                                            myvector &q,int num_of_nn);

#endif
