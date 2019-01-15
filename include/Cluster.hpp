#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "myvector.hpp"
#include "Metric.hpp"

#include <vector>
#include <iostream>

class Cluster{
    myvector centroid;
    myvector* medoid; //-1 if center is not part of the dataset
    //points that belong to the cluster, excluding centers
    std::vector<myvector*> members;
  public:
    Cluster(myvector &center);
    //in case center is part of the dataset, save its index
    Cluster(myvector* c_index);
    ~Cluster();
    myvector getCenter();//get the vector that used as center of the cluster
    myvector* getMedoid();
    void setMedoid(myvector*);
    void setCentroid(myvector);
    myvector* setMedoid();
    std::vector<myvector*> getMembers() const;
    void Print(std::ostream &out);
    int size();
    void AddVector(myvector* vindex);
    /*Calc dissimilarity of v from cluster*/
    double ClusterDissimilarity(myvector &v);
    //clear members vector
    void UnassignMembers();
};

#endif
