/*
 * Program.cpp
 *
 *  Created on: 21/03/2017
 *      Author: Tiago
 */

#include "Program.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>

#define DEFAULT_PURCHASES 10

Program::Program(char** files): avgVelocity(30), running(true), lastEdgeID(-1), lastNodeID(-1)
{
	loadGraph(files[1], files[2], files[3]);
	loadMarkets(files[4]);
	loadMap(files[5]);
	this->gv = new GraphViewer(xRes, yRes, false);
	gv->setBackground(mapName);
	gv->createWindow(xRes, yRes);
	generatePurchases(DEFAULT_PURCHASES);
}

void Program::loadGraph(char* nodesFile, char* roadInfoFile, char* roadFile)
{
	ifstream nodes(nodesFile);
	if (!nodes.is_open())
		throw new FileNotFound(nodesFile);

	ifstream roadInfo(roadInfoFile);
	if (!roadInfo.is_open())
		throw new FileNotFound(roadInfoFile);

	ifstream roads(roadFile);
	if (!roads.is_open())
		throw new FileNotFound(roadFile);

	string s;

	while (getline(nodes, s))
	{
		istringstream ss(s);
		long long id;
		float latDeg, latRad, lonDeg, lonRad;
		char marker;
		ss >> id >> marker >> latDeg >> marker >> lonDeg;
		ss >> marker >> latRad >> marker >> lonRad;
		RoadNode n(id, latDeg, latRad, lonDeg, lonRad);
		graph.addVertex(n);
	}

	while (getline(roadInfo, s))
	{
		road_t road;
		istringstream ss(s);
		char marker;
		string aux;
		ss >> road.id >> marker;
		getline(ss, aux, ';');
		if (aux == "")
			road.name = "Undefined street name";
		else
			road.name = aux;
		ss >> marker >> aux;
		if (aux.find("lse", 0) != aux.npos)
			road.twoWay = false;
		else
			road.twoWay = true;
		r.push_back(road);
	}

	while (getline(roads, s))
	{
		long long id, v1, v2;
		char marker;
		istringstream ss(s);
		ss >> id >> marker >> v1 >> marker >> v2;
		road_t road;
		for (int i = 0; i < r.size(); i++)
		{
			if (r.at(i).id == id)
			{
				road = r.at(i);
				break;
			}
		}

		RoadNode aux1(v1, 0, 0, 0, 0), aux2(v2, 0, 0, 0, 0);
		RoadNode n1 = graph.getVertex(aux1)->getInfo();
		RoadNode n2 = graph.getVertex(aux2)->getInfo();
		float edgeDistance = n1.getDistanceBetween(n2);
		graph.addEdge(aux1, aux2, edgeDistance, id);
		if (road.twoWay)
			graph.addEdge(aux2, aux1, edgeDistance, id);
	}
	nodes.close();
	roadInfo.close();
	roads.close();
}

void Program::loadMap(char* mapFile)
{
	ifstream map(mapFile);
	if (!map.is_open())
		throw new FileNotFound(mapFile);

	string s;
	istringstream* ss;
	char comma;
	getline(map, s);
	mapName = s;
	getline(map, s);
	ss = new istringstream(s);
	pair<float, float> p1;
	(*ss) >> p1.first >> comma >> p1.second;
	origin = p1;
	getline(map, s);
	ss = new istringstream(s);
	pair<float, float> p2;
	(*ss) >> p2.first >> comma >> p1.second;
	yMax = p2;
	getline(map, s);
	ss = new istringstream(s);
	pair<float, float> p3;
	(*ss) >> p3.first >> comma >> p3.second;
	xMax = p3;
	getline(map, s);
	ss = new istringstream(s);
	(*ss) >> xRes >> yRes;
}

void Program::loadMarkets(char* marketsFile)
{
	ifstream mark(marketsFile);
	if (!mark.is_open())
		throw new FileNotFound(marketsFile);

	string s;
	while (getline(mark, s))
	{
		istringstream ss(s);
		long long marketID;
		string mName;
		ss >> marketID;
		getline(ss, mName);
		mName.erase(0, 1);
		for (int i = 0; i < graph.getNumVertex(); i++)
		{
			if (graph.getVertexSet().at(i)->getInfo().getID() == marketID)
			{
				RoadNode n = graph.getVertexSet().at(i)->getInfo();
				markets.push_back(n);
				marketNames.push_back(mName);
				break;
			}
		}
	}
	mark.close();
}

