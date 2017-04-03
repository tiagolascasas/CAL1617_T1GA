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

#define DEFAULT_PURCHASES 10

Program::Program(char** files): avgVelocity(30), running(true)
{
	this->gv = new GraphViewer(600, 600, true);
	loadGraph(files[1], files[2], files[3]);
	loadMarkets(files[4]);
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
		{
			displayGraphStatistics(graph);
			displayGraph(graph);
			break;
		}
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
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	gv->defineEdgeCurved(true);
	for (int i = 0; i < g.getVertexSet().size(); i++)
	{
		gv->addNode(g.getVertexSet().at(i)->getInfo().getID());
		gv->setVertexSize(g.getVertexSet().at(i)->getInfo().getID(), 5);
	}
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
		//	gv->setEdgeLabel(edgeID, n->getAdj().at(i).)
			edgeID++;
		}
	}
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
		vector<RoadNode> path = graph.getPath(markets.at(marketIdx), purchases.at(clientIdx).getAddr());
		if (length == INT_INFINITY)
			cout << "There is no connection between the market and the client\n";
		else
			cout << "Shortest path from market "<< marketIdx + 1 << " (" << getMarketName(marketIdx) <<
					") is " << length << " meters (" << setprecision(2) << length / 1000.0 <<
					" Km), estimated time is " << calculateTime(length) << " min\n";
	}
	catch (out_of_range &ex)
	{
		cout << "Index of one or more choices was invalid\n";
	}
	return;
}

void Program::checkValidMarkets()
{
	for (int i = 0; i < markets.size(); i++)
	{
		graph.dijkstraShortestPath(markets.at(i));
		for (int j = 0; j < purchases.size(); j++)
		{
			if (graph.getVertex(purchases.at(j).getAddr())->getDist() != INT_INFINITY)
				purchases.at(j).addValidMarket(markets.at(i));
		}
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
