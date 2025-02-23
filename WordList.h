#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <random>
#include "Graph.h"



using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::queue;

using WordMasks = unordered_map<string, vector<size_t>>;
using StringList = vector<string>;

class WordList
{
private:
	size_t mNumWords = 0;
	string mFileName;
	vector<StringList> mWords;// index 0 - words of length 1, 1 - length 2 and so on
	vector<WordMasks> mMasks;//vector of maps of masks to word indices to detect words with edit distance 1
	vector<Graph> mEditDistanceOneGraph;//vector of graphs for words of different lengths
	size_t addWord(const string& word);//add word to the list and return its index
	void addWordToGraph(const string& word);//add word to the graph, relies on addWord being called previously
public:
	WordList();
	WordList(const string &name);
	void setFileName(const string &name) { mFileName = name; }
	bool load();// load words from the file
	StringList findTransform(const string& a, const string& b);//find a transform between two words of the same length
	StringList findRandomTransform();//find a random transform between two words of the same length (good for testing)
	void printMemoryUsage() const;
	~WordList();
};