void Program::generatePurchases(int n)
{
	if (n >= graph.getNumVertex() - markets.size())
	{
		cout << "Number of purchases selected is too big, value defaulted to ";
		n = (graph.getNumVertex() - markets.size()) / 10;
		cout << n << endl;
	}
	purchases.clear();
	for (int i = 0; i < n; i++)
	{
		int randIndex = rand() % graph.getNumVertex();
		RoadNode node = graph.getVertexSet().at(randIndex)->getInfo();
		Purchase p(node);
		if (find(purchases.begin(), purchases.end(), p) != purchases.end())
			i--;
		else
			purchases.push_back(p);
	}
	checkValidMarkets();
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
			displayMarketsInfo();
			break;
		case 2:
			displayGraphStatistics(graph);
			displayGraph(graph);
			break;
		case 3:
			cout << "\nNumber of purchases to generate:";
			int n;
			cin >> n;
			generatePurchases(n);
			cout << n << " purchases generated\n";
			break;
		case 4:
			displayPurchasesInfo();
			break;
		case 5:
			displayConnectivity();
			break;
		case 6:
			singleMarketSingleClient();
			break;
		case 7:
			allMarketsSingleClient();
			break;
		case 8:
			singleMarketAllClients();
			break;
		case 9:
			allMarketsAllClients();
			break;
		case 0:
			running = false;
			break;
		default:
			break;
		}
	}
}

void Program::displayMenu()
{
	cout << endl;
	cout << "1. Display all markets\n";
	cout << "2. Display the whole graph\n";
	cout << "3. Generate random clients/purchases\n";
	cout << "4. Display all clients/purchases\n";
	cout << "5. Check connectivity between all clients and all markets\n";	//Conectividade para gr�ficos dirigidos
	cout << "6. Distribute from a single market to a single client\n";		//Dijkstra	- feito, falta mostrar o grafo
	cout << "7. Distribute from all markets to a single client\n";			//Dijkstra	- feito, falta mostrar o grafo
	cout << "8. Distribute from a single market to all clients\n";			//Dijkstra, minimum spanning tree
	cout << "9. Distribute from all markets to all clients\n";				//Mesmo que o anterior, mas com um preprocessamento
	cout << "0. Quit program\n";											//que indica o supermercado mais pr�ximo de cada cliente
	cout << endl;
}

void Program::displayGraph(Graph<RoadNode> g)
{
/*	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(false);
	for (int i = 0; i < g.getVertexSet().size(); i++)
	{
		pair<int, int> coord = mapCoordToXY(g.getVertexSet().at(i)->getInfo());
		gv->addNode(0, coord.first, coord.second);
		gv->setVertexSize(0, 5);
	}
	lastNodeID = g.getVertexSet().size() - 1;
	int edgeID = 0;
	for (int i = 0; i < g.getVertexSet().size(); i++)
	{
		Vertex<RoadNode>* n = g.getVertexSet().at(i);
		for (int j = 0; j < n->getAdj().size(); j++)
		{
			int id1 = n->getInfo().getID();
			int id2 = n->getAdj().at(j).getDest()->getInfo().getID();
			gv->addEdge(edgeID, id1, id2, EdgeType::DIRECTED);
			gv->setEdgeWeight(edgeID, n->getAdj().at(j).getWeight());
			edgeID++;
		}
	}
	lastEdgeID = edgeID;
	gv->rearrange();*/

}

void Program::displayGraphStatistics(Graph<RoadNode> g)
{
	cout << "\nGraph statistics: " << g.getNumVertex() << " nodes and ";
	int nEdges = 0;
	for (int i = 0; i < g.getVertexSet().size(); i++)
		nEdges += g.getVertexSet().at(i)->getAdj().size();
	cout << nEdges << " edges\n";
}

