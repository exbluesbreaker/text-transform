# Text-transform
C++ implementation to find transform between 2 words using word list/vocabulary 

Given two words and a word list, find a shortest sequence of words from the word list
that turns the first input word into the second input word by changing one letter in
each step.

[Example of a word list (around 173K)](https://www.wordgamedictionary.com/enable/download/enable.txt)

[Another word list (around 467K)](https://github.com/dwyl/english-words/blob/master/words.txt)

# Example 

Transform word ***fleys*** into ***mania*** using the list above:

***fleys*** -> gleys -> gleds -> sleds -> seeds -> sends -> sands -> sandy -> candy -> canny -> canna -> manna -> ***mania***

# Algorithm

- To solve this task we store list of words combined by word length.
- Then we represent all words of the same length as a graph with nodes being words and edge being single character transform existing between words. 
E.g. ***gleds*** and ***sleds*** will have and edge between them (there exist 1 character transform between them), but ***gleds*** and ***seeds*** won't have an edge between them, because you cannot make one from the other by changing single character.
- To find all edges effectively we make all 1-replacement masks for a given word replacing single character by ***\****, so for ***seeds*** masks would be (***\*eeds,s\*eds,se\*ds,see\*s,seed\****).
We use hashtable with those masks as entries and we we add given word to vector corresponding to its masks.
- After building this hashtable for all word, every vector corresponding to the same mask have words with single character transform between them.
So we just need to go through all pairs of entries and add an edge between them.
- After this transform between 2 words from the word list can be done by doing shortest path search between 2 nodes (corresponding to words).
- It can be done by simple bread-first search algorithm, because there are no different weights for edges and shortest path will correspond to just shortest list of edges to connect, no need for Dijkstra algorithm etc.
- When search for transform for 2 words is being requested, if any of the words is not in the word list already it would be added to it with all corresponding data structures update before search.

# Main executable
- Asks for path to word list file, if entered path is not valid default **C:\Dev\enable.txt** would be used
- Offers 2 different options for transform search
    - user enter 2 words to find transform between
    - randomly chosen pair of same length words from the word list will be checked (random pair will be generated till transform could be successfully found)
# Source files
- main.cpp - entry point
- WordList.h/cpp - word list storage/processing unit
- Graph.h/cpp - graph/shortest path implementation
# Library interface
Use class WordList to access functionality.
- ***WordList::WordList()*** - default constructor
- ***WordList::WordList(const string &name)*** - constructor receiving file name
- ***WordList::setFileName(const string &name)*** - set file name
- ***WordList::load()*** - load words from the given word list file, take some time, should be called before any function below for reasonable result
- ***StringList WordList::findTransform(const string& a, const string& b)*** - find transform between 2 given words
- ***StringList WordList::findRandomTransform()*** - find transform between 2 randomly selected words of the same length
- ***void WordList::printMemoryUsage() const*** - print approximate memory consumption for the class (take only the main contributors into accout)
# Build process
From the folder of your choice (e.g. create ***build/*** in ***text_transform*** root directory) run:

`cmake <path to text_transform repo root>`

In case of running from ***build/*** in ***text_transform*** root directory:

`cmake ..`

This generate by default Visual Studio project/solution. To generate MinGW you can use:

`cmake -G "MinGW Makefiles" <path to text_transform repo root>`

Or in case of running from ***build/*** in ***text_transform*** root directory:

`cmake -G "MinGW Makefiles" ..`

After this you can either build Visual Studio project. Or in case of MinGW build run:

` mingw32-make.exe`
# Running
Just run ***TextTransform.exe*** and follow prompts
