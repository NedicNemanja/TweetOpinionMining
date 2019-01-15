#include <algorithm>
#include <float.h> //DBL_MAX

#include "LSH.hpp"
#include "utility.hpp"
#include "Metric.hpp"
#include "myvector.hpp"

using namespace std;

vector<myvector*> NearestNeighbors(vector<HashTable*> &Hashtables, myvector &q,
int num_of_nn){
  vector<pair<myvector*,double>> nns;
  double furthest_nn_dist = DBL_MAX;

  //for every Hashtable
  for(int i=0; i<Hashtables.size(); i++){
    //find corresponding bucket (and filter for g's in case of euclidean)
    Bucket bucket = Hashtables[i]->get_bucket_filtered(q);
    Metric* metric = Hashtables[i]->get_metric();
    //for each p in bucket
    for(Bucket::iterator p=bucket.begin(); p != bucket.end(); p++){
      if(q.get_id() == (*p)->get_id()) continue;  //user cant be selfneighbor
      double distance = metric->vectorDistance(q.begin(),q.end(),(*p)->begin());
      if(nns.size()<num_of_nn){
        /*there is free room for nns, just insert dont check distance*/
        nns.push_back(pair<myvector*,double>(*p,distance)); //insert element
        //reorder elements to keep heap property
        push_heap(nns.begin(), nns.end(), [](pair<myvector*,double>&a,
                  pair<myvector*,double>&b){return a.second<b.second;});
        if(distance < furthest_nn_dist){
          furthest_nn_dist = distance;
        }
      }
      else{
        if(distance < furthest_nn_dist){
          /*vector full we must discard the furthest neighbor*/
          pop_heap(nns.begin(),nns.end());  //reorder elements
          nns.pop_back();                   //remove max dist element
          //insert new neighbor
          nns.push_back(pair<myvector*,double>(*p,distance));
          //reorder elements
          push_heap(nns.begin(), nns.end(),[](pair<myvector*,double>&a,
                    pair<myvector*,double>&b){return a.second<b.second;});
          //update furthest_nn_dist
          furthest_nn_dist = distance;
        }
      }
    }
  }
  vector<myvector*> result(nns.size());
  transform(nns.begin(),nns.end(),result.begin(),
    [](pair<myvector*,double>&p) {return p.first;});
  return result;
}
