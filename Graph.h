#pragma once
#include <vector>

using std::vector;

struct Graph
{
public:
	vector<vector<int>> mAdjacencyList;// make public for simplicity
	Graph();
	Graph(int numVertices);
	inline void addEdge(int u, int v)
	{
		// For performance reasons don't do any additional checks
		// Caller should ensure that u and v are valid vertices
		mAdjacencyList[u].push_back(v);
		mAdjacencyList[v].push_back(u);
	};
	int getNumVertices() const { return mAdjacencyList.size(); }
	~Graph();
};
