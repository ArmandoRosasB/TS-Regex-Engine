#include "../../Librerias/C++/AFD.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
bool match(string, string, string);
vector<int> matchFirstIndex(string, string, string);

int main(int argc, char* argv[]) {

    vector<int> substr = vector<int>(2, 0);
    substr = matchFirstIndex("abcdefghijklmnopqrstuvwxyz", "class", "public class HelloWorld {}");
    cout << match("mnpq", "(mn*|p?q)+", "m") << endl;
    cout << substr[0] << ", " <<  substr[1] << endl;
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

    set<char>:: iterator itr = afd->getAceptacion().find(nodo);
    if (itr != afd->getAceptacion().end()){
        return true;
    }
    return false;
}

vector<int>  matchFirstIndex(string alfabeto, string regex, string cadena) {
    AFD* afd = new AFD(new AFN(alfabeto, regex));
    WGraph<char, char>* camino = afd->getGrafo();
    vector<int> pos = vector<int>(2, -1); 

    set<char>:: iterator itr;
    
    bool aceptado = false;
    char nodo = 'A';

    for(int i = 0; i < cadena.size(); i++) { // Comprobar un inicio viable 
        nodo = 'A';
        
        if (alfabeto.find(cadena[i]) == -1) continue; // Verficar si el caracter pertenece al alfabeto

        itr = afd->getAceptacion().find(nodo);
        
        if (camino->getConnectionsFrom(nodo, cadena[i]) == '-' &&
            itr == afd->getAceptacion().end()) continue; // Verficar que no tenga conexion y si no es de aceptacion
        
        pos[0] = i;
        
        for(int j = i; j < cadena.size(); j++) {
            char movimiento = camino->getConnectionsFrom(nodo, cadena[j]);
            
            if (movimiento == '-') break;
            nodo = movimiento;

            itr = afd->getAceptacion().find(nodo);
            
            if (itr != afd->getAceptacion().end()) {
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

