#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Graph.h"
#include "graphviewer.h"
#include "Purchase.h"
#include "RoadNode.h"
#include <string>

class Program
{
private:
	GraphViewer* gv;
	Graph<RoadNode> graph;
	vector<road_t> r;
	bool running;
public:
	Program(char** files);
	void loadGraph(char* nodesFile, char* roadInfoFile, char* roadFile);
	void loadMarkets(char* marketsFile);
	void run();
	void displayMenu();
	void displayGraph(Graph<RoadNode> g);
	void generatePurchases(int n);
};


#endif /* PROGRAM_H_ */
