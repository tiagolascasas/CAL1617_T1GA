/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <stddef.h>
#include <limits.h>
#include <list>
#include <iostream>
using namespace std;

const int INT_INFINITY = INT_MAX;

template <class T> class Edge;
template <class T> class Graph;

//------------------------------
//Vertex<T>
//------------------------------

template <class T>
class Vertex {
	T info;					/// Information on vertex's content
	vector<Edge<T>  > adj;	/// Edge that connects this vertex to another
	bool visited;			///	Keeps track whether the vertex was already visited or not
	bool processed;			/// Keeps track whether the vertex was processed or not
	int indegree;			/// Number of edges leading to this vertex
	int dist;				/// Distance to starting vertex of an algorithm
public:
	/**
	 * Creates an instance of vertex
	 * @param in vertex's info
	 */
	Vertex(T in);

	/**
	 * Gets this vertex's info
	 * @return vertex's info
	 */
	T getInfo() const;

	/**
	 * Gets the number of edges leading into the vertex
	 * @return Vertex::indegree
	 */
	int getIndegree() const;

	/**
	 * Gets the edges starting from the vertex
	 * @return vector of edges stored in Vertex::adj
	 */
	vector<Edge<T> > getAdj() const;

	/**
	 * Gets distance to starting vertex of an algorithm
	 * @return Vertex::dist
	 */
	int getDist() const;

	/**
	 * Gets the boolean value of Vertex::visited
	 * @return value of Vertex::visited
	 */
	bool getVisited() const;

	/**
	 * Sets Vertex::visited as true or false according to the visited parameter
	 * @param visited
	 */
	void setVisited(bool visited);

	/**
	 * Declares the Graph class as friend
	 * @see Graph
	 */
	friend class Graph<T>;
	Vertex<T>* path;			/// Initialy set as NULL, keeps track of the path taken to a specific vertex during some processing algorithms
};

template <class T>
struct vertex_greater_than {
    bool operator()(Vertex<T> * a, Vertex<T> * b) const {
        return a->getDist() > b->getDist();
    }
};

template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false), indegree(0), dist(0), processed(false), path(NULL){}

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
int Vertex<T>::getDist() const
{
	return dist;
}

template <class T>
bool Vertex<T>::getVisited() const
{
	return visited;
}

template <class T>
void Vertex<T>::setVisited(bool visited)
{
	this->visited = visited;
}

//------------------------------
//Edge<T>
//------------------------------

template <class T>
class Edge {
	Vertex<T> * dest;		/// Pointer to the vertex the edge leads to
	double weight;			/// Edge's weight
	int id;					/// Edge's id
public:
	/**
	 * Creates an instance of Edge
	 * @param d pointer to this edge's destiny
	 * @param w this edge's weight
	 * @param id this edge's id
	 */
	Edge(Vertex<T> *d, double w, int id);

	/**
	 * Gets the edge's id
	 * @return content of Edge::id
	 */
	int getID() const;

	/**
	 * Gets a pointer to the vertex this edge points to
	 * @return content of Edge::dest
	 */
	Vertex<T>* getDest() const;

	/**
	 * Gets the edge's weight
	 * @return weight stored in Edge::weight
	 */
	double getWeight() const;

	/**
	 * Declares the Graph class as friend
	 * @see Graph
	 */
	friend class Graph<T>;

	/**
	 * Declares the Vertex class as friend
	 * @see Vertex
	 */
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

//------------------------------
//Graph<T>
//------------------------------

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;				/// Vector containing pointers to all the vertexes in the graph
	vector<T> dfsResult;						/// Vector containing the result of the last Depth-First Search
	bool isDAGflag;								/// Set to True if this is a Directed acyclic-graph and false otherwise

public:
	/**
	 * Gets the vector containing pointers to all the vertexes of the graph
	 * @return content of Graph::vertexSet
	 */
	vector<Vertex<T> * > getVertexSet() const;

	/**
	 * Gets the amount of vertexes in the graph
	 * @return size of Graph::vertexSet
	 */
	int getNumVertex() const;

	/**
	 * Adds a vertex to the current graph based on its content
	 * @param in content of the vertex to be added
	 * @return true if successful and false if the vertex already exists
	 */
	bool addVertex(const T &in);

	/**
	 * Adds an edge to the current graph
	 * @param sourc starting point of this edge
	 * @param dest destiny of the edge
	 * @param w edge's weight
	 * @param id edge's id
	 * @return true if successful and false otherwise
	 */
	bool addEdge(const T &sourc, const T &dest, double w, int id);

	/**
	 * Adds an edge with id set to 0 to the current graph
	 * @param sourc starting point of this edge
	 * @param dest destiny of the edge
	 * @param w edge's weight
	 * @return true if successful and false otherwise
	 */
	bool addEdge(const T &sourc, const T &dest, double w);

	/**
	 * Removes a vertex from the graph
	 * @param in reference of the vertex to remove
	 * @return false if vertex doesn't exist and true otherwise
	 */
	bool removeVertex(const T &in);