void Program::displayMarketsInfo()
{
	cout << endl;
	for (int i = 0; i < markets.size(); i++)
		cout << "Market " << i + 1 << ": " << markets.at(i) << " Name: " << marketNames.at(i) << endl;
}

void Program::displayPurchasesInfo()
{
	cout << endl;
	for (int i = 0; i < purchases.size(); i++)
	{
		cout << "Purchase " << setw(3) << left << i + 1 << ": " << purchases.at(i).getAddr() << setw(16) << "Valid Markets: ";
		for (int j = 0; j < purchases.at(i).getValidMarkets().size(); j++)
		{
			vector<RoadNode>::iterator it = find(markets.begin(), markets.end(), purchases.at(i).getValidMarkets().at(j));
			cout << distance(markets.begin(), it) + 1 << " ";
		}
		cout << endl;
	}
}

void Program::singleMarketSingleClient()
{
	displayPurchasesInfo();
	cout << "\nSelect by index the client/purchase: ";
	int clientIdx;
	cin >> clientIdx;
	displayMarketsInfo();
	cout << "\nSelect by index the market: ";
	int marketIdx;
	cin >> marketIdx;
	clientIdx--;
	marketIdx--;
	try
	{
		int length = graph.dijkstraShortestPath(markets.at(marketIdx), purchases.at(clientIdx).getAddr());
		vector<Vertex<RoadNode>* > path = graph.getPathVertex(markets.at(marketIdx), purchases.at(clientIdx).getAddr());

		if (length == INT_INFINITY)
			cout << "There is no connection between the market and the client\n";
		else
		{
			try
			{
				displaySubGraph(path);
			}
			catch(...)
			{
				cout << "Problem displaying graph\n";
			}
			cout << "Shortest path from market "<< marketIdx + 1 << " (" << getMarketName(marketIdx) <<
					") is " << length << " meters (" << setprecision(2) << length / 1000.0 <<
					" Km), estimated time is " << calculateTime(length) << " min\n";
		}
	}
	catch (out_of_range &ex)
	{
		cout << "Index of one or more choices was invalid\n";
	}
	return;
}

void Program::addMarketToPurchase(RoadNode market, RoadNode purchaseAddr)
{
	for (int i = 0; i < purchases.size(); i++)
	{
		if (purchases.at(i).getAddr() == purchaseAddr)
		{
			purchases.at(i).addValidMarket(market);
			break;
		}
	}
}

void Program::dfsConnectivity(Vertex<RoadNode>* v, RoadNode market)
{
	v->setVisited(true);
	addMarketToPurchase(market, v->getInfo());

	for (int i = 0; i < v->getAdj().size(); i++)
	{
		if (!v->getAdj().at(i).getDest()->getVisited())
			dfsConnectivity(v->getAdj().at(i).getDest(), market);
	}
}

void Program::checkValidMarkets()
{
	for (int i = 0; i < markets.size(); i++)
	{
		dfsConnectivity(graph.getVertex(markets.at(i)), markets.at(i));
		graph.resetVisited();
	}
}

int Program::getIndexOfMarket(RoadNode m)
{
	for (int i = 0; i < markets.size(); i++)
	{
		if (markets.at(i) == m)
			return i;
	}
	return -1;
}

string Program::getMarketName(int idx)
{
	try
	{
		return marketNames.at(idx);
	}
	catch (out_of_range &ex)
	{
		cout << "Invalid market selected\n";
	}
}

void Program::displayConnectivity()
{
	cout << endl;
	for (int i = 0; i < purchases.size(); i++)
	{
		cout << "Purchase " << left << setw(3) << i + 1 << ": Markets ";
		for (int j = 0; j < purchases.at(i).getValidMarkets().size(); j++)
			cout << getIndexOfMarket(purchases.at(i).getValidMarkets().at(j)) + 1 << " ";
		cout << endl;
	}
}

