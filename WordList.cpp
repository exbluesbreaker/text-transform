#include "WordList.h"

bool WordList::editDistanceIsOne(string a, string b)
{
	// Ignore length check for faster code, caller should check lengths
	// Also we assume that a and b are not equal so we don't check for that
	int len = a.size();
	int diff = 0;
	for (int i = 0; i < len; i++)
	{
		if (a[i] != b[i])
		{
			diff++;
			if (diff > 1)
				return false;
		}
	}
	return true;
}

WordList::WordList()
{
}

WordList::WordList(string name) :mFileName(name)
{
}

bool WordList::load()
{
	std::ifstream file(mFileName);
	if (!file) 
	{
		// Simple error handling without exceptions
		std::cerr << "Error opening file: " << mFileName << std::endl;
		return false;
	}
	std::string line;
	int num_words = 0;
	while (std::getline(file, line)) 
	{
		int len = line.size();
		if (len > mWords.size()) 
		{
			mWords.resize(len);
			mMasks.resize(len);
		}
		num_words++;
		mWords[len-1].push_back(line);
		for (size_t i = 0; i < line.size(); ++i) {
			// Generate a mask by replacing the i-th character with '*'
			std::string mask = line;
			mask[i] = '*';
			// Store the word index in the vector of words with the same mask
			mMasks[len - 1][mask].push_back(mWords[len - 1].size()-1);
		}
	}
	file.close();
	std::cout << "Loaded " << num_words << " words from " << mFileName << std::endl;
	for (int i = 0; i < mWords.size(); i++)
	{
		std::cout << "Words of length " << i + 1 << ": " << mWords[i].size() << std::endl;
	}
	// Create the graph for edit distance 1
	for (int i = 0; i < mWords.size(); i++)
	{
		mEditDistanceOneGraph.push_back(Graph(mWords[i].size()));
		struct pair_hash {
			inline std::size_t operator()(const std::pair<int, int>& v) const {
				return v.first * 31 + v.second;
			}
		};
		unordered_set<std::pair<int,int>, pair_hash> seenPairs;// To avoid duplicates
		for (const auto& [mask, wordIds] : mMasks[i])
		{
			for (size_t j = 0; j < wordIds.size(); ++j)
			{
				for (size_t k = j + 1; k < wordIds.size(); ++k)
				{
					pair<int, int> new_pair;
					if (wordIds[j] < wordIds[k])
					{
						new_pair = std::make_pair(wordIds[j], wordIds[k]);
					}
					else
					{
						new_pair = std::make_pair(wordIds[k], wordIds[j]);
					}
					if (!seenPairs.contains(new_pair))
					{
						mEditDistanceOneGraph[i].addEdge(new_pair.first, new_pair.second);
						seenPairs.insert(new_pair);
					}
				}
			}
		}
	}
	return true;
}

vector<string> WordList::findTransform(string a, string b)
{
	if (a.size() != b.size())
	{
		std::cerr << "Words must be of the same length" << std::endl;
		return vector<string>();
	}
	int a_id = -1;
	int b_id = -1;
	int word_len_id = a.size()-1;
	for (int i = 0; i< mWords[word_len_id].size(); i++)
	{
		auto &word = mWords[word_len_id][i];
		if (word == a)
			a_id = i;
		if (word == b)
			b_id = i;
	}
	if (a_id == -1 || b_id == -1)
	{
		std::cerr << "Words not found in the list" << std::endl;
		return vector<string>();
	}
	// Use BFS to find the shortest path
	int num_nodes = mEditDistanceOneGraph[word_len_id].getNumVertices();
	vector<bool> visited(num_nodes, false);
	queue<int> q;
	vector<int> parent(num_nodes, -1);
	q.push(a_id);
	while (!q.empty()) {
		int current = q.front();
		q.pop();
		visited[current] = true;

		// If we reach the target node, stop early
		if (current == b_id) {
			break;
		}

		// Explore neighbors
		auto &adj_list = mEditDistanceOneGraph[word_len_id].mAdjacencyList;
		for (int neighbor : adj_list[current]) {
			if (!visited[neighbor]) {
				parent[neighbor] = current; // Record the parent to reconstruct the path
				q.push(neighbor);
			}
		}
	}
	if (!visited[b_id]) {
		std::cerr << "No transform found between " << a << " and " << b << std::endl;
		return vector<string>(); // No path found
	}
	vector<string> path;
	for (int at = b_id; at != -1; at = parent[at]) {
		path.push_back(mWords[word_len_id][at]);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

vector<string> WordList::findRandomTransform()
{
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist(4, 12);
	// Select random words from "bigger"(4-12 in enable.txt list) sets of words
	int word_len_id = uniform_dist(e1);
	int a_id = rand() % mWords[word_len_id].size();
	int b_id = rand() % mWords[word_len_id].size();
	return findTransform(mWords[word_len_id][a_id], mWords[word_len_id][b_id]);
}

WordList::~WordList()
{
}
