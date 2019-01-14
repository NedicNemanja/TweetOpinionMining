#ifndef LSH_HPP
#define LSH_HPP

#include <vector>

#include "myvector.hpp"
#include "HashTable.hpp"

std::vector<std::pair<myvector,double>> NearestNeighbors(
                                            std::vector<HashTable*> &Hashtables,
                                            myvector &q);

#endif
