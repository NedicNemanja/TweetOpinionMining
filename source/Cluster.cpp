#include "Cluster.hpp"
#include "utility.hpp"

using namespace std;

Cluster::Cluster(myvector &c)
:centroid(c),medoid(NULL){
}

//in case center is part of the dataset, save its index
Cluster::Cluster(myvector* m)
:medoid(m){
}

Cluster::~Cluster(){}

myvector Cluster::getCenter(){
  if(medoid == NULL)
    return centroid;
  return *medoid;
}

myvector* Cluster::getMedoid(){
  return medoid;
}

void Cluster::setMedoid(myvector* m){
  medoid = m;
}

void Cluster::setCentroid(myvector c){
  medoid = NULL;
  centroid = c;
}

std::vector<myvector*> Cluster::getMembers() const{
  return members;
}

int Cluster::size(){
  int cluster_size = members.size();
  if(medoid != NULL)
    cluster_size++;
  return cluster_size;
}

void Cluster::Print(std::ostream &out){
  for(auto it=members.begin(); it!=members.end(); it++){
    out << (*it)->get_id();
    if(it+1 != members.end())
       out << ",";
  }
}

void Cluster::AddVector(myvector* v){
  members.push_back(v);
}

/*Calc dissimilarity of v from cluster (sum of distances)*/
double Cluster::ClusterDissimilarity(myvector &v){
  double sum = 0;
  for(auto it=members.begin(); it!=members.end(); it++){
    sum += EuclideanVectorDistance(v.begin(),v.end(),(*it)->begin());
  }
  if(medoid != NULL)
   sum += EuclideanVectorDistance(v.begin(),v.end(),medoid->begin());
  return sum;
}

void Cluster::UnassignMembers(){
  members.clear();
}
