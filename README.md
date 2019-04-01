# TweetOpinionMining

Opining mining/Sentiment analysis of a Tweet vectorized dataset on the topic of cruptocurrencies.

Every Tweet is preprocessed as a set of tokens. Each token is checked again our Lexicon where, it carries a value in the range[-1,+1], if it does not exist in the Lexicon it is simply regarded as a neutral word and its value is 0.
A totalScore is calculated, for each Tweet, as following:
<a href="https://www.codecogs.com/eqnedit.php?latex=totalScore_i&space;=&space;\sum_{k=0}^{n}Lexicon.getValue(token_k)" target="_blank"><img src="https://latex.codecogs.com/gif.latex?totalScore_i&space;=&space;\sum_{k=0}^{n}Lexicon.getValue(token_k)" title="totalScore_i = \sum_{k=0}^{n}Lexicon.getValue(token_k)" /></a>

Each totalScore is normalised using this formula:

<a href="https://www.codecogs.com/eqnedit.php?latex=S_i&space;=&space;\frac{totalScore}{\sqrt{totalScore^2&plus;alpha}}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?S_i&space;=&space;\frac{totalScore}{\sqrt{totalScore^2&plus;alpha}}" title="S_i = \frac{totalScore}{\sqrt{totalScore^2+alpha}}" /></a>
