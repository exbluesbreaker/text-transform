#pragma once
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::vector;
using std::queue;
using std::string;

struct Graph
{
private:
	size_t mNumVertices = 0;
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
	inline void addNode(int node_id)
	{
		// Basically, it just resized the adjacency list if needed
		if (node_id >= mNumVertices)
		{
			mNumVertices = node_id + 1;
			mAdjacencyList.resize(node_id + 1);
		}
	};
	int getNumVertices() const { return mNumVertices; }
	// Find the shortest path between two vertices if it exists
	// Return it as a list of vertices 
	vector<int> getShortestPath(size_t u, size_t v);
	size_t getMemoryUsage() const;
	~Graph();
};
