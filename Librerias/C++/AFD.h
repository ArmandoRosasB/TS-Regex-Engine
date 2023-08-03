// =================================================================
//
// Archivo: AFD.h
// Autores: José Armando Rosas Balderas
//          Ramona Nájera Fuentes
// Descripción: Este archivo es una librería que contiene la 
//              construcción de un automata finito determinista
//              representado con un grafo con costo.
//
// Copyright© 2023 by TomatoStack.
//
// =================================================================

#ifndef AFD_H
#define AFD_H

#include "AFN.h"

typedef pair<int, int> automata;

set<int> cerradura(set<int>&, WGraph<int, char>*);
char buscar_equivalencia(set<int>&, map<char, set<int>>&);

class AFD {
    private:
        WGraph<char, char>* grafo;
        set<char> nodos_aceptacion;

    public:
        AFD(AFN);
        AFD(AFN*);
        ~AFD(void);
        WGraph<char, char>* getGrafo(void);
        set<char> getAceptacion(void);
        std::string toString(void);
};


AFD :: AFD(AFN noDeterminista){
    // Creamos un grafo con costo para representar el Automata Finito Determinista
    grafo = new WGraph<char, char>(true);
    char nuevo_nodo = 'A';
    
    int objetivo = noDeterminista.getFin();
    string alfabeto = noDeterminista.getAlfabeto();
    
    queue<char> xVisitar;
    map<char, set<int>> equivalencias;
    set<int>::iterator itrEq;

    // 1era cerradura
    equivalencias.insert(pair<char, set<int>>(nuevo_nodo, bfs(noDeterminista.getInicio(), EPSILON, noDeterminista.getGrafo())));        
    xVisitar.push(nuevo_nodo);
    nuevo_nodo++;

    if(equivalencias['A'].find(objetivo) != equivalencias['A'].end()){
        nodos_aceptacion.insert('A');
    }
   
    while (!xVisitar.empty()){
        char nodo_actual = xVisitar.front();
        xVisitar.pop();
        
        for(int i = 0; i < alfabeto.size(); i++) { // Iteramos sobre el alfabeto
            if (alfabeto[i] == EPSILON) continue;

            set<int> alcance;
            
            for(itrEq = equivalencias[nodo_actual].begin(); itrEq != equivalencias[nodo_actual].end(); itrEq++) {
                set<int> ayudante = bfs(*itrEq, alfabeto[i], noDeterminista.getGrafo());
                alcance.merge(ayudante);
            }

            if (alcance.size() == 0) continue;
 
            set<int> alcance_cerradura = cerradura(alcance, noDeterminista.getGrafo());
            char bandera = buscar_equivalencia(alcance_cerradura, equivalencias);

            if(bandera == '-') {
              
                equivalencias.insert(pair<char, set<int>>(nuevo_nodo, alcance_cerradura));
                xVisitar.push(nuevo_nodo);
                bandera = nuevo_nodo;

                grafo->addEdge(nodo_actual, nuevo_nodo, alfabeto[i]);
                nuevo_nodo++;
                
            } else {
                grafo->addEdge(nodo_actual, bandera, alfabeto[i]);
            }

            if(alcance_cerradura.find(objetivo) != alcance_cerradura.end()){
                nodos_aceptacion.insert(bandera);
            }
        }   

    }

}

AFD :: AFD(AFN* noDeterminista){
    // Creamos un grafo con costo para representar el Automata Finito Determinista
    grafo = new WGraph<char, char>(true);
    char nuevo_nodo = 'A';
    
    int objetivo = noDeterminista->getFin();
    string alfabeto = noDeterminista->getAlfabeto();
    
    queue<char> xVisitar;
    map<char, set<int>> equivalencias;
    set<int>::iterator itrEq;

    // 1era cerradura
    equivalencias.insert(pair<char, set<int>>(nuevo_nodo, bfs(noDeterminista->getInicio(), EPSILON, noDeterminista->getGrafo())));   
    xVisitar.push(nuevo_nodo);
    nuevo_nodo++;

    if(equivalencias['A'].find(objetivo) != equivalencias['A'].end()){
        nodos_aceptacion.insert('A');
    }

    while (!xVisitar.empty()){
        char nodo_actual = xVisitar.front();
        xVisitar.pop();
        
        for(int i = 0; i < alfabeto.size(); i++) { // Iteramos sobre el alfabeto
            if (alfabeto[i] == EPSILON) continue;

            set<int> alcance;
            
            for(itrEq = equivalencias[nodo_actual].begin(); itrEq != equivalencias[nodo_actual].end(); itrEq++) {
                set<int> ayudante = bfs(*itrEq, alfabeto[i], noDeterminista->getGrafo());
                alcance.merge(ayudante);
            }

            if (alcance.size() == 0) continue;

            set<int> alcance_cerradura = cerradura(alcance, noDeterminista->getGrafo());
            char bandera = buscar_equivalencia(alcance_cerradura, equivalencias);

            if(bandera == '-') {
                equivalencias.insert(pair<char, set<int>>(nuevo_nodo, alcance_cerradura));
                xVisitar.push(nuevo_nodo);
                bandera = nuevo_nodo;

                grafo->addEdge(nodo_actual, nuevo_nodo, alfabeto[i]);
                nuevo_nodo++;
                
            } else {
                grafo->addEdge(nodo_actual, bandera, alfabeto[i]);
            }

            if(alcance_cerradura.find(objetivo) != alcance_cerradura.end()){
                nodos_aceptacion.insert(bandera);
            }
        }   

    }

}

AFD :: ~AFD() { }

WGraph<char, char>* AFD :: getGrafo() { return grafo; } 

set<char> AFD :: getAceptacion() { return nodos_aceptacion; }

std::string AFD :: toString() {return grafo->toString(); }




set<int> cerradura(set<int>& check, WGraph<int, char>* AFN) {
    set<int>::iterator itr;
    set<int> alcance;

    for(itr = check.begin(); itr != check.end(); itr++) {
        set<int> ayudante = bfs(*itr, EPSILON, AFN);
        alcance.merge(ayudante);
    }

    return alcance;
}

char buscar_equivalencia(set<int>& check, map<char, set<int>>& aux) { 
    map<char, set<int>>::iterator itr;
    itr = aux.begin();
    while(itr != aux.end()) {
        if(itr->second == check) return itr->first;
        itr++;

    }

    return '-';
}

#endif // AFD_H
