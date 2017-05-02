#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Graph.h"
#include "graphviewer.h"
#include "Purchase.h"
#include "RoadNode.h"
#include <string>
#include <map>

class Program
{
private:
	GraphViewer* gv;					/// Pointer to a GraphViewer instantiation
	Graph<RoadNode> graph;				/// The main graph
	vector<road_t> r;					/// A vector which holds information about all roads
	string roadNames;					/// A string holding all names of the roads, without duplicates
	vector<RoadNode> markets;			/// A vector that holds all markets
	vector<string> marketNames;			/// A vector that holds the names of all markets
	map<string, string>	roadMarkets;	///	A map holding, for a road, its adjacent market
	vector<Purchase> purchases;			/// A vector that holds all the clients/purchases
	unsigned int xRes;					/// The x resolution of the map
	unsigned int yRes;					/// The y resolution of the map
	string mapName;						/// The path and/or name of the map picture
	pair<float, float> origin;			/// The geographical coordinate of the map's top left corner
	pair<float, float> xMax;			/// The geographical coordinate of the map's top right corner
	pair<float, float> yMax;			/// The geographical coordinate of the map's bottom left corner
	bool running;						/// Flag which tells if the main loop is running
	float avgVelocity;					/// Average velocity value for the trucks (in Km/h)
	int deliveryTime;					/// Time spent on a single delivery (in min)
	int lastEdgeID;						/// Last id used for an Edge on GraphViewer
	int lastNodeID;						/// Last id used for a Node on GraphViewer

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

	/**
	 * Displays the graph given as argument
	 * @param path path to be displayed in graphviewer
	 */
	void displaySubGraph(vector<Vertex<RoadNode>* > path);

	/**
	 * Lists the market closest to every client
	 * Verifies whether the client is reachable or not
	 */
	void displayClosestMarketsToClients();

	/**
	 * Displays multiple paths at once, signalling the clients of each path appropriately
	 * @param paths a vector with paths (each path is a vector of RoadNodes)
	 * @param clients a vector of the clients to be signalled
	 */
	void displaySetOfPaths(vector<vector<RoadNode> > paths, vector<RoadNode> clients);

	/**
	 * Clears the graphviewer of all edges and nodes
	 */
	void resetGV();

	/**
	 * Generates the specified amount of purchases
	 * @param n amount of purchases to generate
	 */
	void generatePurchases(int n);

	/**
	 * Calculates the average amount of time needed to travel a specified distance
	 * @param length travelled distance
	 * @return average time to travel specified distance (in minutes)
	 */
	int calculateTime(int length, int numberOfClients);

	/**
	 * Converts a node's geographical coordinates to a (x, y) coordinate system
	 * @param n node whose coordinates will be converted
	 * @return pair with the coordinates x and y (in this order)
	 */
	pair<int, int> mapCoordToXY(RoadNode n);


	/**
	 * Gets the Market's index in the markets vector
	 * @param m market to search for in the markets vector
	 * @return index of given market or -1 if it doesn't exists
	 */
	int getIndexOfMarket(RoadNode m);

	/**
	 * Gets the market's name based on its index on the markets vector
	 * @param idx index of the market
	 * @return market's name if the index is valid
	 */
	string getMarketName(int idx);

	/**
	 * Gets the market's name
	 * @param n RoadNode that represents the market
	 * @return market's name
	 */
	string getMarketName(RoadNode n);

	/**
	 * Goes through the markets vector to check the valid state of each market
	 * @see Program::dfsConnectivity
	 */
	void checkValidMarkets();

	/**
	 * Adds the given market to the address that matches purchaseAddr if purchaseAddr is a purchase
	 * @param market RoadNode of the market to add to the purchase
	 * @param purchaseAddr RoadNode of the client's adress
	 */
	void addMarketToPurchase(RoadNode market, RoadNode purchaseAddr);

	/**
	 * Tries to find a path from the market to a purchase
	 * @see Program::addMarketToPurchase
	 * @param v vertex purchase
	 * @param market market's address
	 */
	void dfsConnectivity(Vertex<RoadNode>* v, RoadNode market);

	/**
	 * Allows the user to change parameters such as average velocity and time per delivery
	 */
	void changeParameters();

	/**
	 * Uses Dijkstra's shortest path algorithm to get the shortest way from the market to the client
	 * And calls setClosestMarketIndex
	 * @see Purchase::setClosestMarketIndex
	 */
	void setClosestMarketToAllClients();

	/**
	 * Gets the path for the truck to deliver to all clients
	 * @param market node that represents both the starting point and the finish
	 * @param clients vector with all the clients for the delivery
	 * @param distance pointer used to return the distance between the market and the farthest client
	 * @return vector containing the trucks path for the delivery
	 */
	vector<RoadNode> getTruckPath(RoadNode market, vector<RoadNode> &clients, int &distance);

	/*
	 * Analyzes data about several paths (their distance and duration)
	 * and displays various alternatives based on possible number of trucks
	 * @param distTime a vector with pairs holding info about paths. Each
	 * pair holds the distance of a path and its duration, by this order
	 */
	void analyzeData(vector<pair<int, int> > distTime);

	/**
	 * Distributes from a single market to a single client
	 */
	void singleMarketSingleClient();

	/**
	 * Distributes from all markets to a single client
	 */
	void allMarketsSingleClient();

	/**
	 * Distributes from a single market to all clients
	 */
	void singleMarketAllClients();

	/**
	 * Distributes from all markets to all clients
	 */
	void allMarketsAllClients();

	/**
	 * Shows a menu with multiple search options, asking in a loop for inputs
	 */
	void searchMenu();

	/**
	 * Searches for a road using exact matching and then shows if the road has any adjacent market
	 */
	void searchRoadExact();

	/*
	 * Searches for a market using exact matching
	 */
	void searchMarketExact();

	/**
	 * Searches for a road using approximate matching, showing similar roads if the road does not exist
	 */
	void searchRoadApprox();

	/**
	 * Searches for a market using approximate matching, showing similar markets if the market does not exist
	 */
	void searchMarketApprox();


public:
	/**
	 * Creates a Program object
	 * @param files array containing data files' names
	 */
	Program(char** files);

	/**
	 * Starts the application's interface
	 */
	void run();
};


#endif /* PROGRAM_H_ */
