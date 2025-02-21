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
	void addWord(string word);
	WordMasks getMasks(size_t len);
public:
	WordList();
	WordList(string name);
	bool load();
	vector<string> findTransform(string a, string b);
	vector<string> findRandomTransform();
	~WordList();
};