void Program::allMarketsSingleClient()
{
	displayPurchasesInfo();
	cout << "\nSelect by index the client/purchase: ";
	int clientIdx;
	cin >> clientIdx;
	clientIdx--;
	try
	{
		if (purchases.at(clientIdx).getValidMarkets().size() == 0)
		{
			cout << "There isn't any market that can reach the specified client\n";
			return;
		}
		for (int i = 0; i < purchases.at(clientIdx).getValidMarkets().size(); i++)
		{
			int length = graph.dijkstraShortestPath(purchases.at(clientIdx).getValidMarkets().at(i),
													purchases.at(clientIdx).getAddr());
			cout << "Shortest path from market "<< i + 1 << " (" << getMarketName(i) << ") is " << length << " meters (" <<
					setprecision(2) << length / 1000.0 << " Km), estimated time is " << calculateTime(length) << " min\n";
		}
	}
	catch (out_of_range &ex)
	{
		cout << "Selected client index was invalid\n";
		return;
	}
	return;
}

int Program::calculateTime(int length)
{
	float v = avgVelocity / 3.6;
	float t = length / v;
	return static_cast<int>(t / 60);
}

string Program::getMarketName(RoadNode n)
{
	for (int i = 0; i < markets.size(); i++)
	{
		if (markets.at(i) == n)
			return marketNames.at(i);
	}
}

void Program::displaySubGraph(vector<Vertex<RoadNode>* > path)
{
	resetGV();
//	gv->setBackground(mapName);
//	gv->createWindow(xRes, yRes);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(false);

	pair<int, int> coord = mapCoordToXY(path.at(0)->getInfo());
	gv->addNode(0, coord.first, coord.second);
	gv->setVertexLabel(0, getMarketName(path.at(0)->getInfo()));
	gv->setVertexColor(0, RED);
	gv->setVertexSize(0, 5);

	for (int i = 1; i < path.size() - 1; i++)
	{
		coord = mapCoordToXY(path.at(i)->getInfo());
		gv->addNode(i, coord.first, coord.second);
		gv->setVertexSize(i, 5);
		gv->addEdge(i - 1, i - 1, i, EdgeType::DIRECTED);
		gv->setEdgeWeight(i, path.at(i - 1)->getInfo().getDistanceBetween(path.at(i)->getInfo()));
	}
	coord = mapCoordToXY(path.at(path.size() - 1)->getInfo());
	gv->addNode(path.size() - 1, coord.first, coord.second);
	gv->setVertexLabel(path.size() - 1, "Destination");
	gv->setVertexColor(path.size() - 1, GREEN);
	gv->setVertexSize(path.size() - 1, 5);
	gv->addEdge(path.size() - 2, path.size() - 2, path.size() - 1, EdgeType::DIRECTED);
	gv->setEdgeWeight(path.size() - 1,
			path.at(path.size() - 2)->getInfo().getDistanceBetween(path.at(path.size() - 1)->getInfo()));
	gv->rearrange();
	lastEdgeID = path.size() - 2;
	lastNodeID = path.size() - 1;
}

void Program::setClosestMarketToAllClients()
{
	for (int i = 0; i < markets.size(); i++)
	{
		graph.dijkstraShortestPath(markets.at(i));
		for (int j = 0; j < purchases.size(); j++)
		{
			int dist = graph.getVertex(purchases.at(j).getAddr())->getDist();
			if (dist != INT_INFINITY)
				purchases.at(j).setClosestMarketIndex(i, dist);
		}
	}
}

void Program::displayClosestMarketsToClients()
{
	cout << endl;	//use sort()
	for (int i = 0; i < purchases.size(); i++)
	{
		if (purchases.at(i).getClosestMarketIndex() != -1)
			cout << "Purchase " << i + 1 << ": " << purchases.at(i).getClosestMarketIndex() + 1 <<
					" (" << getMarketName(purchases.at(i).getClosestMarketIndex()) << ")\n";
		else
			cout << "Purchase " << i + 1 << ": no market can reach this client\n";
	}
}

