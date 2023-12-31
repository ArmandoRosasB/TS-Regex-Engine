//import tsregex.Automata;
//import tsregex.Wgraph;

import java.util.Iterator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Stack;

import java.util.LinkedList;
import java.util.Queue;

import java.util.Scanner;

import java.io.FileInputStream;
import java.io.InputStream;

import java.io.FileWriter;
import java.io.File;

public class Main {
    static char EPSILON = '$';
    static Automata<Integer, Integer> ultimo_automata = new Automata<Integer,Integer>(-1, -1);
    static Automata<Integer, Integer> automata_bloqueado = new Automata<Integer, Integer>(-1, -1);

    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.out.println("Uso: java nombre_archivo archivo_entrada archivo_salida \n");
            System.exit(-1);
        }

        File file = new File(args[1]);
        FileWriter output = new FileWriter(file, true);
        
        StringBuilder sbAFN = new StringBuilder("");
        Scanner scanner = reader(args[0]);

        String alfabeto, regex;
        alfabeto = scanner.next();
        regex = scanner.next();


        // Creamos un grafo con costo para representar el Automata Finito No Determinista
        Wgraph<Integer, Character> AFN = new Wgraph<Integer, Character>(true);

        int nodo = 0, semaforo = 0, _origen = 0;
        Stack<Character> operadores = new Stack<Character>();
        Stack<Automata<Integer, Integer>> operandos = new Stack<Automata<Integer, Integer>>();

        // ------------------------------- Autómata Finito No Determinista -------------------------------
        
        for(int i = 0; i < regex.length(); i++){
            if (regex.charAt(i) == '(') {
                operadores.push(regex.charAt(i));
                semaforo = 1;

                if(!operandos.isEmpty()) {
                    automata_bloqueado = operandos.peek();
                }

            } else if (regex.charAt(i) == '*' || regex.charAt(i) == '?' || regex.charAt(i) == '+') {
                Automata<Integer, Integer> top = operandos.pop();

                if (ultimo_automata.equals(new Automata<Integer, Integer>(-1, -1))) {
                    if (regex.charAt(i) == '*'){
                        // Agregamos un nodo de aceptación
                        AFN.addEdge(top.second, nodo, EPSILON);

                        // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                        AFN.addEdge(top.first, nodo, EPSILON);

                        //Conectamos el último nodo con el primero (1...* veces)
                        AFN.addEdge(top.second, top.first, EPSILON);

                    } 
                    else if (regex.charAt(i) == '?'){
                        // Agregamos un nodo de aceptación
                        AFN.addEdge(top.second, nodo, EPSILON);

                        // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                        AFN.addEdge(top.first, nodo, EPSILON);

                    } 
                    else if (regex.charAt(i) == '+'){
                        // Agregamos un nodo de aceptación
                        AFN.addEdge(top.second, nodo, EPSILON);

                        //Conectamos el último nodo con el primero (1...* veces)
                        AFN.addEdge(top.second, top.first, EPSILON);

                    }
                } else {
                    if (regex.charAt(i) == '*'){
                        // Agregamos un nodo de aceptación
                        AFN.addEdge(ultimo_automata.second, nodo, EPSILON);

                        // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                        AFN.addEdge(ultimo_automata.first, nodo, EPSILON);

                        // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                        AFN.addEdge(ultimo_automata.second, ultimo_automata.first, EPSILON);
                    }
                    else if (regex.charAt(i) == '?'){
                        // Agregamos un nodo de aceptación
                        AFN.addEdge(ultimo_automata.second, nodo, EPSILON);

                        // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                        AFN.addEdge(ultimo_automata.first, nodo, EPSILON);

                    } 
                    else if (regex.charAt(i) == '+'){
                        // Agregamos un nodo de aceptación
                        AFN.addEdge(ultimo_automata.second, nodo, EPSILON);

                        // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                        AFN.addEdge(ultimo_automata.second, ultimo_automata.first, EPSILON);
                    }
                    ultimo_automata = new Automata<Integer, Integer>(-1, -1);
                }
                
                operandos.push(new Automata<Integer, Integer>(top.first, nodo));
                nodo++;
                
            } else if (regex.charAt(i) == '|') {
                operadores.push('|');
                
                // Bloqueamos último autómata en operandos
                automata_bloqueado = operandos.peek();
            
            } else if (regex.charAt(i) == ')') {

                while(operadores.pop() != '(') {
                    int origen, destino;

                    Automata<Integer, Integer> segundo_automata = operandos.pop();

                    Automata<Integer, Integer> primer_automata = operandos.pop();

                    // Agregamos nodo origen
                    origen = nodo;
                    AFN.addEdge(origen, primer_automata.first, EPSILON);
                    AFN.addEdge(origen, segundo_automata.first, EPSILON);
                    nodo++;

                    // Agregamos nodo final
                    AFN.addEdge(segundo_automata.second, nodo, EPSILON);
                    AFN.addEdge(primer_automata.second, nodo, EPSILON);

                    // Agregamos nodo de aceptación
                    destino = nodo + 1;
                    AFN.addEdge(nodo, destino, EPSILON);
                    nodo += 2;

                    // Agregamos a la pila de operandos el nuevo autómata
                    operandos.push(new Automata<Integer, Integer>(origen, destino));
                    _origen = origen;
                }
                
                automata_bloqueado = new Automata<Integer, Integer>(-1, -1);
                ultimo_automata = new Automata<Integer,Integer>(_origen, operandos.peek().second);

                // Verificar si podemos concatenar autómatas
                while (operandos.size() > 1) {
                    Automata<Integer, Integer> second = operandos.pop();
                    Automata<Integer, Integer> first = operandos.pop();

                    AFN.addEdge(first.second, second.first, EPSILON);

                    operandos.push(new Automata<Integer, Integer>(first.first, second.second)); 
                }
                
            } else { // Caracter del alfabeto
                // Creamos su autómata
                AFN.addEdge(nodo, nodo+1, regex.charAt(i)); 

                // Indicamos que es el ultimo automata
                ultimo_automata = new Automata<Integer, Integer>(nodo, nodo+1);

                if (semaforo == 1){
                    semaforo++;
                    _origen = nodo;
                }
                
                // Verificar si podemos concatenar autómatas
                if (!operandos.isEmpty() && operandos.peek() != automata_bloqueado) {
                    Automata<Integer, Integer> top = operandos.pop();

                    AFN.addEdge(top.second, ultimo_automata.second, regex.charAt(i));
                    AFN.deleteFrom(ultimo_automata.first);

                    operandos.push(new Automata<Integer, Integer>(top.first, ultimo_automata.second)); 
                    ultimo_automata.first = top.second; 

                    if (semaforo == 3){
                        _origen--;
                    }
                    semaforo++;

                } else {
                    operandos.push(new Automata<Integer, Integer>(nodo, nodo + 1));
                } 

                nodo += 2;           
            }

        }

        while(operandos.size() > 1 && !operadores.isEmpty()) {
            operadores.pop();
            int origen, destino;

            Automata<Integer, Integer> segundo_automata = operandos.pop();

            Automata<Integer, Integer> primer_automata = operandos.pop();

            // Agregamos nodo origen
            origen = nodo;
            AFN.addEdge(origen, primer_automata.first, EPSILON);
            AFN.addEdge(origen, segundo_automata.first, EPSILON);
            nodo++;

            // Agregamos nodo final
            AFN.addEdge(segundo_automata.second, nodo, EPSILON);
            AFN.addEdge(primer_automata.second, nodo, EPSILON);

            // Agregamos nodo de aceptación
            destino = nodo + 1;
            AFN.addEdge(nodo, destino, EPSILON);
            nodo += 2;

            // Agregamos a la pila de operandos el nuevo autómata
            operandos.push(new Automata<Integer, Integer>(origen, destino));
        }

        ultimo_automata = new Automata<Integer, Integer>(-1, -1);
        automata_bloqueado = new Automata<Integer, Integer>(-1, -1);
        AFN.addEdge(nodo, operandos.peek().first, EPSILON);
        nodo++;
        AFN.addEdge(operandos.peek().second, nodo, EPSILON);
        
        sbAFN.append("----- Automata Finito No Determinista -----\n");
        sbAFN.append(AFN.toString()).append("\n\n\n");
        output.write(sbAFN.toString());

        // ------------------------------- Autómata Finito Determinista -------------------------------

        // Creamos un grafo con costo para representar el Automata Finito Determinista
        Wgraph<Character, Character> AFD = new Wgraph<Character, Character>(true);
        StringBuilder sbAFD = new StringBuilder("");
        char nuevo_nodo = 'A';
        int objetivo = nodo;
        
        Queue<Character> xVisitar = new LinkedList<Character>();
        HashSet<Character> nodos_aceptacion = new HashSet<Character>();
        HashMap<Character, HashSet<Integer>> equivalencias = new  HashMap<Character, HashSet<Integer>>();
        Iterator<Integer> itrEq; 

        // 1era cerradura
        equivalencias.put(nuevo_nodo, Wgraph.bfs(nodo-1, EPSILON, AFN));        
        xVisitar.add(nuevo_nodo);
        nuevo_nodo++;

        if(equivalencias.get('A').contains(objetivo)){
            nodos_aceptacion.add('A');
        }
        
        while (!xVisitar.isEmpty()){
            char nodo_actual = xVisitar.poll();
            
            for(int i = 0; i < alfabeto.length(); i++) { // Iteramos sobre el alfabeto
                if (alfabeto.charAt(i) == EPSILON) continue;

                HashSet<Integer> alcance = new HashSet<Integer>();
                
                itrEq = equivalencias.get(nodo_actual).iterator();
                while(itrEq.hasNext()) {
                    HashSet<Integer> ayudante = Wgraph.bfs(itrEq.next(), alfabeto.charAt(i), AFN);
                    alcance.addAll(ayudante);
                }

                if (alcance.isEmpty()) continue;

                HashSet<Integer> alcance_cerradura = cerradura(alcance, AFN);
                char bandera = buscar_equivalencia(alcance_cerradura, equivalencias);

                if(bandera == '-') {
                    equivalencias.put(nuevo_nodo, alcance_cerradura);
                    xVisitar.add(nuevo_nodo);
                    bandera = nuevo_nodo;

                    AFD.addEdge(nodo_actual, nuevo_nodo, alfabeto.charAt(i));
                    nuevo_nodo++;
                    
                } else {
                    AFD.addEdge(nodo_actual, bandera, alfabeto.charAt(i));
                }

                if(alcance_cerradura.contains(objetivo)){
                    nodos_aceptacion.add(bandera);
                }
            }   
        }
        
        sbAFD.append("----- Automata Finito Determinista -----\n\n");
        sbAFD.append(AFD.toString()).append("\n");
        
        sbAFD.append("Nodos de aceptación\n");
        for(char nodo_aceptado: nodos_aceptacion){
            sbAFD.append(nodo_aceptado + " ");
        }
        sbAFD.append("\n");

        output.write(sbAFD.toString());
        output.close();

        scanner.close();
    }

    public static HashSet<Integer> cerradura(HashSet<Integer> check, Wgraph<Integer, Character> AFN) {
        Iterator <Integer> itr = check.iterator();
        HashSet<Integer> alcance = new HashSet<Integer>();

        while(itr.hasNext()) {
            HashSet<Integer> ayudante = Wgraph.bfs(itr.next(), EPSILON, AFN);
            alcance.addAll(ayudante);
        }

        return alcance;
    }

    public static char buscar_equivalencia(HashSet<Integer> check, HashMap<Character, HashSet<Integer>> aux) { 
        Iterator<Character> itr = aux.keySet().iterator();
        
        while(itr.hasNext()) {
            Character c = itr.next();

            if(aux.get(c).equals(check) == true) return c;
        }

        return '-';
    }

    public static Scanner reader (String file) throws Exception{
        InputStream ins = new FileInputStream(file);
        return new Scanner(ins);
    }
}
