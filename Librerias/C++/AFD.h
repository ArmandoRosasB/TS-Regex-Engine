#ifndef AFD_H
#define AFD_H

#include "AFN.h"

typedef pair<int, int> automata;



class AFD {
    private:
        WGraph<char, char>* grafo;
        set<char> nodos_aceptacion;

    public:
        AFD(AFN*);
        ~AFD(void);
        WGraph<char, char>* getGrafo(void);
        set<char> getAceptacion(void);
        std::string toString(void);
};


AFD :: AFD(AFN* noDeterminista){
    
}

AFD :: ~AFD() { }

WGraph<char, char>* AFD :: getGrafo() { return grafo; } 

set<char> AFD :: getAceptacion() { return nodos_aceptacion; }

std::string AFD :: toString() { return grafo->toString(); }


#endif // AFD_H
