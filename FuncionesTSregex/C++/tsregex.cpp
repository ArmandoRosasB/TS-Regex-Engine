#include "../../Librerias/C++/AFD.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool match(string, string, string);
vector<int> search(string, string, string, int i = 0);
void replace(string, string, string&, string);
void replaceAll(string, string, string&, string);

const string A_Z = "abcdefghijklmnopqrstuvwxyz";
const string BIN = "01";

int main(int argc, char* argv[]) {

    cout  << "TSregex match: " << match("mnpq", "(mn*|p?q)+", "mnnnnnnnnpqqqq") << endl;
    
    vector<int> substr = vector<int>(2, 0);
    substr = search(A_Z, "class", "public class HelloWorld {}");
    
    cout  << "TSregex search: [" << substr[0] << ", " <<  substr[1] << "]"<< endl;
    
    string cadena = "0111";
    cout  << "TSregex replace: " << cadena << " ---> ";
    
    replace(BIN, "1*01|0+", cadena, "9876");
    cout << cadena << endl;


    string cadena2 = "11100011101";
    cout  << "TSregex replaceAll: " << cadena2 << " ---> ";
    
    replaceAll(BIN, "1+", cadena2, "1");
    cout << cadena2 << endl;

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

vector<int> search(string alfabeto, string regex, string cadena, int i) {
    AFD* afd = new AFD(new AFN(alfabeto, regex));
    WGraph<char, char>* camino = afd->getGrafo();
    vector<int> pos = vector<int>(2, -1); 

    set<char>:: iterator itr;
    
    bool aceptado = false;
    char nodo = 'A';

    for(; i < cadena.size(); i++) { // Comprobar un inicio viable 
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

void replace(string alfabeto, string regex, string& cadena, string reemplazo) {
    vector<int> pos = search(alfabeto, regex, cadena);

    if(pos[0] == -1) return;

    cadena = (cadena.substr(0, pos[0])) + reemplazo + (cadena.substr(pos[1] + 1, cadena.size() - pos[1] - 1));
}

void replaceAll(string alfabeto, string regex, string& cadena, string reemplazo) {
    vector<int> pos = search(alfabeto, regex, cadena);

    while (pos[0] != -1 && pos[1] != -1) {
        cadena = (cadena.substr(0, pos[0])) + reemplazo + (cadena.substr(pos[1] + 1, cadena.size() - pos[1] - 1));
        pos = search(alfabeto, regex, cadena, pos[0]+reemplazo.size());
    }
    
}
