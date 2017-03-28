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
	void loadGraph(char* nodesFile, char* roadInfoFile, char* roadFile);
	void loadMarkets(char* marketsFile);
	void displayMenu();
	void displayGraphStatistics(Graph<RoadNode> g);
	void displayGraph(Graph<RoadNode> g);
	void generatePurchases(int n);
public:
	Program(char** files);
	void run();
};


#endif /* PROGRAM_H_ */
