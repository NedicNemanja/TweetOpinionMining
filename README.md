# TweetOpinionMining

Opining mining/Sentiment analysis of a Tweet vectorized dataset on the topic of cruptocurrencies.

Every Tweet is preprocessed as a set of tokens. Each token is checked again our Lexicon where, if it exists, it carries a value in the range[-1,+1]. A totalScore 

![equation](<a href="https://www.codecogs.com/eqnedit.php?latex=S_i&space;=&space;\frac{totalScore}{\sqrt{totalScore^2&plus;alpha}}" target="_blank"><img src="https://latex.codecogs.com/gif.latex?S_i&space;=&space;\frac{totalScore}{\sqrt{totalScore^2&plus;alpha}}" title="S_i = \frac{totalScore}{\sqrt{totalScore^2+alpha}}" /></a>)
