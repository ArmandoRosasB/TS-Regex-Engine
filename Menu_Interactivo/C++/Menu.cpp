#include <iostream>
#include <stdlib.h>
#include "../../Librerias/C++/tsregex.h"

using namespace std;

void desplegar_alfabeto(string&);

int main (int argc, char* argv[]){
    string regex, alfabeto;
    string nombre;
    tsregex * camino;

    int opcion;
    int eleccion_alfabeto;

    cout << "¡Hola! ¿Cuál es tu nombre? ";
    cin >> nombre;

    cout << "Hola " << nombre << ". Bienvenido al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅";
    cout << "Aquí podrás hacer uso de nuestro propio motor de expresiones regulares..." << endl;
    cout << "Para comenzar, ingresa la expresión regular con la que estarás trabajando" << endl;
    cout << "NOTA Esta versión solo soporta los siguientes operadores * + ? | ()" << endl << endl;

    cout << "Regex: "; cin >> regex;

    cout << "\nAhora ingresa el alfabeto que la compone" << endl;
    
    desplegar_alfabeto(alfabeto);

    do
    {
        system("cls");
        cout << "¡Bienvenido al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅!" << endl << endl;
        cout << "Selecciona alguna de las siguientes opciones..." << endl;
        
        cout << "FUNCIONES\n\t1. match\n\t2. search\n\t3. replace\n\t4. replace all"<< endl;
        cout << "\nAUTOMATAS\n\t5. Visualizar Automata Finito No Determinista\n\t6. Visualizar Automata Finito Determinista"<< endl;
        cout << "\nCONFIGURACION\n\t7. Cambiar alfabeto\n\t8. Cambiar regex\n\t9. Cambiar mi nombre"<< endl;
        cout << "\nSALIR\n\t10. Salir"<< endl;
        
        cout << "Elige tu opcion " << nombre << ": ";
        cin >> opcion;

        cout << "\n\n";

        switch (opcion)
        {
        case 1:
         
            break;
        
        case 2:
         
            break;
        
        case 3:
         
            break;
        
        case 4:
         
            break;
        
        case 5:
         
            break;
        
        case 6:
         
            break;
        
        case 7:
         
            break;
        
        case 8:
         
            break;
        
        case 9:
         
            break;
        
        case 10:
         
            break;
        
        default:

            break;
        }


    } while (opcion != 10);
    
    system("cls");
    cout << ""

    system("pause");
    return 0;
}


void desplegar_alfabeto(string& alfabeto){
    int eleccion_alfabeto;

    cout << "Selecciona alguna de las siguientes alternativas" << endl;
    cout << "\t1. A-Z\n\t2. a-z\n\t3. A-Za-Z\n\t4. Binario\n\t5. Digitos\n\t6. Numeros y letras\n\t7. Personalizado" << endl;
    cout << "Opción: "; cin >> eleccion_alfabeto;

    switch (eleccion_alfabeto)
    {
    case 1:
        alfabeto = tsregex :: MAYUS_A_Z;
        break;
    
    case 2:
        alfabeto = tsregex :: MINUS_A_Z;
        break;
    
    case 3:
        alfabeto = tsregex :: A_Z;
        break;
    
    case 4:
        alfabeto = tsregex :: BIN;
        break;
    
    case 5:
        alfabeto = tsregex :: DIGITS;
        break;
    
    case 6:
        alfabeto = tsregex :: ALL;
        break;
    
    case 7:
        cout << "Introduce las letras de tu alfabeto separadas sin espacios: ";
        cin >> alfabeto;
        break;
    
    default:
        alfabeto = tsregex :: ALL;
        break;
    }
}