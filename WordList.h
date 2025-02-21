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
	string mFileName;
	vector<vector<string>> mWords;//vector of vectors of words for given length
	vector<unordered_map<string,vector<int>>> mMasks;// one replacement masks of words
	vector<Graph> mEditDistanceOneGraph;
	bool editDistanceIsOne(string a, string b);
public:
	WordList();
	WordList(string name);
	bool load();
	vector<string> findTransform(string a, string b);
	vector<string> findRandomTransform();
	~WordList();
};
