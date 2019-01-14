#ifndef COSINESIMILARITY_HPP
#define COSINESIMILARITY_HPP

#include <vector>
#include <cmath>

#include "myvector.hpp"
#include "Metric.hpp"
#include "utility.hpp"

class CosineSimilarity: public Metric{
    int dimension;
    std::vector<std::vector<double>> r_vectors;
  public:
    CosineSimilarity(int dim);
    ~CosineSimilarity();
    void SetRandRvectors(int d);
    unsigned int Hash(const myvector &p);
    int get_h(int i,const myvector &p);
    int dim();
    std::vector<long int> get_g(myvector &p);
    //cosine vector distance
    double vectorDistance(std::vector<coord>::const_iterator first,
                          std::vector<coord>::const_iterator last,
                          std::vector<coord>::const_iterator first2);
};

template<class Iter_T, class Iter2_T>
double CosineVectorDistance(Iter_T first, Iter_T last, Iter2_T first2) {
  double ip = inner_product(first, last, first2, 0);
  double norm1 = 0.0, norm2 = 0.0;
  while(first != last){
    norm1 += (*first)*(*first);  //sum of squares
    norm2 += (*first2)*(*first2);
    first++;
    first2++;
  }
  double divisor = sqrt(norm1)*(sqrt(norm2));
  if( divisor == 0)
    exit(UNDEFINED_COSINE_ZERO_VECTOR);
  double cos_similarity = ip/divisor;
  return 1 - cos_similarity;
};

template<class Iter_T, class Iter2_T>
double CosineVectorSimilarity(Iter_T first, Iter_T last, Iter2_T first2) {
  double ip = inner_product(first, last, first2, 0);
  double norm1 = 0.0, norm2 = 0.0;
  while(first != last){
    norm1 += (*first)*(*first);  //sum of squares
    norm2 += (*first2)*(*first2);
    first++;
    first2++;
  }
  double divisor = sqrt(norm1)*(sqrt(norm2));
  if( divisor == 0)
    exit(UNDEFINED_COSINE_ZERO_VECTOR);
  return ip/divisor;
};

#endif
