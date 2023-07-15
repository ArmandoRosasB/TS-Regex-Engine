#include <map>
#include <set>
#include <queue>
#include <stack>
#include "exception.h"

using namespace std;

template <class Vertex, class Edge>
class WGraph {
    private:
        bool direction;
        set<Vertex> vertexes;
        map<Vertex, multimap<Vertex, Edge>> edges;

    public:
        WGraph(bool);
        bool containsVertex(Vertex) const;
        void addEdge(Vertex, Vertex, Edge);
        multimap<Vertex, Edge> getConnectionsFrom(Vertex) const;
        void deleteFrom(Vertex);
        string toString() const;
};

template<class Vertex, class Edge>
WGraph<Vertex, Edge>::WGraph(bool direction) {
    this->direction = direction;
}

template<class Vertex, class Edge >
bool WGraph<Vertex, Edge>::containsVertex(Vertex v) const {
    return (vertexes.find(v) != vertexes.end())? true : false;
}
        

template<class Vertex, class Edge>
void WGraph<Vertex, Edge>::addEdge(Vertex from, Vertex to, Edge cost){
    if(!containsVertex(from)) {
        vertexes.insert(from);
        edges.insert(pair<Vertex, multimap<Vertex, Edge>>(from, multimap<Vertex, Edge>()));
    }

    if(!containsVertex(to)) {
        vertexes.insert(to);
        edges.insert(pair<Vertex, multimap<Vertex, Edge>>(to, multimap<Vertex, Edge>()));
    }

    edges[from].insert(pair<Vertex, Edge>(to, cost));
    if(!direction){
        edges[to].insert(pair<Vertex, Edge>(from, cost));
    }
}
        
template<class Vertex, class Edge>
multimap<Vertex, Edge> WGraph<Vertex, Edge>::getConnectionsFrom(Vertex v) const{
    if(!containsVertex(v)) {
        throw NoSuchElement();
    }

    //return edges[v];
    multimap<Vertex, Edge> result(edges.at(v).begin(), edges.at(v).end());
	return result;
}

template<class Vertex, class Edge>
void WGraph<Vertex, Edge>::deleteFrom(Vertex v) {
    vertexes.erase(v);
    edges.erase(v);
}
        
template<class Vertex, class Edge>
string WGraph<Vertex, Edge>::toString() const{
    stringstream aux;

	typename set<Vertex>::iterator i;
	typename map<Vertex, Edge>::const_iterator j;

	for(i = vertexes.begin(); i != vertexes.end(); i++) {
		aux << (*i) << "\t";
		for(j = edges.at((*i)).begin(); j != edges.at((*i)).end(); j++) {
			aux << "(" << j->first << "," << j->second << ")\t";
		}
		aux << "\n";
	}
	aux << "\n";

	return aux.str();
}


/***********************************************************/
/**************************** BFS **************************/
/***********************************************************/

template <class Vertex, class Edge>
std::set<Vertex> bfs(Vertex start, Edge& cost, WGraph<Vertex, Edge>* graph) {
	std::set<Vertex> visited;
	std::queue<Vertex> xVisit;
	typename std::map<Vertex, Edge>::iterator itr;

	xVisit.push(start);
	while (!xVisit.empty()) {
		Vertex v = xVisit.front(); xVisit.pop();
		if (visited.find(v) == visited.end()) {
			visited.insert(v);
			std::multimap<Vertex, Edge> connected = graph->getConnectionsFrom(v);
			for (itr = connected.begin(); itr != connected.end(); itr++) {
                if (itr->second == cost) xVisit.push( itr->first );
			}
		}
	}
	return visited;
}