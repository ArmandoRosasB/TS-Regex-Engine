#include <iostream>
#include "AFN.h"

using namespace std;

int main (int argc, char* argv[]){
    AFN prueba = AFN("ab", "(($|a)b*)*");
    
    cout << prueba.getInicio() << endl;
    cout << prueba.getFin() << endl;
    cout << prueba.toString();

    return 0;
}