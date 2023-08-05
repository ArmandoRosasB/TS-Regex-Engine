#include <stdlib.h>
#include "../../Librerias/C++/tsregex.h"

#ifdef __linux__
#define LIMPIAR "clear"
#endif // __linux__
 
#ifdef __MINGW32__
#define LIMPIAR "cls"
#endif // __MINGW32_

void desplegar_alfabeto(string&);
void pausa();

int main (int argc, char* argv[]){
    string regex, alfabeto;
    string nombre, cadena, reemplazo;
    tsregex * camino;

    vector<int> posiciones;

    int opcion;
    int eleccion_alfabeto;

    cout << "¡Hola! ¿Cuál es tu nombre? ";
    cin >> nombre;

    cout << "Hola " << nombre << ". Bienvenido al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅" << endl;
    cout << "Aquí podrás hacer uso de nuestro propio motor de expresiones regulares..." << endl;
    cout << "Para comenzar, ingresa la expresión regular con la que estarás trabajando" << endl;
    cout << "  📌 Esta versión solo soporta los siguientes operadores * + ? | ()" << endl << endl;
    cout << "  📌 Epsilon está representado con el caracter '" << EPSILON << "'\n" << endl << endl;

    cout << "Regex: "; cin >> regex;

    cout << "\nAhora ingresa el alfabeto que la compone" << endl;
    
    desplegar_alfabeto(alfabeto);

    camino = new tsregex(alfabeto, regex);

    do
    {
        system(LIMPIAR);
        cout << "¡Bienvenido al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅!" << endl << endl;
        cout << "Alfabeto: " << alfabeto << endl;
        cout << "Regex: " << regex << endl << endl; 
        cout << "Selecciona alguna de las siguientes opciones..." << endl;
        
        cout << "🔹FUNCIONES🔹\n\t1. match\n\t2. search\n\t3. replace\n\t4. replace all"<< endl;
        cout << "\n🔹AUTOMATAS🔹\n\t5. Visualizar Automata Finito No Determinista\n\t6. Visualizar Automata Finito Determinista"<< endl;
        cout << "\n🔹CONFIGURACION🔹\n\t7. Cambiar alfabeto\n\t8. Cambiar regex\n\t9. Cambiar mi nombre"<< endl;
        cout << "\n🔹SALIR🔹\n\t10. Salir"<< endl;
        
        cout << "Elige tu opcion " << nombre << ": ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:

            cout << "Ingresa la cadena a analizar: ";
            cin >> cadena;

            if (camino->match(cadena)) {
                cout << "\nLa cadena ingresada cumple con la expresión regular" << endl;
            } else {
                cout << "\nLa cadena ingresada no cumple con la expresión regular" << endl;
            }
            pausa();
            break;
        
        case 2:
            cout << "Ingresa la cadena a analizar: ";
            cin >> cadena;

            posiciones = camino->search(cadena);

            if (posiciones[0] == -1){
                cout << "La cadena dada no contiene subcadenas que cumpan con la expresión regular";
            } else {
                cout << "La subcadena  "<< "\" "<< cadena.substr(posiciones[0], posiciones[1] - posiciones[0] + 1) <<"\"" <<
                " cumple con la expresion regular " << "y se ubica en los indices " << posiciones[0] << " y " << posiciones[1] << endl;
            }

            pausa();
            break;
        
        case 3:

            cout << "Ingresa la cadena a analizar: ";
            cin >> cadena;

            cout << "Ingresa la cadena reemplazo: ";
            cin >> reemplazo;

            camino->replace(cadena, reemplazo);

            cout << "Cadena resultado: " << cadena << endl;

            pausa();
            break;
        
        case 4:
            cout << "Ingresa la cadena a analizar: ";
            cin >> cadena;

            cout << "Ingresa la cadena reemplazo: ";
            cin >> reemplazo;

            camino->replaceAll(cadena, reemplazo);

            cout << "Cadena resultado: " << cadena << endl;

            pausa();
            break;
        
        case 5:
            cout << camino->getAFN()->toString();

            pausa();
            break;
        
        case 6:
            cout << camino->getAFD()->toString();
            cout << "Nodos de aceptacion: ";

            for(char c : camino->getAceptacion()){
                cout << c << " ";
            }

            cout << endl;

            pausa();
            break;
        
        case 7:
            desplegar_alfabeto(alfabeto);
            camino = new tsregex(alfabeto, regex);

            pausa();
            break;
        
        case 8:
            cout << "Ingresa la nueva regex: ";
            cin >> regex;
            camino = new tsregex(alfabeto, regex);

            pausa();
            break;
        
        case 9:
            cout << "Ingresa tu nuevo nombre: ";
            cin >> nombre;

            pausa();
            break;
        
        case 10:
            system(LIMPIAR);
            break;
        
        default:
            break;
        }


    } while (opcion != 10);

    cout << "\n¡Vuelve pronto " << nombre << "!" << endl;
    cout << "Atte. Tomato Stack 🍅";
    cout << endl;

    cin.ignore();
    do {
        cout << "\n¡Listo! Presiona cualquier tecla para salir...";
   } while (cin.get() != '\n');
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
        cout << "Opción inválida, se ha asignado el alfabeto de numeros y letras";
        alfabeto = tsregex :: ALL;
        break;
    }
}

void pausa () {
    cin.ignore();
    do {
        cout << "\n¡Listo! Presiona cualquier tecla para regresar al menú...";
   } while (cin.get() != '\n');
}