#include "AFD.h"

using namespace std;

int main (int argc, char* argv[]){
    AFN* prueba = new AFN("ab", "ab|ab*");
    AFD* prueba2 = new AFD(prueba);
    
    cout << "Ini: " << prueba->getInicio() << endl;
    cout << "Fin: "  << prueba->getFin() << endl << endl;
    cout << prueba->toString() << endl;
    
    cout << prueba2->toString() << endl;
    
    for(char c : prueba2->getAceptacion()){
        cout << c << " ";
    }
    
    cout << endl;

    return 0;
}