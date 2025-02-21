#include "Graph.h"

Graph::Graph()
{
}

Graph::Graph(int numVertices)
{
	mAdjacencyList.resize(numVertices);
}

Graph::~Graph()
{
}
