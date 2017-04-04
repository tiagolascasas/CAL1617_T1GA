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
	vector<RoadNode> markets;
	vector<string> marketNames;
	vector<Purchase> purchases;
	bool running;
	float avgVelocity;

	void loadGraph(char* nodesFile, char* roadInfoFile, char* roadFile);
	void loadMarkets(char* marketsFile);

	void displayMenu();
	void displayGraphStatistics(Graph<RoadNode> g);
	void displayGraph(Graph<RoadNode> g);
	void displayMarketsInfo();
	void displayPurchasesInfo();
	void displayConnectivity();
	void displaySubGraph(vector<Vertex<RoadNode>* > path);
	void displayClosestMarketsToClients();

	void generatePurchases(int n);
	int calculateTime(int length);

	int getIndexOfMarket(RoadNode m);
	string getMarketName(int idx);
	string getMarketName(RoadNode n);
	void checkValidMarkets();
	void addMarketToPurchase(RoadNode market, RoadNode purchaseAddr);

	void dfsConnectivity(Vertex<RoadNode>* v, RoadNode market);
	void setClosestMarketToAllClients();
	void singleMarketSingleClient();
	void allMarketsSingleClient();
	void singleMarketAllClients();
	void allMarketsAllClients();


public:
	Program(char** files);
	void run();
};


#endif /* PROGRAM_H_ */
