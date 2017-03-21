#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Graph.h"
#include "graphviewer.h"
#include "Purchase.h"
#include <string>

class Program
{
private:
	GraphViewer* gv;
	Graph<string> graph;
	bool running;
public:
	Program(char** files);
	void loadGraph(char* nodesFile, char* roadNodesFile, char* roadFile);
	void loadMarkets(char* marketsFile);
	void run();
	void displayMenu();
	void generatePurchases(int n);
};


#endif /* PROGRAM_H_ */
