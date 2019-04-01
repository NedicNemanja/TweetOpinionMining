# TweetOpinionMining

Opining mining/Sentiment analysis of a Tweet vectorized dataset on the topic of cruptocurrencies. The goal is to be able to predict User sentiment towards cruptocurrencies which they have never mentioned. That is achieved using 2 distinct methods:


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

#Rate unknown cryptos using Nearest Neighbor (CosineSimilarity+LocalitySensitiveHashing) based on other users
Find NUM_NN (you can edit this variable) Nearest Neighbors. Calculate the similarity betwen the target-user and each NN using Cosine Similarity. Use the following formula to calculate the sentiment value of cryptos that are unknown(has not mentioned) to the user:


<a href="https://www.codecogs.com/eqnedit.php?latex=targetUser\_sentimentValueOfCrypto=\sum^{NNs}NN.sentimentValue(crypto)*CosineSimilarity(targetUser,NN)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?targetUser\_sentimentValueOfCrypto=\sum^{NNs}NN.sentimentValue(crypto)*CosineSimilarity(targetUser,NN)" title="targetUser\_sentimentValueOfCrypto=\sum^{NNs}NN.sentimentValue(crypto)*CosineSimilarity(targetUser,NN)" /></a>


Using a previous project of mine (ClusteringAlgorithms) in order to construct 
