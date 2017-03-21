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
	//...
public:
	Program();
	void loadGraph();
	void generatePurchases();
	//...
	void run();
};

#endif /* PROGRAM_H_ */
