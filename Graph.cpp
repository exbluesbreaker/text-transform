#include "Graph.h"

Graph::Graph()
{
}

Graph::Graph(int numVertices) : mNumVertices(numVertices), mAdjacencyList(numVertices)
{
}

vector<int> Graph::getShortestPath(int u, int v)
{
	// Use Bread-first search to find the shortest path
	vector<bool> visited(mNumVertices, false); // Marker if vertex was visited in the process
	queue<int> q; // Use queue (FIFO) to traverse through vertices so we would visit neighbors before farther vertices
	vector<int> parent(mNumVertices, -1); // Parent of each vertex in the shortest path (if there is a shortest path to this vertex)
	q.push(u);
	while (!q.empty()) {
		int current = q.front();
		q.pop();
		visited[current] = true;

		// If we reach the target node, stop early
		if (current == v) {
			break;
		}

		// Explore neighbors
		auto& adj_list = mAdjacencyList;
		for (int neighbor : adj_list[current]) {
			if (!visited[neighbor]) {
				parent[neighbor] = current; // Record the parent to reconstruct the path
				q.push(neighbor);
			}
		}
	}
	if (!visited[v]) {
		return vector<int>(); // No path found if we didn't visit the target node in the process
	}
	vector<int> path;
	for (int at = v; at != -1; at = parent[at]) {
		path.push_back(at);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

Graph::~Graph()
{
}
