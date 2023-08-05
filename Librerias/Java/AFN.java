//package tsregex;

import java.util.Stack;

public class AFN {
    private Wgraph<Integer, Character> grafo;
    private String alfabeto;
    private int inicio;
    private int fin;

    /*public*/ AFN(String alfabeto, String regex) {
        this.alfabeto = alfabeto;
        Automata<Integer, Integer> ultimo_automata = new Automata<Integer,Integer>(-1, -1);
        Automata<Integer, Integer> automata_bloqueado = new Automata<Integer, Integer>(-1, -1);
        
        // Creamos un grafo con costo para representar el Automata Finito No Determinista
        this.grafo = new Wgraph<Integer, Character>(true);

        int nodo = 0;
        Stack<Character> operadores = new Stack<Character>();
        Stack<Automata<Integer, Integer>> operandos = new Stack<Automata<Integer, Integer>>();

        for(int i = 0; i < regex.length(); i++){
            if (regex.charAt(i) == '(') {
                operadores.push(regex.charAt(i));

            } else if (regex.charAt(i) == '*' || regex.charAt(i) == '?' || regex.charAt(i) == '+') {
                Automata<Integer, Integer> top = operandos.pop();

                // Agregamos un nodo de aceptación
                grafo.addEdge(top.second, nodo, Wgraph.EPSILON);

                if (ultimo_automata.equals(new Automata<Integer, Integer>(-1, -1))) {
                    if (regex.charAt(i) == '*'){
                        // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                        grafo.addEdge(top.first, nodo, Wgraph.EPSILON);

                        //Conectamos el último nodo con el primero (1...* veces)
                        grafo.addEdge(top.second, top.first, Wgraph.EPSILON);

                    } 
                    else if (regex.charAt(i) == '?'){
                        // Conectamos el 1er nodo con el nuevo nodo (0 veces)
                        grafo.addEdge(top.first, nodo, Wgraph.EPSILON);

                    } 
                    else if (regex.charAt(i) == '+'){
                        //Conectamos el último nodo con el primero (1...* veces)
                        grafo.addEdge(top.second, top.first, Wgraph.EPSILON);

                    }
                } else {
                    if (regex.charAt(i) == '*'){
                        // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                        grafo.addEdge(ultimo_automata.first, nodo, Wgraph.EPSILON);

                        // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                        grafo.addEdge(ultimo_automata.second, ultimo_automata.first, Wgraph.EPSILON);
                    }
                    else if (regex.charAt(i) == '?'){
                        // Conectamos el inicio del ultimo_automata con el nuevo nodo (0 veces)
                        grafo.addEdge(ultimo_automata.first, nodo, Wgraph.EPSILON);
                    } 
                    else if (regex.charAt(i) == '+'){
                        // Conectamos el final del ultimo automata con el inicio del ultimo_automata (1...* veces)
                        grafo.addEdge(ultimo_automata.second, ultimo_automata.first, Wgraph.EPSILON);
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
                int auxOrigen = nodo;

                while(operadores.pop() != '(') {
                    int origen, destino;

                    Automata<Integer, Integer> segundo_automata = operandos.pop();

                    Automata<Integer, Integer> primer_automata = operandos.pop();

                    // Agregamos nodo origen
                    origen = nodo;
                    grafo.addEdge(origen, primer_automata.first, Wgraph.EPSILON);
                    grafo.addEdge(origen, segundo_automata.first, Wgraph.EPSILON);
                    nodo++;

                    // Agregamos nodo final
                    grafo.addEdge(segundo_automata.second, nodo, Wgraph.EPSILON);
                    grafo.addEdge(primer_automata.second, nodo, Wgraph.EPSILON);

                    // Agregamos nodo de aceptación
                    destino = nodo + 1;
                    grafo.addEdge(nodo, destino, Wgraph.EPSILON);
                    nodo += 2;

                    // Agregamos a la pila de operandos el nuevo autómata
                    operandos.push(new Automata<Integer, Integer>(origen, destino));
                    ultimo_automata = new Automata<Integer, Integer>(auxOrigen, destino);
                }
                
                automata_bloqueado = new Automata<Integer, Integer>(-1, -1);
                
            } else { // Caracter del alfabeto
                // Creamos su autómata
                grafo.addEdge(nodo, nodo+1, regex.charAt(i)); 

                // Indicamos que es el ultimo automata
                ultimo_automata = new Automata<Integer, Integer>(nodo, nodo+1);
                
                // Verificar si podemos concatenar autómatas
                if (!operandos.isEmpty() && operandos.peek() != automata_bloqueado) {
                    Automata<Integer, Integer> top = operandos.pop();

                    grafo.addEdge(top.second, ultimo_automata.second, regex.charAt(i));
                    grafo.deleteFrom(ultimo_automata.first);

                    operandos.push(new Automata<Integer, Integer>(top.first, ultimo_automata.second)); 
                    ultimo_automata.first = top.second; 

                } else {
                    operandos.push(new Automata<Integer, Integer>(nodo, nodo + 1));
                } 

                nodo += 2;           
            }

        }

        while(!operadores.isEmpty()) {
            operadores.pop();
            int origen, destino;

            Automata<Integer, Integer> segundo_automata = operandos.pop();

            Automata<Integer, Integer> primer_automata = operandos.pop();

            // Agregamos nodo origen
            origen = nodo;
            grafo.addEdge(origen, primer_automata.first, Wgraph.EPSILON);
            grafo.addEdge(origen, segundo_automata.first, Wgraph.EPSILON);
            nodo++;

            // Agregamos nodo final
            grafo.addEdge(segundo_automata.second, nodo, Wgraph.EPSILON);
            grafo.addEdge(primer_automata.second, nodo, Wgraph.EPSILON);

            // Agregamos nodo de aceptación
            destino = nodo + 1;
            grafo.addEdge(nodo, destino, Wgraph.EPSILON);
            nodo += 2;

            // Agregamos a la pila de operandos el nuevo autómata
            operandos.push(new Automata<Integer, Integer>(origen, destino));
        }

        ultimo_automata = new Automata<Integer, Integer>(-1, -1);
        automata_bloqueado = new Automata<Integer, Integer>(-1, -1);
        grafo.addEdge(nodo, operandos.peek().first, Wgraph.EPSILON);
        this.inicio = nodo;

        nodo++;
        grafo.addEdge(operandos.peek().second, nodo, Wgraph.EPSILON);

        this.fin = nodo;
    }
    
    public Wgraph<Integer, Character> getGrafo() { return grafo; }

    public String getAlfabeto() {return alfabeto;}
    
    public int getInicio() { return inicio; }

    public int getFin() { return fin; }
    
    @Override
    public String toString() { return grafo.toString(); }

}
