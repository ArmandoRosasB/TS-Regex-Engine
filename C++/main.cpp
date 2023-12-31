#include <iostream>
#include <sstream>
#include <fstream>
#include "../Librerias/C++/wgraph.h"
#include <stdlib.h>
#include <queue>

using namespace std;

typedef pair<int, int> automata;

automata ultimo_automata (-1, -1);
automata automata_bloqueado(-1, -1);

// Prototipos
set<int> cerradura(set<int>&, WGraph<int, char>*&);
char buscar_equivalencia(set<int>&, map<char, set<int>>&);

int main(int argc, char* argv[]){
    ifstream entrada;
    ofstream salida;

    if (argc != 3) {
        cout << "Uso: " << argv[0] << "./app archivo_entrada archivo_salida \n";
        return -1;
    }

    entrada.open(argv[1]);

    if (!entrada.is_open()) {
        cout << argv[0] << ": Archivo \"" << argv[1] << "\" no encontrado\n";
        return -1;
    }

    salida.open(argv[2]);

    string alfabeto, regex;
    entrada >> alfabeto >> regex;

    // Creamos un grafo con costo para representar el Automata Finito No Determinista
    WGraph<int, char>* AFN = new WGraph<int, char>(true);

    stack<char> operadores;
    stack<automata> operandos;
    int nodo = 0, semaforo = 0, _origen;

    // ------------------------------- Autómata Finito No Determinista -------------------------------
    
    for(int i = 0; i < regex.size(); i++){
        
        if (regex[i] == '(') {
            operadores.push(regex[i]);
            semaforo = 1;

            if(!operandos.empty()) {
                automata_bloqueado = operandos.top();
            }
        } else if (regex[i] == '*' || regex[i] == '?' || regex[i] == '+') {
            automata top = operandos.top();
            operandos.pop();
    
            if (ultimo_automata == automata(-1, -1)) {
                if (regex[i] == '*'){
                    AFN->addEdge(top.second, nodo, EPSILON);

                    // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                    AFN->addEdge(top.first, nodo, EPSILON);

                    //Conectamos el último nodo con el primero (1...* veces)
                    AFN->addEdge(top.second, top.first, EPSILON);

                } 
                else if (regex[i] == '?'){
                    AFN->addEdge(top.second, nodo, EPSILON);
                    
                    // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                    AFN->addEdge(top.first, nodo, EPSILON);

                } 
                else if (regex[i] == '+'){
                    // Agregamos un nodo de aceptación
                    AFN->addEdge(top.second, nodo, EPSILON);

                    //Conectamos el último nodo con el primero (1...* veces)
                    AFN->addEdge(top.second, top.first, EPSILON);

                }
            } else {
                
                if (regex[i] == '*'){
                    AFN->addEdge(ultimo_automata.second, nodo, EPSILON);

                    // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                    AFN->addEdge(ultimo_automata.first, nodo, EPSILON);

                    // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                    AFN->addEdge(ultimo_automata.second, ultimo_automata.first, EPSILON);

                    
                }
                else if (regex[i] == '?'){
                    AFN->addEdge(ultimo_automata.second, nodo, EPSILON);

                    // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                    AFN->addEdge(ultimo_automata.first, nodo, EPSILON);

                } 
                else if (regex[i] == '+'){
                    // Agregamos un nodo de aceptación
                    AFN->addEdge(ultimo_automata.second, nodo, EPSILON);

                    // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                    AFN->addEdge(ultimo_automata.second, ultimo_automata.first, EPSILON);
                }
                ultimo_automata = automata(-1, -1);
            }

            operandos.push(automata(top.first, nodo));
            nodo++;
                
        } else if (regex[i] == '|') {
            operadores.push('|');
            
            // Bloqueamos último autómata en operandos
            automata_bloqueado = operandos.top();
        
        } else if (regex[i] == ')') {
            
            while(operadores.top() != '(') {

                operadores.pop();
                int origen, destino;

                automata segundo_automata = operandos.top();
                operandos.pop();

                automata primer_automata = operandos.top();
                operandos.pop();

                // Agregamos nodo origen
                origen = nodo;
                AFN->addEdge(origen, primer_automata.first, EPSILON);
                AFN->addEdge(origen, segundo_automata.first, EPSILON);
                nodo++;

                // Agregamos nodo final
                AFN->addEdge(segundo_automata.second, nodo, EPSILON);
                AFN->addEdge(primer_automata.second, nodo, EPSILON);

                // Agregamos nodo de aceptación
                destino = nodo + 1;
                AFN->addEdge(nodo, destino, EPSILON);
                nodo += 2;

                // Agregamos a la pila de operandos el nuevo autómata
                operandos.push(automata(origen, destino));
                _origen = origen;

            }
            
            operadores.pop();
            automata_bloqueado = automata(-1, -1);
            ultimo_automata = automata(_origen, operandos.top().second);

            // Verificar si podemos concatenar autómatas
            while (operandos.size() > 1) {
                automata second = operandos.top();
                operandos.pop();

                automata first = operandos.top();
                operandos.pop();

                AFN->addEdge(first.second, second.first, EPSILON);

                operandos.push(automata(first.first, second.second)); 
            }
            //ultimo_automata = automata(-1, -1);
            
        } else { // Caracter del alfabeto
            // Creamos su autómata
            AFN->addEdge(nodo, nodo+1, regex[i]); 

            // Indicamos que es el ultimo automata
            ultimo_automata = automata(nodo, nodo+1);

            if(semaforo == 1) {
                semaforo++;
                _origen = nodo;
            }
            
            // Verificar si podemos concatenar autómatas
            if (!operandos.empty() && operandos.top() != automata_bloqueado) {
                automata top = operandos.top();
                operandos.pop();

                AFN->addEdge(top.second, ultimo_automata.second, regex[i]);
                AFN->deleteFrom(ultimo_automata.first);

                operandos.push(automata(top.first, ultimo_automata.second)); 
                ultimo_automata.first = top.second; 

                if(semaforo == 3) { 
                    _origen--;
                } 
                semaforo++;

            } else {
                operandos.push(automata(nodo, nodo + 1));
            } 

            nodo += 2;           
        }
    }

    while(!operadores.empty() && operadores.size() > 1) {
        operadores.pop();
        int origen, destino;

        automata segundo_automata = operandos.top();
        operandos.pop();

        automata primer_automata = operandos.top();
        operandos.pop();

        // Agregamos nodo origen
        origen = nodo;
        AFN->addEdge(origen, primer_automata.first, EPSILON);
        AFN->addEdge(origen, segundo_automata.first, EPSILON);
        nodo++;

        // Agregamos nodo final
        AFN->addEdge(segundo_automata.second, nodo, EPSILON);
        AFN->addEdge(primer_automata.second, nodo, EPSILON);

        // Agregamos nodo de aceptación
        destino = nodo + 1;
        AFN->addEdge(nodo, destino, EPSILON);
        nodo += 2;

        // Agregamos a la pila de operandos el nuevo autómata
        operandos.push(automata(origen, destino));
    }

    ultimo_automata = automata(-1, -1);
    automata_bloqueado = automata(-1, -1);
    AFN->addEdge(nodo, operandos.top().first, EPSILON);
    nodo++;
    AFN->addEdge(operandos.top().second, nodo, EPSILON);
    
    salida << "----- Automata Finito No Determinista -----" << endl;
    salida << AFN->toString() << endl << endl;


    // ------------------------------- Autómata Finito Determinista -------------------------------

    // Creamos un grafo con costo para representar el Automata Finito Determinista
    WGraph<char, char>* AFD = new WGraph<char, char>(true);
    char nuevo_nodo = 'A';
    int objetivo = nodo;
    
    queue<char> xVisitar;
    set<char> nodos_aceptacion;
    map<char, set<int>> equivalencias;
    set<int>::iterator itrEq;

    // 1era cerradura
    equivalencias.insert(pair<char, set<int>>(nuevo_nodo, bfs(nodo-1, EPSILON, AFN)));        
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
                set<int> ayudante = bfs(*itrEq, alfabeto[i], AFN);
                alcance.merge(ayudante);
            }
            
            if (alcance.size() == 0) continue;

            set<int> alcance_cerradura = cerradura(alcance, AFN);
            char bandera = buscar_equivalencia(alcance_cerradura, equivalencias);

            if(bandera == '-') {
                equivalencias.insert(pair<char, set<int>>(nuevo_nodo, alcance_cerradura));
                xVisitar.push(nuevo_nodo);
                bandera = nuevo_nodo;

                AFD->addEdge(nodo_actual, nuevo_nodo, alfabeto[i]);
                nuevo_nodo++;
                
            } else {
                AFD->addEdge(nodo_actual, bandera, alfabeto[i]);
            }

            if(alcance_cerradura.find(objetivo) != alcance_cerradura.end()){
                nodos_aceptacion.insert(bandera);
            }
        }   
    }
    
    salida << "----- Automata Finito Determinista -----" << endl;
    salida << AFD->toString();

    salida << "Nodos de aceptación" << endl;
    for(auto nodo_aceptado: nodos_aceptacion){
        salida << nodo_aceptado << " ";
    }
    salida << endl;

    entrada.close();
    salida.close();

    return 0;
}

set<int> cerradura(set<int>& check, WGraph<int, char>*& AFN) {
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
