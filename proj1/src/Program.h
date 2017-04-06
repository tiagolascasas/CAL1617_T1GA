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
	GraphViewer* gv;			/// Pointer to a GraphViewer instantiation
	Graph<RoadNode> graph;		/// The main graph
	vector<road_t> r;			/// A vector which holds information about all roads
	vector<RoadNode> markets;	/// A vector that holds all markets
	vector<string> marketNames;	/// A vector that holds the names of all markets
	vector<Purchase> purchases;	/// A vector that holds all the clients/purchases
	unsigned int xRes;			/// The x resolution of the map
	unsigned int yRes;			/// The y resolution of the map
	string mapName;				/// The path and/or name of the map picture
	pair<float, float> origin;	/// The geographical coordinate of the map's top left corner
	pair<float, float> xMax;	/// The geographical coordinate of the map's top right corner
	pair<float, float> yMax;	/// The geographical coordinate of the map's bottom left corner
	bool running;				/// Flag which tells if the main loop is running
	float avgVelocity;			/// Average velocity value for the trucks (in Km/h)
	int lastEdgeID;				/// Last id used for an Edge on GraphViewer
	int lastNodeID;				/// Last id used for a Node on GraphViewer

	/**
	 * Loads the main graph from three files
	 * @param nodesFile file with info about the nodes
	 * @param roadInfoFile file with info about the roads
	 * @param roadFile file with info about which roads connect to each node
	 */
	void loadGraph(char* nodesFile, char* roadInfoFile, char* roadFile);

	/**
	 * Loads all markets from a file
	 * @param marketsFile file with the markets
	 */
	void loadMarkets(char* marketsFile);

	/**
	 * Loads the info about the map
	 * @param mapFile file with the info about the map
	 */
	void loadMap(char* mapFile);

	/**
	 * Displays the main menu
	 */
	void displayMenu();

	/**
	 * Displays the statistics of a graph (number of nodes and edges)
	 * @param g the graph whose statistics will be displayed
	 */
	void displayGraphStatistics(Graph<RoadNode> g);

	/**
	 * Displays a full graph using GraphViewer
	 * @param g the graph to be displayed
	 */
	void displayGraph(Graph<RoadNode> g);

	/**
	 * Displays all markets (id and name)
	 */
	void displayMarketsInfo();

	/**
	 * Displays all purchases (id and geographical location),
	 * plus the valid markets for each purchase
	 */
	void displayPurchasesInfo();

	/**
	 * Displays which markets
	 */
	void displayConnectivity();
	void displaySubGraph(vector<Vertex<RoadNode>* > path);
	void displayClosestMarketsToClients();

	void resetGV();
	void generatePurchases(int n);
	int calculateTime(int length);
	pair<int, int> mapCoordToXY(RoadNode n);

	int getIndexOfMarket(RoadNode m);
	string getMarketName(int idx);
	string getMarketName(RoadNode n);
	void checkValidMarkets();
	void addMarketToPurchase(RoadNode market, RoadNode purchaseAddr);

	void dfsConnectivity(Vertex<RoadNode>* v, RoadNode market);
	void setClosestMarketToAllClients();
	vector<RoadNode> getTruckPath(RoadNode market, vector<RoadNode> &clients, int &distance);

	void singleMarketSingleClient();
	void allMarketsSingleClient();
	void singleMarketAllClients();
	void allMarketsAllClients();


public:
	Program(char** files);
	void run();
};


#endif /* PROGRAM_H_ */
