#include "WordList.h"

WordList::WordList()
{
}

WordList::WordList(const string &name) :mFileName(name)
{
}

bool WordList::load()
{
	std::ifstream file(mFileName);
	if (!file) 
	{
		// Simple error handling without exceptions
		cerr << "Error opening file: " << mFileName << endl;
		return false;
	}
	// Clear the data structures
	mWords.clear();
	mWords.shrink_to_fit();
	mMasks.clear();
	mMasks.shrink_to_fit();
	mEditDistanceOneGraph.clear();
	mEditDistanceOneGraph.shrink_to_fit();
	string line;
	int num_words = 0;
	while (std::getline(file, line)) 
	{
		addWord(line);
	}
	file.close();
	cout << "Loaded " << num_words << " words from " << mFileName << endl;
	for (size_t i = 0; i < mWords.size(); ++i)
	{
		cout << "Words of length " << i + 1 << ": " << mWords[i].size() << endl;
	}
	// Create the graphs for words of different lengths
	for (size_t i = 0; i < mWords.size(); ++i)
	{
		auto &masks = mMasks[i];

		mEditDistanceOneGraph.push_back(Graph(mWords[i].size()));
		// Simple hash function for pairs of integers
		// https://stackoverflow.com/questions/15160889/how-can-i-make-an-unordered-set-of-pairs-of-integers-in-c
		struct pair_hash {
			inline size_t operator()(const pair<int, int>& v) const {
				return v.first * 31 + v.second;
			}
		};
		unordered_set<pair<int,int>, pair_hash> seenPairs;// To avoid duplicates
		for (const auto& [mask, wordIds] : masks)
		{
			// Go through all pairs of words with the same mask and add an edge between them
			for (size_t j = 0; j < wordIds.size(); ++j)
			{
				for (size_t k = j + 1; k < wordIds.size(); ++k)
				{
					pair<int, int> new_pair;
					// Order pair to avoid duplicates
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

size_t WordList::addWord(const string &word)
{
	// Add word to the list
	int len = word.size();
	if (len > mWords.size())
	{
		mWords.resize(len);
		mMasks.resize(len);
	}
	mNumWords++;
	int word_id = mWords[len - 1].size();
	mWords[len - 1].push_back(word);
	// Add masks for a word
	for (size_t i = 0; i < word.size(); ++i) 
	{
		// Generate a mask by replacing the i-th character with '*'
		std::string mask = word;
		mask[i] = '*';
		// Store the word index in the vector of words with the same mask
		mMasks[len - 1][mask].push_back(word_id);
	}
	return word_id;
}

void WordList::addWordToGraph(const string &word)
{
	// Method assumes it follows addWord for a given word
	size_t word_len_id = word.size()-1;
	auto& masks = mMasks[word_len_id];
	auto& words = mWords[word_len_id];
	mEditDistanceOneGraph[word_len_id].addNode(words.size()-1);
	for (const auto& [mask, wordIds] : masks)
	{
		if (wordIds.size() < 2)
			continue;
		// Just added word is the last one always
		int word_id = wordIds[wordIds.size() - 1];
		if (words[word_id] != word)
			continue;
		for (size_t i = 0; i < wordIds.size() - 1; ++i)
		{
			mEditDistanceOneGraph[word_len_id].addEdge(wordIds[i], word_id);
		}
	}
}

StringList WordList::findTransform(const string &a, const string &b)
{
	// Search for tranform is straightforward
	// Just use the graph to find the shortest path between nodes corresponding to words
	if (a.size() != b.size())
	{
		std::cerr << "Words must be of the same length" << std::endl;
		return StringList();
	}
	int a_id = -1;
	int b_id = -1;
	int word_len_id = a.size()-1;
	for (size_t i = 0; i< mWords[word_len_id].size(); ++i)
	{
		auto &word = mWords[word_len_id][i];
		if (word == a)
			a_id = i;
		if (word == b)
			b_id = i;
	}
	if (a_id == -1)
	{
		cout << "Word "<< a<<" is not in the word list, adding it to the list" << endl;
		a_id = addWord(a);
		addWordToGraph(a);
	}
	if (b_id == -1)
	{
		cout << "Word " << b << " is not in the word list, adding it to the list" << endl;
		b_id = addWord(b);
		addWordToGraph(b);
	}
	auto path = mEditDistanceOneGraph[word_len_id].getShortestPath(a_id, b_id);
	if (path.empty())
	{
		cerr << "No transform between "<<a<<" and "<<b<<" found" << endl;
		return StringList();
	}
	StringList words;
	for (auto& id : path)
	{
		words.push_back(mWords[word_len_id][id]);
	}
	cout << "Transform between " << a << " and " << b << " was found :" << endl;
	return words;
}

StringList WordList::findRandomTransform()
{
	std::random_device r;
	std::default_random_engine e1(r());
	std::uniform_int_distribution<int> uniform_dist_len(4, 12);
	// Select random words from "bigger"(4-12 in enable.txt list) sets of words
	int word_len_id = uniform_dist_len(e1);
	int a_id = 0;
	int b_id = 0;
	while (a_id == b_id)
	{
		std::uniform_int_distribution<int> uniform_dist_word(0, mWords[word_len_id].size());
		a_id = uniform_dist_word(e1);
		b_id = uniform_dist_word(e1);
	}
	return findTransform(mWords[word_len_id][a_id], mWords[word_len_id][b_id]);
}

void WordList::printMemoryUsage() const
{
	// It doesn't show perfectly exact calculation
	// Just takes the main contributors to memory usage into account
	cout << "Memory usage estimation (main contributors, not exact):" << endl;
	size_t size_graph = 0;
	for (size_t i = 0; i < mWords.size(); ++i)
	{
		size_graph += mEditDistanceOneGraph[i].getMemoryUsage();
	}
	cout << "All graphs consume: " << size_graph << " bytes" << endl;
	size_t size_words = 0;
	for (size_t i = 0; i < mWords.size(); ++i)
	{
		//just estimate size of actually stored strings as the main contributor to memory usage
		for (const auto& word : mWords[i])
		{
			size_words += word.capacity() * sizeof(char);
		}
	}
	cout << "All words consume: " << size_words << " bytes" << endl;
	size_t size_masks = 0;
	for (size_t i = 0; i < mMasks.size(); ++i)
	{
		// estimate hash map table usage
		size_masks += mMasks[i].bucket_count() * sizeof(void*);
		// estimate entries
		for (const auto& [mask, wordIds] : mMasks[i])
		{
			// key is a string
			size_masks += mask.capacity() * sizeof(char);
			// value for given key is a vector of integers
			size_masks += wordIds.capacity() * sizeof(int);
		}
	}
	cout << "All masks consume: " << size_masks << " bytes" << endl;
	cout << "Total memory usage: " << size_graph + size_words + size_masks << " bytes" << endl;
}

WordList::~WordList()
{
}
