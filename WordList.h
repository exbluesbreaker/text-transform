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



using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using std::queue;

class WordList
{
private:
	using WordMasks = unordered_map<string, vector<int>>;
	size_t mNumWords = 0;
	string mFileName;
	vector<vector<string>> mWords;//vector of vectors of words for given length
	vector<Graph> mEditDistanceOneGraph;
public:
	WordList();
	WordList(string name);
	bool load();
	void addWord(string word);
	void addMasks(string word, size_t word_id, WordMasks& masks);
	vector<string> findTransform(string a, string b);
	vector<string> findRandomTransform();
	~WordList();
};
