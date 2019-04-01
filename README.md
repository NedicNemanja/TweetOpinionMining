# TweetOpinionMining

```
Opining mining/Sentiment analysis of a Tweet vectorized dataset on the topic of cruptocurrencies. The goal is to be able to predict User sentiment towards cruptocurrencies which they have never mentioned. That is achieved using 2 distinct methods discussed below.
```


Every Tweet is preprocessed as a set of tokens.
Each token is checked again our Lexicon where, it carries a value in the range[-1,+1]. If it does not exist in the Lexicon it is simply regarded as a neutral word and its value is 0.

A totalScore is calculated, for each Tweet, as following:

<a href="https://www.codecogs.com/eqnedit.php?latex=totalScore_i&space;=&space;\sum_{k=0}^{n}Lexicon.value(token_k)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?totalScore_i&space;=&space;\sum_{k=0}^{n}Lexicon.value(token_k)" title="totalScore_i = \sum_{k=0}^{n}Lexicon.value(token_k)" /></a>

Each totalScore is normalised using this formula:

<a href="https://www.codecogs.com/eqnedit.php?latex=S_i&space;=&space;\frac{totalScore}{\sqrt{totalScore^2&plus;alpha}}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?S_i&space;=&space;\frac{totalScore}{\sqrt{totalScore^2&plus;alpha}}" title="S_i = \frac{totalScore}{\sqrt{totalScore^2+alpha}}" /></a>
where alpha=15 by default.

Another Lexicon exist containing all the cryptocurrencies an their relevant names, abbrievations etc.


Each user holds a sentiment value towards the cruptocurrencies he/she has mentioned in a Tweet. 
For example: Whenever a User mentions "bitcoin" in a Tweet, that Tweet's totalScore get added to the bitcoin sentiment value of that user. If that user also mentions ethernum then the totalScore also get added to ethernum sentiment value etc.

# Rate unknown cryptos using Nearest Neighbor (CosineSimilarity+LocalitySensitiveHashing) based on other users

Find NUM_NN (you can edit this variable) Nearest Neighbors. Calculate the similarity betwen the target-user and each NN using Cosine Similarity. Use the following formula to calculate the sentiment value of cryptos that are unknown(has not mentioned) to the user:


<a href="https://www.codecogs.com/eqnedit.php?latex=targetUser\_sentimentValueOfCrypto=\sum^{num_NNs}NNeighbor.sentimentValue(crypto)*CosineSimilarity(targetUser,NN)/num_NNs" target="_blank"><img src="https://latex.codecogs.com/gif.latex?targetUser\_sentimentValueOfCrypto=\sum^{num_NNs}NNeighbor.sentimentValue(crypto)*CosineSimilarity(targetUser,NN)/num_NNs" title="targetUser\_sentimentValueOfCrypto=\sum^{num_NNs}NNeighbor.sentimentValue(crypto)*CosineSimilarity(targetUser,NN)/num_NNs" /></a>

The general gist of the formula is that we extract the average sentiment value for each unkown crypto of our target_user using the sum of sentiment values that his Nearest Neighbors had for that crypto, but adjusted/weighted by the similarity between our target_user and each of his NearestNeighbors.

# Rate unknown cryptos using Cosine-LSH based on tweet Clusters

This time we create Tweet clusters (using ClusteringAlgorithms from my previous project). For each cluster we will create a Virutal User.
Now we repeat the previously described NearestNeighbor sentiment analysis process for the Virtual Users instead of all the Users.
The point here being that a Virtual User is much more representative of the average user a thus a more stable are reliable metric.

# Results

The results are written in an output file. For each method the users are enumerated. One user per line. The user's name is the first string, the next 5 represent the top5 cryptos that are recommended for the user using only the NearestNeighbor method and the last 2 represent the top2 cryptos recommended for the user using the Clustering Method.

This project was done as a task for a University project. Although I learned a lot, I wish I had more time to dedicate to it. There is much more that could've been done such as implementing and testing more method based on User Clustering instead of tweet clustering and better evaluation for each method using 10-fold cross-validation.
