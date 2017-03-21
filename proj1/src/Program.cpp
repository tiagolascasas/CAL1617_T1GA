/*
 * Program.cpp
 *
 *  Created on: 21/03/2017
 *      Author: Tiago
 */

#include "Program.h"
#include "Exceptions.h"
#include <fstream>

Program::Program(char** files)
{
	this->gv = new GraphViewer(600, 600, true);
	this->running = true;

	loadGraph(files[1], files[2], files[3]);
	loadMarkets(files[4]);
}

void Program::loadGraph(char* nodesFile, char* roadNodesFile, char* roadFile)
{
	ifstream nodes(nodesFile);
	if (!nodes.is_open())
		throw new FileNotFound(nodesFile);

	ifstream roadNodes(roadNodesFile);
	if (!roadNodes.is_open())
		throw new FileNotFound(roadNodesFile);

	ifstream roads(roadFile);
	if (!roads.is_open())
		throw new FileNotFound(roadFile);

	//...
	//...

	nodes.close();
	roadNodes.close();
	roads.close();
}

void Program::loadMarkets(char* marketsFile)
{
	ifstream markets(marketsFile);
	if (!markets.is_open())
		throw new FileNotFound(marketsFile);

	//...
	//...

	markets.close();
}

void Program::generatePurchases(int n)
{

}

void Program::run()
{
	while (running)
	{
		displayMenu();
		cout << "Option: ";
		int choice;
		cin >> choice;

		switch (choice)
		{
		case 1:
			cout << "\nNumber of purchases to generate:";
			int n;
			cin >> n;
			generatePurchases(n);
			cout << n << " purchases generated\n";
			break;
		case 4:
			running = false;
			break;
		default: break;
		}
	}
}

void Program::displayMenu()
{
	cout << endl;
	cout << "1. Generate random purchases\n";
	cout << "2. do something #2\n";
	cout << "3. do something #3\n";
	cout << "4. Quit program\n";
	cout << endl;
}