	/**
	 * Removes an edge from the graph
	 * @param sourc starting vertex of the edge to be removed
	 * @param dest target vertex of the edge to be removed
	 * @return false if edge doesn't exist and true otherwise
	 */
	bool removeEdge(const T &sourc, const T &dest);

	/**
	 * Gets a vertex based on its content
	 * @param info vertex's content
	 * @return pointer to the vertex
	 */
	Vertex<T>* getVertex(const T &info);

	/**
	 * Sets Graph::isDAGflag as false and does a Depth-First Search
	 * @return content of Graph::dfsResult
	 */
	vector<T> dfs();

	/**
	 * Does a Depth-First Search updating the result in Graph::dfsResult
	 * @param v intended vertex for the dfs
	 */
	void dfs(Vertex<T>* v);

	/**
	 * Does a Breadth-first search for v
	 * @param v vertex intended for the bfs
	 * @return result of the bfs
	 */
	vector<T> bfs(Vertex<T> *v) const;

	/**
	 * Gets vertexes that aren't any edge's destiny
	 * @return vector of pointers to vertexes with Vertex::indegree set to 0
	 */
	vector<Vertex<T>*> getSources() const;

	/**
	 * Sets the Vertex::indegree to match the amount of edges that have said vertex as destiny
	 */
	void resetIndegrees();

	/**
	 * Sets Graph::isDAGflag and calls Graph::dfs()
	 * @return value of Graph::isDAGflag
	 */
	bool isDAG();

	/**
	 * Orders the vertex's contents topologically
	 * @return vector containing the vertexes topologically ordered
	 */
	vector<T> topologicalOrder();

	/**
	 * Gets the path from one vertex to another
	 * @param origin content of starting vertex for the path
	 * @param dest content of finishing vertex for the path
	 * @return vector with the contents of the vertexes in the path
	 */
	vector<T> getPath(const T &origin, const T &dest);

	/**
	 * Gets the path from one vertex to another
	 * @param origin content of starting vertex for the path
	 * @param dest content of finishing vertex for the path
	 * @return vector with the vertexes in the path
	 */
	vector<Vertex<T>* > getPathVertex(const T &origin, const T &dest);

	/**
	 * Calculates the shortest path from the first vertex for an unweighted graph
	 * @param s content of the path's finishing vertex
	 */
	void unweightedShortestPath(const T &s);

	/**
	 * Calculates the shortest path from the first vertex for a weighted graph using Dijkstra's algorithm
	 * @param s content of the path's finishing vertex
	 */
	void dijkstraShortestPath(const T &s);

	/**
	 * Calculates the shortest path from vertex s to vertex d for a weighted graph using Dijkstra's algorithm
	 * @param s content of the path's starting vertex
	 * @param d content of the path's destiny vertex
	 * @return
	 */
	int dijkstraShortestPath(const T &s, const T &d);

	/**
	 * Resets all the vertex's visited status to false
	 */
	void resetVisited();

	/**
	 * Constructs a Minimum Spanning Tree using Prim's algorithm, starting at the vertex with s as content
	 * @param s starting point for the tree
	 * @param d vector containing the destinies of the various trees to be created
	 * @return
	 */
	int primMinimumSpanningTree(const T &s, const  vector<T> &d);

	/**
	 * Constructs a Minimum Spanning Tree using Prim's algorithm, from the vertex with s as content to the vertex with d as content
	 * @param s starting point for the tree
	 * @param d finishing point for the tree
	 * @return
	 */
	int primMinimumSpanningTree(const T &s, const T &d);

