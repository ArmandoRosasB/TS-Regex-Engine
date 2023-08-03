// =================================================================
//
// Archivo: AFN.h
// Autores: José Armando Rosas Balderas
//          Ramona Nájera Fuentes
// Descripción: Este archivo contiene la construcción de un 
//              automata finito no determinista a partir del 
//              uso de un grafo con peso.
//
// Copyright© 2023 by TomatoStack.
//
// =================================================================

#ifndef AFN_H
#define AFN_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include "wgraph.h"

typedef pair<int, int> automata;

class AFN {
    private:
        WGraph<int, char>* grafo;
        std::string alfabeto;
        int inicio;
        int fin;

    public:
        AFN(std::string, std::string);
        ~AFN(void);
        WGraph<int, char>* getGrafo(void);
        std::string getAlfabeto(void);
        int getInicio(void);
        int getFin(void);
        std::string toString(void);
};


AFN :: AFN(std::string alfabeto, std:: string regex){
    grafo = new WGraph<int, char>(true);
    this->alfabeto = alfabeto;

    int nodo = 0;
    stack<char> operadores;
    stack<automata> operandos;
    automata ultimo_automata (-1, -1);
    automata automata_bloqueado(-1, -1);

    // ------------------------------- Autómata Finito No Determinista -------------------------------
    
    for(int i = 0; i < regex.size(); i++){
        
        if (regex[i] == '(') {
            operadores.push(regex[i]);

        } else if (regex[i] == '*' || regex[i] == '?' || regex[i] == '+') {
            automata top = operandos.top();
            operandos.pop();

            // Agregamos un nodo de aceptación
            grafo->addEdge(top.second, nodo, EPSILON);

            if (ultimo_automata == automata(-1, -1)) {
                if (regex[i] == '*'){
                    // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                    grafo->addEdge(top.first, nodo, EPSILON);

                    //Conectamos el último nodo con el primero (1...* veces)
                    grafo->addEdge(top.second, top.first, EPSILON);

                } 
                else if (regex[i] == '?'){
                    // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                    grafo->addEdge(top.first, nodo, EPSILON);

                } 
                else if (regex[i] == '+'){
                    //Conectamos el último nodo con el primero (1...* veces)
                    grafo->addEdge(top.second, top.first, EPSILON);

                }
            } else {
                if (regex[i] == '*'){
                    // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                    grafo->addEdge(ultimo_automata.first, nodo, EPSILON);

                    // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                    grafo->addEdge(ultimo_automata.second, ultimo_automata.first, EPSILON);
                }
                else if (regex[i] == '?'){
                    // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                    grafo->addEdge(ultimo_automata.first, nodo, EPSILON);
                } 
                else if (regex[i] == '+'){
                    // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                    grafo->addEdge(ultimo_automata.second, ultimo_automata.first, EPSILON);
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
                grafo->addEdge(origen, primer_automata.first, EPSILON);
                grafo->addEdge(origen, segundo_automata.first, EPSILON);
                nodo++;

                // Agregamos nodo final
                grafo->addEdge(segundo_automata.second, nodo, EPSILON);
                grafo->addEdge(primer_automata.second, nodo, EPSILON);

                // Agregamos nodo de aceptación
                destino = nodo + 1;
                grafo->addEdge(nodo, destino, EPSILON);
                nodo += 2;

                // Agregamos a la pila de operandos el nuevo autómata
                operandos.push(automata(origen, destino));
            }

            operadores.pop();
            ultimo_automata = automata(-1, -1);
            automata_bloqueado = automata(-1, -1);
            
        } else { // Caracter del alfabeto
            // Creamos su autómata
            grafo->addEdge(nodo, nodo+1, regex[i]); 

            // Indicamos que es el ultimo automata
            ultimo_automata = automata(nodo, nodo+1);
            
            // Verificar si podemos concatenar autómatas
            if (!operandos.empty() && operandos.top() != automata_bloqueado) {
                automata top = operandos.top();
                operandos.pop();

                grafo->addEdge(top.second, ultimo_automata.second, regex[i]);
                grafo->deleteFrom(ultimo_automata.first);

                operandos.push(automata(top.first, ultimo_automata.second)); 
                ultimo_automata.first = top.second; 

            } else {
                operandos.push(automata(nodo, nodo + 1));
            } 

            nodo += 2;           
        }
    }

    while(!operadores.empty()) {
        operadores.pop();
        int origen, destino;

        automata segundo_automata = operandos.top();
        operandos.pop();

        automata primer_automata = operandos.top();
        operandos.pop();

        // Agregamos nodo origen
        origen = nodo;
        grafo->addEdge(origen, primer_automata.first, EPSILON);
        grafo->addEdge(origen, segundo_automata.first, EPSILON);
        nodo++;

        // Agregamos nodo final
        grafo->addEdge(segundo_automata.second, nodo, EPSILON);
        grafo->addEdge(primer_automata.second, nodo, EPSILON);

        // Agregamos nodo de aceptación
        destino = nodo + 1;
        grafo->addEdge(nodo, destino, EPSILON);
        nodo += 2;

        // Agregamos a la pila de operandos el nuevo autómata
        operandos.push(automata(origen, destino));
    }

    ultimo_automata = automata(-1, -1);
    automata_bloqueado = automata(-1, -1);

    grafo->addEdge(nodo, operandos.top().first, EPSILON);
    this->inicio = nodo;

    nodo++;
    grafo->addEdge(operandos.top().second, nodo, EPSILON);

    this->fin = nodo;
}

AFN :: ~AFN() { }

WGraph<int, char>* AFN :: getGrafo() { return grafo; } 

std::string AFN :: getAlfabeto() { return alfabeto; }

int AFN :: getInicio() { return inicio; }

int AFN :: getFin() { return fin; }

std::string AFN :: toString() { return grafo->toString(); }

#endif // AFN_H