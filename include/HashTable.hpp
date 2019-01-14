#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <list>
#include <vector>
#include <map>

#include "myvector.hpp"
#include "Metric.hpp"

typedef std::vector<myvector*> Bucket;

class HashTable
{
    std::string tabletype;  //lsh or hypercube
    std::vector<Bucket> buckets;
    Metric* metric;
  public:
    HashTable(MyVectorContainer&,std::string,int dim, std::string);
    HashTable(MyVectorContainer&,std::string,int dim,int tsize,std::string);
    HashTable(int size, std::string metric_name, int dim, std::string);
    ~HashTable();
    Bucket get_bucket(myvector& v);
    int num_buckets();
    /*get bucket, but filter out the vectors with different g()'s*/
    Bucket get_bucket_filtered(MyVectorContainer &vectors, myvector &q);
    /*Find in which bucket q should belong
    and filter out any vectors with a different g*/
    Bucket get_bucket_filtered(myvector& q);
    /*Compare metric hashes of two vectors, true if same.*/
    bool CompareHashes(myvector& q, myvector& p);
    /*get bucket at pos*/
    Bucket get_bucket_at(int pos);
    Metric* get_metric();
    /*Get the hash value for p*/
    int get_hash(const myvector &p);
    void Insert(myvector* v);
    void InsertVector(MyVectorContainer &vectors);
    void InitBuckets(int size);
    void PrintBuckets();
    /*Return all index to vectors of the bucket that are in the range of center*/
    Bucket RangeSearch(int b, myvector center,double radius,
      MyVectorContainer&vectors,std::map<std::string,bool>&AssignedVectorBitMap);
};

unsigned int TableSize(std::string tabletype,std::string metric,int numvectors);

#endif
