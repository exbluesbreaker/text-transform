// ShortBread.cpp : Defines the entry point for the application.
//

#include "ShortBread.h"
#include "WordList.h"

using namespace std;

int main()
{
	WordList list(R"(C:\Dev\enable.txt)");
	list.load();
	vector<string> result;
	while (result.empty())
	{
		result = list.findRandomTransform();
		for (int i = 0; i < result.size(); i++)
		{
			cout << result[i];
			if (i < result.size() - 1)
				cout << " -> ";
			else
				cout << endl;
		}
	}
	return 0;
}
