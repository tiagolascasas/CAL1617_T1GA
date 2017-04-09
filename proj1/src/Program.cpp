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
#include <functional>

#ifdef linux
#include <curses.h>
#else
#include <conio.h>
#endif

#define DEFAULT_PURCHASES 10

Program::Program(char** files): avgVelocity(30), running(true), lastEdgeID(-1), lastNodeID(-1), deliveryTime(2)
{
	loadGraph(files[1], files[2], files[3]);
	loadMarkets(files[4]);
	loadMap(files[5]);
	this->gv = new GraphViewer(xRes, yRes, false);
	while(!gv->setBackground(mapName));
	while(!gv->createWindow(xRes, yRes));
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
			displayGraphStatistics(graph);
			displayGraph(graph);
			break;
		case 2:
			displayMarketsInfo();
			break;
		case 3:
			displayPurchasesInfo();
			break;
		case 4:
			cout << "\nNumber of purchases to generate:";
			int n;
			cin >> n;
			generatePurchases(n);
			cout << n << " purchases generated\n";
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
		case 10:
			changeParameters();
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
	cout << "1.  Display the whole graph\n";
	cout << "2.  Display all markets\n";
	cout << "3.  Display all clients/purchases\n";
	cout << "4.  Generate random clients/purchases\n";
	cout << "5.  Check connectivity between all clients and all markets\n";
	cout << "6.  Distribute from a single market to a single client\n";
	cout << "7.  Distribute from all markets to a single client\n";
	cout << "8.  Distribute from a single market to all clients\n";
	cout << "9.  Distribute from all markets to all clients\n";
	cout << "10. Change delivery parameters\n";
	cout << "0.  Quit program\n";
	cout << endl;
}

void Program::displayGraph(Graph<RoadNode> g)
{
	resetGV();
	while(!gv->defineVertexColor("blue"));
	while(!gv->defineEdgeColor("black"));
	while(!gv->defineEdgeCurved(false));

	vector<pair<int, long long> > processed;
	for (int i = 0; i < g.getVertexSet().size(); i++)
	{
		pair<int, int> coord = mapCoordToXY(g.getVertexSet().at(i)->getInfo());
		while(!gv->addNode(i, coord.first, coord.second));
		while(!gv->setVertexSize(i, 5));
		if (getIndexOfMarket(g.getVertexSet().at(i)->getInfo()) != -1)
		{
			while(!gv->setVertexColor(i, RED));
			while(!gv->setVertexLabel(i, getMarketName(g.getVertexSet().at(i)->getInfo())));
		}
		processed.push_back(pair<int, long long>(i, g.getVertexSet().at(i)->getInfo().getID()));
	}
	lastNodeID = g.getVertexSet().size() - 1;

	int edgeID = 0;
	for (int i = 0; i < g.getVertexSet().size(); i++)
	{
		Vertex<RoadNode>* n = g.getVertexSet().at(i);
		for (int j = 0; j < n->getAdj().size(); j++)
		{
			int id1, id2;
			for (int k = 0; k < processed.size(); k++)
			{
				if (processed.at(k).second == n->getInfo().getID())
					id1 = processed.at(k).first;
				if (processed.at(k).second == n->getAdj().at(j).getDest()->getInfo().getID())
					id2 = processed.at(k).first;
			}
			while(!gv->addEdge(edgeID, id1, id2, EdgeType::DIRECTED));
			while(!gv->setEdgeWeight(edgeID, n->getAdj().at(j).getWeight()));
			edgeID++;
		}
	}
	lastEdgeID = edgeID;
	while(!gv->rearrange());

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
					" Km), estimated time is " << calculateTime(length, 1) << " min\n";
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
		graph.resetVisited();
		dfsConnectivity(graph.getVertex(markets.at(i)), markets.at(i));
	}
	graph.resetVisited();
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
					setprecision(2) << length / 1000.0 << " Km), estimated time is " << calculateTime(length, 1) << " min\n";
		}
	}
	catch (out_of_range &ex)
	{
		cout << "Selected client index was invalid\n";
		return;
	}
	return;
}

