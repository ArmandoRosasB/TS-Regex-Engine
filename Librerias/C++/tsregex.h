// =================================================================
//
// Archivo: tsregex.h
// Autores: José Armando Rosas Balderas
//          Ramona Nájera Fuentes
// Descripción: Este archivo es una librería que contiene un motor 
//				de expresiones regulares a partir de la 
//              construcción de automatas finitos representados con 
//              un grafo con costo.
//
// Copyright© 2023 by TomatoStack.
//
// =================================================================

#ifndef TSREGEX_H
#define TSREGEX_H

#include <iostream>
#include <vector>
#include <string>

#include "AFD.h"

class tsregex {
    private:
        AFN* automata_finito_no_determinista;
        AFD* automata_finito_determinista;
        std::string alfabeto;
        std::string regex;

    public:
        inline const static std::string A_Z = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
        inline static const std::string MINUS_A_Z = "abcdefghijklmnopqrstuvwxyz";
        inline static const std::string MAYUS_A_Z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

        inline static const std::string DIGITS = "0123456789";
        inline static const std::string BIN = "01";

        tsregex(std::string, std::string);
        ~tsregex();
        bool match(std::string);
        vector<int> search(std::string, int i = 0);
        void replace(std::string&, std::string);
        void replaceAll(std::string&, std::string);

        WGraph<int, char>* getAFN();
        WGraph<char, char>* getAFD();

};

tsregex :: tsregex(std::string alfabeto, std::string regex){
    automata_finito_no_determinista = new AFN(alfabeto, regex);
    automata_finito_determinista = new AFD(automata_finito_no_determinista);

    this->alfabeto = alfabeto;
    this->regex = regex;
}

tsregex :: ~tsregex(){}

bool tsregex :: match(std::string cadena) {
    WGraph<char, char>* camino = automata_finito_determinista->getGrafo();
    char nodo = 'A';

    for(int i = 0; i < cadena.size(); i++) {
        if (alfabeto.find(cadena[i]) != -1) {
            nodo = camino->getConnectionsFrom(nodo, cadena[i]);
            if (nodo == '-') return false;
        } else {
            throw IllegalCharacter();
        }
    }

    set<char>:: iterator itr = automata_finito_determinista->getAceptacion().find(nodo);
    if (itr != automata_finito_determinista->getAceptacion().end()){
        return true;
    }
    return false;
}

vector<int> tsregex :: search(std::string cadena, int i) {
    WGraph<char, char>* camino = automata_finito_determinista->getGrafo();
    vector<int> pos = vector<int>(2, -1); 

    set<char>:: iterator itr;
    
    bool aceptado = false;
    char nodo = 'A';

    for(; i < cadena.size(); i++) { // Comprobar un inicio viable 
        nodo = 'A';
        
        if (alfabeto.find(cadena[i]) == -1) continue; // Verficar si el caracter pertenece al alfabeto

        itr = automata_finito_determinista->getAceptacion().find(nodo);
        
        if (camino->getConnectionsFrom(nodo, cadena[i]) == '-' &&
            itr == automata_finito_determinista->getAceptacion().end()) continue; // Verficar que no tenga conexion y si no es de aceptacion
        
        pos[0] = i;
        
        for(int j = i; j < cadena.size(); j++) {
            char movimiento = camino->getConnectionsFrom(nodo, cadena[j]);
            
            if (movimiento == '-') break;
            nodo = movimiento;

            itr = automata_finito_determinista->getAceptacion().find(nodo);
            
            if (itr != automata_finito_determinista->getAceptacion().end()) {
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


void tsregex :: replace(std::string& cadena, std::string reemplazo) {
    vector<int> pos = search(cadena);

    if(pos[0] == -1) return;

    cadena = (cadena.substr(0, pos[0])) + reemplazo + (cadena.substr(pos[1] + 1, cadena.size() - pos[1] - 1));
}


void tsregex :: replaceAll(std::string& cadena, std::string reemplazo) {
    vector<int> pos = search(cadena);

    while (pos[0] != -1 && pos[1] != -1) {
        cadena = (cadena.substr(0, pos[0])) + reemplazo + (cadena.substr(pos[1] + 1, cadena.size() - pos[1] - 1));
        pos = search(cadena, pos[0]+reemplazo.size());
    }
    
}

WGraph<int, char>* tsregex:: getAFN(){
    return automata_finito_no_determinista->getGrafo();
}

WGraph<char, char>* tsregex:: getAFD(){
    return automata_finito_determinista->getGrafo();
}

#endif // TSREGEX_H