vector<RoadNode> Program::getTruckPath(RoadNode market, vector<RoadNode> &clients, int &distance)
{
	//First part: get client node that is farther away from the market
	Vertex<RoadNode>* farthestVertex;
	int farthestDistance = 0;
	for (int i = 0; i < graph.getVertexSet().size(); i++)
	{
		vector<RoadNode>::iterator it = find(clients.begin(), clients.end(),
										graph.getVertexSet().at(i)->getInfo());
		if (it != clients.end() && farthestDistance < graph.getVertexSet().at(i)->getDist())
		{
			farthestVertex = graph.getVertexSet().at(i);
			farthestDistance = graph.getVertexSet().at(i)->getDist();
			clients.erase(it);
		}
	}
	distance = farthestDistance;

	//Second part: get the path from the previous vertex to the market, and if it
	//goes through a client, remove that client from the client vector
	list<Vertex<RoadNode>* > buffer;
	buffer.push_front(farthestVertex);
	while ( farthestVertex->path != NULL &&  farthestVertex->path->getInfo() != market) {
		farthestVertex = farthestVertex->path;
		buffer.push_front(farthestVertex);
	}
	if( farthestVertex->path != NULL )
		buffer.push_front(farthestVertex->path);

	//Third part: convert to a vector and return
	vector<RoadNode> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front()->getInfo() );
		buffer.pop_front();
	}
	return res;
}

void Program::singleMarketAllClients()
{
	displayMarketsInfo();
	cout << "\nSelect by index the market: ";
	int marketIdx;
	cin >> marketIdx;
	marketIdx--;
	vector<RoadNode> validPurchases;
	for(int i=0; i < purchases.size(); i++)
		for(int j=0; j<purchases.at(i).getValidMarkets().size(); j++)
			if(markets.at(marketIdx) == purchases.at(i).getValidMarkets()[j])
			{
				validPurchases.push_back(purchases.at(i).getAddr()); break;
			}
	int validPurchasesSize = validPurchases.size();
//	graph.primMinimumSpanningTree(markets.at(marketIdx), validPurchases);
/*
	int distance;
	vector<Vertex<RoadNode>* > mst = graph.incompletePrimMST(markets.at(marketIdx), validPurchases, distance);
	cout << "MST for market " << marketIdx << " has " << mst.size() << " nodes, distance " << distance << endl;
	for (int i = 0; i < mst.size(); i++)
	{
		for (int j = 0; j < validPurchases.size(); j++)
		{
			if (mst.at(i)->getInfo() == validPurchases.at(j))
				cout << validPurchases.at(j) << endl;
		}
	}*/
	graph.dijkstraShortestPath(markets.at(marketIdx));
//	vector<vector<RoadNode> > paths;
	int pathId = 1;
	while (!validPurchases.empty())
	{
		int distance;
		vector<RoadNode> path = getTruckPath(markets.at(marketIdx), validPurchases, distance);
//		paths.push_back(path);
		//print path
		cout << "Path " << pathId << ": " << validPurchasesSize - validPurchases.size() << " clients served, length is ";
		cout << distance << " meters (" << distance / 1000.0 << " Km), estimated time is " << calculateTime(distance) << " min\n";
		validPurchasesSize = validPurchases.size();
		pathId++;
	}
	return;
}

void Program::allMarketsAllClients()
{
	setClosestMarketToAllClients();
	displayClosestMarketsToClients();

	for (int i = 0; i < markets.size(); i++)
	{
		vector<RoadNode> closest;
		for (int j = 0; j < purchases.size(); j++)
		{
			if (purchases.at(j).getClosestMarketIndex() == i)
				closest.push_back(purchases.at(j).getAddr());
		}
		cout << closest.size() << " clients for market " << i << endl;
		int distance;
		vector<Vertex<RoadNode>* > mst = graph.incompletePrimMST(markets.at(i), closest, distance);
		cout << "MST for market " << i << " has " << mst.size() << " nodes, distance " << distance << "\n";
	}
}

pair<int, int> Program::mapCoordToXY(RoadNode n)
{
	float lond = abs(origin.second - xMax.second) / xRes;
	float lond2 = abs(origin.second - n.getDegLong());
	int x = static_cast<int>(lond2 / lond);

	float latd = abs(origin.first - yMax.first) / yRes;
	float latd2 = abs(origin.first - n.getDegLat());
	int y = static_cast<int>(latd2 / latd);

	return pair<int, int>(x, y);
}

void Program::resetGV()
{
	for (int i = 0; i <= lastEdgeID; i++)
		gv->removeEdge(i);
	for (int i = 0; i <= lastNodeID; i++)
		gv->removeNode(i);
	gv->rearrange();
}