int Program::calculateTime(int length, int numberOfClients)
{
	float v = avgVelocity / 3.6;
	float t = length / v;
	return static_cast<int>(t / 60 + deliveryTime * numberOfClients);
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
	while(!gv->defineVertexColor("blue"));
	while(!gv->defineEdgeColor("black"));
	while(!gv->defineEdgeCurved(false));

	pair<int, int> coord = mapCoordToXY(path.at(0)->getInfo());
	while(!gv->addNode(0, coord.first, coord.second));
	while(!gv->setVertexLabel(0, getMarketName(path.at(0)->getInfo())));
	while(!gv->setVertexColor(0, RED));
	while(!gv->setVertexSize(0, 5));

	for (int i = 1; i < path.size() - 1; i++)
	{
		coord = mapCoordToXY(path.at(i)->getInfo());
		while(!gv->addNode(i, coord.first, coord.second));
		while(!gv->setVertexSize(i, 5));
		while(!gv->addEdge(i - 1, i - 1, i, EdgeType::DIRECTED));
		while(!gv->setEdgeWeight(i, path.at(i - 1)->getInfo().getDistanceBetween(path.at(i)->getInfo())));
	}
	coord = mapCoordToXY(path.at(path.size() - 1)->getInfo());
	while(!gv->addNode(path.size() - 1, coord.first, coord.second));
	while(!gv->setVertexLabel(path.size() - 1, "Destination"));
	while(!gv->setVertexColor(path.size() - 1, GREEN));
	while(!gv->setVertexSize(path.size() - 1, 5));
	while(!gv->addEdge(path.size() - 2, path.size() - 2, path.size() - 1, EdgeType::DIRECTED));
	while(!gv->setEdgeWeight(path.size() - 1,
			path.at(path.size() - 2)->getInfo().getDistanceBetween(path.at(path.size() - 1)->getInfo())));
	while(!gv->rearrange());
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
			if (dist > 0 && dist < INT_INFINITY)
				purchases.at(j).setClosestMarketIndex(i, dist);
		}
	}
}

