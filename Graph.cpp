#include "Graph.h"

Graph::Graph()
{
}

Graph::Graph(int numVertices) : mNumVertices(numVertices), mAdjacencyList(numVertices)
{
}

vector<int> Graph::getShortestPath(size_t u, size_t v)
{
	if (u >= mNumVertices || v >= mNumVertices)
	{
		return vector<int>();
	}
	// Use Bread-first search to find the shortest path
	vector<bool> visited(mNumVertices, false); // Marker if vertex was visited in the process
	/*
	* Use queue (FIFO) to traverse through vertices so we would visit neighbors before farther vertices 
	* It would guarantee that we find the shortest path if it exists
	*/
	queue<int> q; 
	vector<int> parent(mNumVertices, -1); // Parent of each vertex in the shortest path (if there is a shortest path to this vertex)
	q.push(u);
	visited[u] = true;
	while (!q.empty()) {
		int current = q.front();
		q.pop();

		// If we reach the target node, stop early
		if (current == v) {
			break;
		}

		// Explore neighbors
		auto& adj_list = mAdjacencyList[current];
		for (int neighbor : adj_list) {
			if (!visited[neighbor]) {
				visited[neighbor] = true;
				parent[neighbor] = current;// Record the parent to reconstruct the path
				q.push(neighbor);
			}
		}
	}
	if (!visited[v]) {
		return vector<int>(); // No path found if we didn't visit the target node in the process
	}
	vector<int> path;
	// Reconstruct the path from the parent array which will be in reverse order
	for (int at = v; at != -1; at = parent[at]) {
		path.push_back(at);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

size_t Graph::getMemoryUsage() const
{
	// just estimate adjacency list sizes for existing nodes
	size_t size = 0;
	for (size_t i = 0; i < mNumVertices; ++i)
	{
		// vector consumes more memory than it actually uses by design
		size += mAdjacencyList[i].capacity()*sizeof(int);
	}
	return size;
}

Graph::~Graph()
{
}
