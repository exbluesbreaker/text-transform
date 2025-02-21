#include "WordList.h"

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
		addWord(line);
	}
	file.close();
	std::cout << "Loaded " << num_words << " words from " << mFileName << std::endl;
	for (int i = 0; i < mWords.size(); i++)
	{
		std::cout << "Words of length " << i + 1 << ": " << mWords[i].size() << std::endl;
	}
	// Create the graphs for words of different lengths
	for (int i = 0; i < mWords.size(); i++)
	{
		WordMasks masks = getMasks(i+1);

		mEditDistanceOneGraph.push_back(Graph(mWords[i].size()));
		// Simple hash function for pairs of integers
		struct pair_hash {
			inline size_t operator()(const pair<int, int>& v) const {
				return v.first * 31 + v.second;
			}
		};
		unordered_set<pair<int,int>, pair_hash> seenPairs;// To avoid duplicates
		for (const auto& [mask, wordIds] : masks)
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

void WordList::addWord(string word)
{
	int len = word.size();
	if (len > mWords.size())
	{
		mWords.resize(len);
	}
	mNumWords++;
	mWords[len - 1].push_back(word);
}

WordList::WordMasks WordList::getMasks(size_t len)
{
	WordMasks masks;
	for (size_t i = 0; i < mWords[len-1].size(); i++)
	{
		auto &word = mWords[len-1][i];
		for (size_t j = 0; j < word.size(); j++) {
			// Generate a mask by replacing the i-th character with '*'
			std::string mask = word;
			mask[j] = '*';
			// Store the word index in the vector of words with the same mask
			masks[mask].push_back(i);
		}
	}
	return masks;
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
	auto path = mEditDistanceOneGraph[word_len_id].getShortestPath(a_id, b_id);
	if (path.empty())
	{
		std::cerr << "No transform between "<<a<<" and "<<b<<" found" << std::endl;
	}
	vector<string> words;
	for (auto& id : path)
	{
		words.push_back(mWords[word_len_id][id]);
	}
	return words;
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