void Program::displayClosestMarketsToClients()
{
	cout << endl;
	function<bool(Purchase,Purchase)> f1 =  [](Purchase p1, Purchase p2)
	{
		return p1.getClosestMarketIndex() < p2.getClosestMarketIndex();
	};
	sort(purchases.begin(), purchases.end(), f1);
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
	int maxIndex = -1;
	distance = 0;
	for (int i = 0; i < clients.size(); i++)
	{
		Vertex<RoadNode>* v = graph.getVertex(clients.at(i));
		if (v->getDist() > distance)
		{
			distance = v->getDist();
			farthestVertex = v;
			maxIndex = i;
		}
	}

	vector<RoadNode>* aux = new vector<RoadNode>();	//apparently vector.erase() does not work,
	for (int i = 0; i < clients.size(); i++)		//have to delete the element like this
	{
		if (i != maxIndex)
			aux->push_back(clients.at(i));
	}
	clients.clear();
	for (int i = 0; i < aux->size(); i++)
		clients.push_back(aux->at(i));
	delete aux;

	//Second part: get the path from the previous vertex to the market, and if it
	//goes through a client, remove that client from the client vector

	list<Vertex<RoadNode>* > buffer;
	buffer.push_front(farthestVertex);
	RoadNode n = farthestVertex->getInfo();

	while ( farthestVertex->path != NULL &&  farthestVertex->path->getInfo() != market)
	{
		farthestVertex = farthestVertex->path;
		clients.erase(remove(clients.begin(), clients.end(), farthestVertex->getInfo()),
						clients.end());
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
	for (int i = 0; i < purchases.size(); i++)
	{
		for (int j = 0; j < purchases.at(i).getValidMarkets().size(); j++)
		{
			if (purchases.at(i).getValidMarkets().at(j) == markets.at(marketIdx))
				validPurchases.push_back(purchases.at(i).getAddr());
		}
	}
	vector<RoadNode> backupVP = validPurchases;
	int validPurchasesSize = validPurchases.size();
	graph.dijkstraShortestPath(markets.at(marketIdx));
	int pathId = 1;
	vector<vector<RoadNode> > paths;
	while (!validPurchases.empty())
	{
		int distance;
		vector<RoadNode> path = getTruckPath(markets.at(marketIdx), validPurchases, distance);
		cout << "Path " << pathId << ": " << validPurchasesSize - validPurchases.size() << " clients served, length is ";
		cout << distance << " meters (" << distance / 1000.0 << " Km), estimated time is " <<
				calculateTime(distance, validPurchasesSize - validPurchases.size()) << " min\n";
		validPurchasesSize = validPurchases.size();
		paths.push_back(path);
		pathId++;
	}
	displaySetOfPaths(paths, backupVP);
	return;
}

void Program::allMarketsAllClients()
{
	setClosestMarketToAllClients();
	displayClosestMarketsToClients();

	for (int i = 0; i < markets.size(); i++)
	{
		cout << "\nMarket " << i + 1 << " (" << getMarketName(i) << "):\n";
		vector<RoadNode> closest;
		for (int j = 0; j < purchases.size(); j++)
		{
			if (purchases.at(j).getClosestMarketIndex() == i &&
				purchases.at(j).getClosestMarketDist() != INT_INFINITY)
				closest.push_back(purchases.at(j).getAddr());
		}
		vector<RoadNode> backupVP = closest;
		int closestSize = closest.size();
		cout << "There are " << closestSize << " clients to be served by this market\n";
		graph.dijkstraShortestPath(markets.at(i));
		int pathId = 1;
		vector<vector<RoadNode> > paths;
		vector<pair<int, int> > distTime;
		while (!closest.empty())
		{
			int distance;
			vector<RoadNode> path = getTruckPath(markets.at(i), closest, distance);
			if (distance != INT_INFINITY)
			{
				cout << "Path " << pathId << ": " << closestSize - closest.size() << " clients served, length is ";
				cout << distance << " meters (" << distance / 1000.0 << " Km), estimated time is " <<
						calculateTime(distance, closestSize - closest.size()) << " min\n";
				paths.push_back(path);
				distTime.push_back(pair<int, int>(distance, calculateTime(distance, closestSize - closest.size())));
				pathId++;
			}
			closestSize = closest.size();
		}
		analyzeData(distTime);
		displaySetOfPaths(paths, backupVP);
		cout << "Press any key to continue to the next market...";
		getch();
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
		while(!gv->removeEdge(i));
	for (int i = 0; i <= lastNodeID; i++)
		while(!gv->removeNode(i));
	lastEdgeID = -1;
	lastNodeID = -1;
	while(!gv->rearrange());
}

void Program::displaySetOfPaths(vector<vector<RoadNode> > paths, vector<RoadNode> clients)
{
	resetGV();
	vector<pair<RoadNode, int> > processed;
	while(!gv->defineVertexColor("blue"));
	while(!gv->defineEdgeColor("black"));
	while(!gv->defineEdgeCurved(false));

	pair<int, int> coord = mapCoordToXY(paths.at(0).at(0));
	while(!gv->addNode(0, coord.first, coord.second));
	while(!gv->setVertexSize(0, 5));
	int nodeID = 1;
	for (int i = 0; i < paths.size(); i++)
	{
		for (int j = 0; j < paths.at(i).size(); j++)
		{
			bool found = false;
			for (int k = 0; k < processed.size(); k++)
			{
				if (processed.at(k).first == paths.at(i).at(j))
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				coord = mapCoordToXY(paths.at(i).at(j));
				while(!gv->addNode(nodeID, coord.first, coord.second));
				if (find(clients.begin(), clients.end(), paths.at(i).at(j)) != clients.end())
				{
					while(!gv->setVertexColor(nodeID, GREEN));
					ostringstream ss;
					ss << "Path " << i + 1;
					while(!gv->setVertexLabel(nodeID, ss.str()));
				}
				while(!gv->setVertexSize(nodeID, 5));
				processed.push_back(pair<RoadNode, int>(paths.at(i).at(j), nodeID));
				nodeID++;
			}
		}
	}
	while(!gv->setVertexColor(0, RED));
	while(!gv->setVertexLabel(0, getMarketName(paths.at(0).at(0))));
	lastNodeID = nodeID;
	int edgeID = 0;
	vector<pair<int, int> > edgesProcessed;
	for (int i = 0; i < paths.size(); i++)
	{
		for (int j = 1; j < paths.at(i).size(); j++)
		{
			pair <RoadNode, int> n1, n2;
			for (int k = 0; k < processed.size(); k++)
			{
				if (processed.at(k).first == paths.at(i).at(j - 1))
					n1 = processed.at(k);
				if (processed.at(k).first == paths.at(i).at(j))
					n2 = processed.at(k);
			}
			pair <int, int> edge(n1.second, n2.second);
			vector<pair <int, int> >::iterator it = find(edgesProcessed.begin(),
														edgesProcessed.end(),
														edge);
			if (it == edgesProcessed.end())
			{
				while(!gv->addEdge(edgeID, n1.second, n2.second, EdgeType::DIRECTED));
				while(!gv->setEdgeWeight(edgeID, n1.first.getDistanceBetween(n2.first)));
				edgesProcessed.push_back(edge);
				edgeID++;
			}
		}
	}
	lastEdgeID = edgeID;
	while(!gv->rearrange());
}

void Program::changeParameters()
{
	cout << "Current average velocity: " << avgVelocity << " Km/h\n";
	cout << "Current time spent per delivery: " << deliveryTime << " min\n\n";
	cout << "New average velocity (between 5 Km/h and 80 Km/h): ";
	float v = 0;
	cin >> v;
	if (v < 5.0 || v > 80.0)
		cout << "Invalid velocity, no changes were made\n";
	else
		avgVelocity = v;
	int i = 0;
	cout << "New time per delivery (between 1 and 10): ";
	cin >> i;
	if (i < 1 || i > 10)
		cout << "Invalid time, no changes were made\n";
	else
		deliveryTime = i;
}

void Program::analyzeData(vector<pair<int, int> > distTime)
{
	int timeTotal = 0;
	int timeMax = 0;
	int distanceTotal = 0;
	for (int i = 0; i < distTime.size(); i++)
	{
		distanceTotal += distTime.at(i).first;
		timeTotal += distTime.at(i).second;
		if (distTime.at(i).second > timeMax)
			timeMax = distTime.at(i).second;
	}
	cout << "The total distance of all paths is " << distanceTotal << " meters (";
	cout << distanceTotal / 1000 << " Km)\n";
	cout << "For a single truck (assuming the return time is the same), it takes ";
	cout << timeTotal * 2 << "minutes ";
	if (timeTotal * 2 > 60)
		cout << "(" << static_cast<float>(timeTotal) / 60 << " hours) ";
	cout << "to deliver to all clients\n";
	cout << "For a maximum number of trucks departing at the same time (" << distTime.size();
	cout << " trucks), it takes " << timeMax << " minutes to deliver to all clients (not accounting for return)\n";

	cout << "Enter the number of trucks within the aforementioned numbers: ";
	int nTrucks;
	cin >> nTrucks;
	vector<vector<int> > trucks(nTrucks);
	while(timeTotal)
	{
		reverse(trucks.begin(), trucks.end());
		for (int i = 0; i < trucks.size(); i++)
		{
			pair<int, int> max(0, 0);
			int index;
			for (int j = 0; j < distTime.size(); j++)
			{
				if (distTime.at(j).second != 0 && distTime.at(j).second > max.second)
				{
					index = j;
					max = distTime.at(j);
				}
			}
			distTime.at(index) = pair<int, int>(0, 0);
			timeTotal -= max.second;
			trucks.at(i).push_back(index);
			if (timeTotal <= 0)
				break;
		}
	}
	for (int i = 0; i < trucks.size(); i++)
	{
		cout << "Truck " << i + 1 << ": path(s) ";
		for (int j = 0; j < trucks.at(i).size(); j++)
			cout << trucks.at(i).at(j) << " ";
		cout << endl;
	}

}
