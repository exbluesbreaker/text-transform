# text-transform
C++ implementation to find transform between 2 words using word list/vocabulary 

Given two words and a word list, find a shortest sequence of words from the word list
that turns the first input word into the second input word by changing one letter in
each step.

[Example of a word list (around 173K)](https://www.wordgamedictionary.com/enable/download/enable.txt)

[Another word list (around 467K)](https://github.com/dwyl/english-words/blob/master/words.txt)

# example 

Transform word ***fleys*** into ***mania*** using the list above:

***fleys*** -> fleas -> flees -> flews -> clews -> plews -> slews -> sleds -> seeds -> deeds -> feeds -> heeds -> meeds -> needs -> reeds -> weeds -> welds -> wends -> wands -> bands -> bandy -> candy -> canny -> canna -> manna -> ***mania***
