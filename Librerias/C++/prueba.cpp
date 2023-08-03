#include "AFD.h"
#include "tsregex.h"

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

    cout << endl << endl;

    cout << "------------ Prueba tsregex -------------\n";

    tsregex* caso1 = new tsregex("mnpq", "(mn*|p?q)+");
    cout << "TSregex match: "<< caso1->match("mnnnnnnnnpqqqq") << endl;

    tsregex* caso2 = new tsregex(caso2->A_Z, "class");
    vector<int> substr = vector<int>(2, 0);
    substr = caso2->search("public class HelloWorld {}");
    cout  << "TSregex search: [" << substr[0] << ", " <<  substr[1] << "]"<< endl;


    tsregex* caso3 = new tsregex(caso3->BIN, "1*01|0+");

    string cadena = "0111";
    cout  << "TSregex replace: " << cadena << " ---> ";
    
    caso3->replace(cadena, "9876");
    cout << cadena << endl;

    tsregex* caso4 = new tsregex(caso3->BIN, "1+");

    string cadena2 = "11100011101";
    cout  << "TSregex replaceAll: " << cadena2 << " ---> ";
    
    caso4->replaceAll(cadena2, "1");
    cout << cadena2 << endl << endl;

    return 0;
}
