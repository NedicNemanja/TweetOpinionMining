#ifndef USER_HPP
#define USER_HPP

#include "Tweet.hpp"
#include "myvector.hpp"
#include "HashTable.hpp"
#include "Cluster.hpp"
#include "ClusterSpace.hpp"

#include <vector>
#include <string>
#include <map>
#include <set>

class User{
    std::string userid;
    std::vector<Tweet*> tweets;
    std::map<std::string,double> crypto_values;
    //vector holding sentiment toward cryptos
    myvector vector;
  public:
    User();
    User(std::string &uid);
    User(const User& user);
    ~User();
    void addTweet(Tweet* tweet);
    void print();
    std::vector<Tweet*> getTweets();
    std::string getUserId();
    std::vector<std::string> getTopCryptos(int n);
    myvector getVector() const;
    /*Take the score of every user's tweet and add it to the
    cryptocurrency/ies the tweet mentions*/
    void CalcCryptoValues(std::map<std::string,std::string> &CryptoNameMap);
    //Average value(sentiment) of user towards cryptocurrencies he mentions
    double AverageValue();
    /*Create myvector using crypto_values,ordered as in CryptoSet.
    If there are unknown crypto_values use average value.*/
    myvector* Vectorize(std::set<std::string> CryptoSet); 
    /*Calculate similarity to P NearetsNeighbors and set values of yet unknown
    cryptos according to NNs*/
    void RateByNNSimilarity(std::vector<HashTable*>&, std::set<std::string>&);
    void RateByClusterSimilarity(const Cluster&,std::set<std::string>&);
};

typedef std::map<std::string,User*> UserMap; //maps users by userid

/*Assign each tweet to its user and return all users (creates Users).
Ignore those that dont mention any crypto currencies.*/
std::vector<User*> GroupTweetsByUser(UserMap &usermap, std::vector<Tweet*> &);

std::vector<myvector*> VectorizeUsers(std::vector<User*>&,std::set<std::string>&);

//create virutal users and assign them the tweets from clusters
std::vector<User*> CreateVirtualUsers(const std::vector<Cluster>& Clusters,
                                      std::map<std::string,Tweet*>& tweetIDmap,
                                      std::map<std::string,std::string>&CrptoM);
//crate new User objects by contant copied from old Users
std::vector<User*> CopyUsers(std::vector<User*> &Users);

std::vector<myvector> UserToMyvector(std::vector<User*> Users);

std::vector<std::vector<User*>> UserCluster(ClusterSpace& CS);

/*From myvector* memebers of a cluster extact the userid
(which is set to be the id of the vector), and usering the usermap and userid as
the key find the user pointer. Mirroring the cluster of myvectors to User.*/
std::vector<std::vector<User*>> UserCluster(const std::vector<Cluster>&, UserMap&);

UserMap MapUsersByID(std::vector<User*>& Users);

#endif
