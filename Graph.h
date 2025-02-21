#pragma once
#include <vector>
#include <queue>

using std::vector;
using std::queue;

struct Graph
{
private:
	size_t mNumVertices;
	vector<vector<int>> mAdjacencyList;
public:
	Graph();
	Graph(int numVertices);
	inline void addEdge(int u, int v)
	{
		// For performance reasons don't do any additional checks
		// Caller should ensure that u and v are valid vertices
		mAdjacencyList[u].push_back(v);
		mAdjacencyList[v].push_back(u);
	};
	int getNumVertices() const { return mNumVertices; }
	// Find the shortest path between two vertices as a list of vertices, if it exists
	vector<int> getShortestPath(int u, int v);
	~Graph();
};
