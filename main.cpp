// main.cpp : Defines the entry point for the application.
//

#include "WordList.h"
#include <filesystem> 
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

int main()
{
	constexpr std::string_view def_path = R"(C:\Dev\enable.txt)";
	WordList word_list;
	cout << "Enter the file name for word list:" << endl;
	string path;
	cin >> path;
	if (fs::exists(path) && fs::is_regular_file(path))
	{
		word_list.setFileName(path);
	}
	else
	{
		cout << "Invalid path, use default "<<def_path << endl;
		word_list.setFileName(string(def_path));
	}	
	auto start = std::chrono::high_resolution_clock::now();
	word_list.load();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	std::cout << "Load execution time: " << duration.count() << " ms\n";
	word_list.printMemoryUsage();
	bool one_more = true;
	while (one_more)
	{
		bool valid = false;
		int option;
		while (!valid)
		{
			cout << endl;
			cout << "Choose the option (press 1/2/3):" << endl;
			cout << "1: Enter words to find a transform between (words would be added to the word list if it didn't contain them)" << endl;
			cout << "2: Generate transform between 2 random words of the same length from existing ones in the word list (repeated till success)" << endl;
			cout << "3: Exit the program" << endl;
			cin >> option;
			if (option == 1 || option == 2 || option == 3)
				valid = true;
		}
		if (option == 1)
		{
			string a, b;
			cout << "Enter the first word:" << endl;
			cin >> a;
			cout << "Enter the second word:" << endl;
			cin >> b;
			StringList result = word_list.findTransform(a, b);
			for (int i = 0; i < result.size(); ++i)
			{
				cout << result[i];
				if (i < result.size() - 1)
					cout << " -> ";
				else
					cout << endl;
			}
		}
		else if (option == 2)
		{
			StringList result;
			while (result.empty())
			{
				result = word_list.findRandomTransform();
			}
			for (int i = 0; i < result.size(); ++i)
			{
				cout << result[i];
				if (i < result.size() - 1)
					cout << " -> ";
				else
					cout << endl;
			}
		}
		else
		{
			one_more = false;
		}
	}
	return 0;
}
