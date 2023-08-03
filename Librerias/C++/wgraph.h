// =================================================================
//
// Archivo: wgraph.h
// Autores: José Armando Rosas Balderas
//          Ramona Nájera Fuentes
// Descripción: Este archivo es una librería que contiene la 
//              construcción de un grafo con costo a partir del
//              uso de un mapa.
//
// Copyright© 2023 by TomatoStack.
//
// =================================================================

#include <map>
#include <set>
#include <queue>
#include <stack>
#include <sstream>
#include <typeinfo>
#include "exception.h"

using namespace std;

const char EPSILON = '$';

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
        Vertex getConnectionsFrom(Vertex, Edge) const;
        void deleteFrom(Vertex);
        string toString() const;

        set<Vertex> getVertexes();
        map<Vertex, multimap<Vertex, Edge>> getEdges();
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

/* 
 * ADVERTENCIA
 * 
 * SOLO UTILIZAR ESTA FUNCION SI Y SOLO SI EN SU GRAFO EXISTE SOLO UNA
 * POSIBLE CONEXION CON EL COSTO DADO
 * 
*/
template<class Vertex, class Edge>
Vertex WGraph<Vertex, Edge>::getConnectionsFrom(Vertex v, Edge cost) const{
    if(!containsVertex(v)) {
        throw NoSuchElement();
    }

    typename multimap<Vertex, Edge>::const_iterator itr;

    for (itr = edges.at(v).begin(); itr != edges.at(v).end(); itr++) {
        if(itr->second == cost) return itr->first;
    }

	return '-';
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

template<class Vertex, class Edge>
set<Vertex> WGraph<Vertex, Edge>::getVertexes() { return vertexes; }

template<class Vertex, class Edge>
map<Vertex, multimap<Vertex, Edge>> WGraph<Vertex, Edge>::getEdges() { return edges; }



/***********************************************************/
/**************************** BFS **************************/
/***********************************************************/

template <class Vertex, class Edge>
set<Vertex> bfs(Vertex start, Edge cost, WGraph<Vertex, Edge>* graph) {
	set<Vertex> visited;
	queue<Vertex> xVisit;
	typename multimap<Vertex, Edge>::iterator itr;


    if (is_same<Edge, char>::value && cost != EPSILON) {
        multimap<Vertex, Edge> connected = graph->getConnectionsFrom(start);
        for (itr = connected.begin(); itr != connected.end(); itr++) {
            if (itr->second == cost) visited.insert( itr->first );
        }
    } else {
        xVisit.push(start);
        while (!xVisit.empty()) {
            Vertex v = xVisit.front(); xVisit.pop();
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                multimap<Vertex, Edge> connected = graph->getConnectionsFrom(v);
                for (itr = connected.begin(); itr != connected.end(); itr++) {
                    if (itr->second == cost) xVisit.push( itr->first );
                }
            }
        }
    }

	return visited;
}