	/**
	 * Constructs a Minimum Spanning Tree using Prim's algorithm
	 * @param s starting point for the tree
	 * @param elem vector containing various finishing points for the final tree
	 * @param distance from the starting point
	 * @return vector of the vertexes in the final tree
	 */
	vector<Vertex<T>* > incompletePrimMST(const T &s, vector<T> elem, int &distance);
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
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w)
{
	for (int i = 0; i < vertexSet.size(); i++)
	{
		if (vertexSet.at(i)->info == sourc)
		{
			for (int j = 0; j < vertexSet.size(); j++)
			{
				if (vertexSet.at(j)->info == dest)
				{
					Edge<T> e(vertexSet.at(j), w, 0);
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
Vertex<T>* Graph<T>::getVertex(const T &info)
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
vector<T> Graph<T>::getPath(const T &origin, const T &dest){

	list<T> buffer;
	Vertex<T>* v = getVertex(dest);

	buffer.push_front(v->info);
	while ( v->path != NULL &&  v->path->info != origin) {
		v = v->path;
		buffer.push_front(v->info);
	}
	if( v->path != NULL )
		buffer.push_front(v->path->info);


	vector<T> res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
	return res;
}

template<class T>
vector<Vertex<T>* > Graph<T>::getPathVertex(const T &origin, const T &dest){

	list<Vertex<T>* > buffer;
	Vertex<T>* v = getVertex(dest);

	buffer.push_front(v);
	while ( v->path != NULL &&  v->path->info != origin) {
		v = v->path;
		buffer.push_front(v);
	}
	if( v->path != NULL )
		buffer.push_front(v->path);


	vector<Vertex<T>* > res;
	while( !buffer.empty() ) {
		res.push_back( buffer.front() );
		buffer.pop_front();
	}
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

template<class T>
void Graph<T>::dijkstraShortestPath(const T &s)
{
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processed = false;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);
	make_heap(pq.begin(), pq.end());

	while( !pq.empty() )
	{
		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();

		for(unsigned int i = 0; i < v->adj.size(); i++)
		{
			Vertex<T>* w = v->adj[i].dest;

			if(v->dist + v->adj[i].weight < w->dist )
			{
				w->dist = v->dist + v->adj[i].weight;
				w->path = v;
				if(!w->processed)
				{
					w->processed = true;
					pq.push_back(w);
				}
				make_heap (pq.begin(),pq.end(),vertex_greater_than<T>());
			}
		}
	}
}

template <class T>
int Graph<T>::dijkstraShortestPath(const T &s, const T &d)
{
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processed = false;
	}

	Vertex<T>* v = getVertex(s);
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);
	make_heap(pq.begin(), pq.end());

	while( !pq.empty() )
	{
		v = pq.front();
		if (v->getInfo() == d)
			return v->dist;
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();

		for(unsigned int i = 0; i < v->adj.size(); i++)
		{
			Vertex<T>* w = v->adj[i].dest;

			if(v->dist + v->adj[i].weight < w->dist )
			{
				w->dist = v->dist + v->adj[i].weight;
				w->path = v;
				if(!w->processed)
				{
					w->processed = true;
					pq.push_back(w);
				}
				make_heap (pq.begin(),pq.end(),vertex_greater_than<T>());
			}
		}
	}
	return INT_INFINITY;
}

template <class T>
void Graph<T>::resetVisited()
{
	for (int i = 0; i < vertexSet.size(); i++)
		vertexSet.at(i)->visited = false;
}

template <class T>
int Graph<T>::primMinimumSpanningTree(const T &s, const T &d)
{
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processed = false;
	}
	Vertex<T>* v = getVertex(s);
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);
	make_heap(pq.begin(), pq.end());

	for (int i = 0; i < this->getNumVertex()-1; i++)
	{
		int min = INT_INFINITY, min_index;

		for (int j = 0; j < this->getNumVertex(); j++)
			if (vertexSet[j]->processed == false && vertexSet[j]->dist < min)
				min = vertexSet[j]->dist, min_index = j;
		int u = min_index;

		vertexSet[u]->processed = true;


		for (int k = 0; k < this->getNumVertex(); k++)
		{
			vector<Edge<T> > adj = vertexSet[u]->getAdj();
			int index = -1;
			for (unsigned j = 0; j < adj.size(); j++)
				if(adj[j].getDest()==vertexSet[k])
					index=j;

			if ((index!=-1) && (vertexSet[k]->processed == false) && (adj[index].getWeight() < vertexSet[k]->dist))
			{
				vertexSet[k]->path  = vertexSet[u];
				vertexSet[k]->dist = adj[index].getWeight();
			}
		}
	}
}

template <class T>
int Graph<T>::primMinimumSpanningTree(const T &s, const  vector<T> &d)
{
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processed = false;
	}


	for (unsigned k=0; k < d.size(); k++)
	{
		primMinimumSpanningTree(s, d.at(k));
	}

}

template <class T>
vector<Vertex<T>* > Graph<T>::incompletePrimMST(const T &s, vector<T> elem, int &distance)
{
	for(unsigned int i = 0; i < vertexSet.size(); i++)
	{
		vertexSet[i]->path = NULL;
		vertexSet[i]->dist = INT_INFINITY;
		vertexSet[i]->processed = false;
	}
	distance = 0;
	vector<Vertex<T>* > res;

	Vertex<T>* v = getVertex(s);
	v->dist = 0;

	vector< Vertex<T>* > pq;
	pq.push_back(v);
	make_heap(pq.begin(), pq.end());

	while( !pq.empty() )
	{
		v = pq.front();
		pop_heap(pq.begin(), pq.end());
		pq.pop_back();
		res.push_back(v);
		typename vector<T>::iterator it = find(elem.begin(), elem.end(), v->getInfo());
		if (it != elem.end())
		{
			cout << "Found " << (*it) << endl;
			elem.erase(it);
		}
		if (elem.size() == 0)
			return res;

		for(unsigned int i = 0; i < v->adj.size(); i++)
		{
			Vertex<T>* w = v->adj[i].dest;

			if(v->dist + v->adj[i].weight < w->dist )
			{
				int min = 0;
				if (w->dist < v->dist + v->adj[i].weight)
					min = w->dist;
				else
					min = v->dist + v->adj[i].weight;
				w->dist = min;
				distance += min;
				w->path = v;
				if(!w->processed)
				{
					w->processed = true;
					pq.push_back(w);
				}
				make_heap (pq.begin(),pq.end(),vertex_greater_than<T>());
			}
		}
	}
	return res;
}
#endif /* GRAPH_H_ */
