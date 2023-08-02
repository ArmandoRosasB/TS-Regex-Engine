#include "../../Librerias/C++/AFD.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
bool match(string, string, string);
vector<int> matchFirstIndex(string, string, string);

int main(int argc, char* argv[]) {

    cout << match("mnpq", "((mn*)|(p?q))+", "mnnn") << endl;
    cout << matchFirstIndex("mnpq", "((mn*)|(p?q))+", "mnnn")[0] << ", " <<  matchFirstIndex("mnpq", "((mn*)|(p?q))+", "mnnn")[1] << endl;
    return 0;
}

bool match(string alfabeto, string regex, string cadena) {
    AFD* afd = new AFD(new AFN(alfabeto, regex));
    WGraph<char, char>* camino = afd->getGrafo();
    char nodo = 'A';

    for(int i = 0; i < cadena.size(); i++) {
        if (alfabeto.find(cadena[i]) != -1) {
            nodo = camino->getConnectionsFrom(nodo, cadena[i]);

            if (nodo == '-') return false;

        } else {
            throw IllegalCharacter();
        }
    }

    return (afd->getAceptacion().find(nodo) == afd->getAceptacion().end())? false : true;
}

vector<int>  matchFirstIndex(string alfabeto, string regex, string cadena) {
    AFD* afd = new AFD(new AFN(alfabeto, regex));
    WGraph<char, char>* camino = afd->getGrafo();
    vector<int> pos = vector<int>(2, -1); 
    
    bool aceptado = false;
    char nodo = 'A';

    for(int i = 0; i < cadena.size(); i++) { // Comprobar un inicio viable 
        nodo = 'A';
        
        if (alfabeto.find(cadena[i]) == -1) continue; // Verficar si el caracter pertenece al alfabeto
        if (camino->getConnectionsFrom(nodo, cadena[i]) == '-' &&
            afd->getAceptacion().find(nodo) == afd->getAceptacion().end()) continue; // Verficar que no tenga conexion y si no es de aceptacion
        else {
            pos[0] = i;
            pos[1] = i;
            
            return pos;
        }
        
        pos[0] = i;
        
        for(int j = i; j < cadena.size(); j++) {
            cout << "Hola";
            char movimiento = camino->getConnectionsFrom(nodo, cadena[j]);
            
            if (movimiento == '-') break;
            nodo = movimiento;

            if (afd->getAceptacion().find(nodo) != afd->getAceptacion().end()) {
                pos[1] = j;
                aceptado = true;
            }
        }

        if (aceptado) break;
        pos[0] = -1;
        pos[1] = -1;
    }

    return pos;
}

