#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Program.h"
#include "Exceptions.h"

using namespace std;

#define NO_ARGS 1
#define ARGS 5

int main(int argc, char** argv)
{
	if (argc != ARGS && argc != NO_ARGS)
	{
		cout << "Invalid number of arguments!\n";
		cout << "Usage: proj1 nodes_file road_info_file road_file markets_file\n";
		cout << "       or simply proj1 to use the default files in ./res\n";
		return 1;
	}

	srand(time(NULL));

	Program* p;

	try
	{
		if (argc == 5)
			p = new Program(argv);
		else
		{
			char* filenames[] = { "", "res/nodes.txt", "res/road_info.txt",
									"res/roads.txt", "res/markets.txt" };
			p = new Program(filenames);
		}
	}
	catch(FileNotFound &ex)
	{
		cout << "File " << ex.filename << " not found, terminating...\n";
		return 1;
	}

	try
	{
		p->run();
	}
	catch(...)
	{
		cout << "An unexpected error occurred, terminating...\n";
		return 1;
	}

	return 0;
}
