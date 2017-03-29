/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <stddef.h>
#include <limits.h>
using namespace std;

const int INT_INFINITY = INT_MAX;

template <class T> class Edge;
template <class T> class Graph;

template <class T>
class Vertex {
	T info;
	vector<Edge<T>  > adj;
	bool visited;
	int indegree;
	int dist;
public:
	Vertex(T in);
	T getInfo() const;
	int getIndegree() const;
	vector<Edge<T> > getAdj() const;
	friend class Graph<T>;
	Vertex* path;
};


template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false), indegree(0), dist(0){
	path = NULL;
}

template <class T>
T Vertex<T>::getInfo() const
{
	return info;
}

template <class T>
int Vertex<T>::getIndegree() const
{
	return indegree;
}

template <class T>
vector<Edge<T> > Vertex<T>::getAdj() const
{
	return adj;
}


template <class T>
class Edge {
	Vertex<T> * dest;
	double weight;
	int id;
public:
	Edge(Vertex<T> *d, double w, int id);
	int getID() const;
	Vertex<T>* getDest() const;
	double getWeight() const;
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w, int id): dest(d), weight(w), id(id){}

template <class T>
int Edge<T>::getID() const
{
	return id;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const
{
	return dest;
}

template <class T>
double Edge<T>::getWeight() const
{
	return weight;
}

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	vector<T> dfsResult;
	bool isDAGflag;
public:
	vector<Vertex<T> * > getVertexSet() const;
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w, int id);
	bool removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);
	Vertex<T>* getVertex(const T &info) const;
	vector<T> dfs();
	void dfs(Vertex<T>* v);
	vector<T> bfs(Vertex<T> *v) const;
	vector<Vertex<T>*> getSources() const;
	void resetIndegrees();
	bool isDAG();
	vector<T> topologicalOrder();
	void unweightedShortestPath(const T &s);
};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> * > Graph<T>::getVertexSet() const {
	return vertexSet;
}

template <class T>
bool Graph<T>::addVertex(const T &in)
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->info == in)
			return false;
	}
	Vertex<T>* v = new Vertex<T>(in);
	vertexSet.push_back(v);
	return true;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w, int id)
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->info == sourc)
		{
			for (int j = 0; j < vertexSet.size(); j++)
			{
				if (vertexSet.at(j)->info == dest)
				{
					Edge<T> e(vertexSet.at(j), w, id);
					vertexSet.at(i)->adj.push_back(e);
					vertexSet.at(j)->indegree++;
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::removeVertex(const T &in)
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->info == in)
		{
			for (int j = 0; j < vertexSet.at(i)->adj.size(); j++)
				removeEdge(vertexSet.at(i)->info, vertexSet.at(i)->adj.at(j).dest->info);

			delete vertexSet.at(i);
			vertexSet.erase(vertexSet.begin() + i);
			return true;
		}
	}
	return false;
}

template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest)
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->info == sourc)
		{
			for (int j = 0; j < vertexSet.at(i)->adj.size(); j++)
			{
				if (vertexSet.at(i)->adj.at(j).dest->info == dest)
				{
					vertexSet.at(i)->adj.erase(vertexSet.at(i)->adj.begin() + j);
					for (int k = 0; j < vertexSet.size(); k++)
					{
						if (vertexSet.at(k)->info == dest)
						{
							vertexSet.at(k)->indegree--;
							break;
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

template <class T>
Vertex<T>* Graph<T>::getVertex(const T &info) const
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->getInfo() == info)
			return vertexSet.at(i);
	}
	return NULL;
}

template <class T>
vector<T> Graph<T>::dfs()
{
	isDAGflag = false;
	for (int i = 0; i < vertexSet.size(); i++)
		vertexSet.at(i)->visited = false;

	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (!vertexSet.at(i)->visited)
			dfs(vertexSet.at(i));
	}
	return dfsResult;
}

template <class T>
void Graph<T>::dfs(Vertex<T>* v)
{
	v->visited = true;
	dfsResult.push_back(v->info);

	for (int i = 0; i < v->adj.size(); i++)
	{
		if (!v->adj.at(i).dest->visited)
			dfs(v->adj.at(i).dest);
		if (v->adj.at(i).dest->info == v->info)
			isDAGflag = true;
	}
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const
{
	vector<T> res;
	queue<Vertex<T>*> q;
	q.push(v);
	v->visited = true;
	res.push_back(v->info);

	while(!q.empty())
	{
		Vertex<T>* ve = q.front();
		q.pop();
		for (int i = 0; i < ve->adj.size(); i++)
		{
			Vertex<T>* w = ve->adj.at(i).dest;
			if (!w->visited)
			{
				w->visited = true;
				res.push_back(w->info);
				q.push(w);
			}
		}
		ve->explored = true;
	}
	return res;
}

template <class T>
vector<Vertex<T>*> Graph<T>::getSources() const
{
	vector<Vertex<T>*> res;

	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->indegree == 0)
			res.push_back(vertexSet.at(i));
	}
	return res;
}

template <class T>
void Graph<T>::resetIndegrees()
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		for (int j = 0; j < vertexSet.size(); j++)
		{
			for (int k = 0; k < vertexSet.at(j)->adj.size(); k++)
			{
				if (vertexSet.at(j)->adj.at(k).dest->info == vertexSet.at(i)->info)
					vertexSet.at(i)->indegree++;
			}
		}
	}
}

template <class T>
bool Graph<T>::isDAG()
{
	isDAGflag = false;
	dfs();
	return isDAGflag;
}

template <class T>
vector<T> Graph<T>::topologicalOrder()
{
	vector<T> res;

	for (int i = 0; i < vertexSet.size(); i++)
		vertexSet.at(i)->indegree = 0;

	for (int i = 0; i < vertexSet.size(); i++)
	{
		for (int j = 0; j < vertexSet.at(i)->adj.size(); j++)
		{
			for (int k = 0; k < vertexSet.size(); k++)
			{
				if (vertexSet.at(k)->info == vertexSet.at(i)->adj.at(j).dest->info)
					vertexSet.at(k)->indegree++;
			}
		}
	}
	queue<Vertex<T>*> q;
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->indegree == 0)
			q.push(vertexSet.at(i));
	}

	while (!q.empty())
	{
		Vertex<T>* v = q.front();
		res.push_back(v->info);
		q.pop();

		for (int i = 0; i < v->adj.size(); i++)
		{
			Vertex<T>* w;
			for (int j = 0; j < vertexSet.size(); j++)
			{
				if (vertexSet.at(j)->info == v->adj.at(i).dest->info)
				{
					w = vertexSet.at(j);
					break;
				}
			}
			w->indegree--;
			if (w->indegree == 0)
				q.push(w);
		}
	}

	if (res.size() != vertexSet.size())
		res.empty();
	resetIndegrees();
	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &s) {

	for(unsigned int i = 0; i < vertexSet.size(); i++) {
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;
	queue< Vertex<T>* > q;
	q.push(v);

	while( !q.empty() ) {
		v = q.front(); q.pop();
		for(unsigned int i = 0; i < v->adj.size(); i++) {
			Vertex<T>* w = v->adj[i].dest;
			if( w->dist == INT_INFINITY ) {
				w->dist = v->dist + 1;
				w->path = v;
				q.push(w);
			}
		}
	}

}

#endif /* GRAPH_H_ */
