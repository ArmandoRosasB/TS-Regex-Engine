import java.util.Iterator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Queue;

public class AFD {
    private Wgraph<Character, Character> grafo;
    private HashSet<Character> nodos_aceptacion;

    private HashSet<Integer> cerradura(HashSet<Integer> check, Wgraph<Integer, Character> AFN) {
        Iterator <Integer> itr = check.iterator();
        HashSet<Integer> alcance = new HashSet<Integer>();

        while(itr.hasNext()) {
            HashSet<Integer> ayudante = Wgraph.bfs(itr.next(), Wgraph.EPSILON, AFN);
            alcance.addAll(ayudante);
        }

        return alcance;
    }

    private char buscar_equivalencia(HashSet<Integer> check, HashMap<Character, HashSet<Integer>> aux) { 
        Iterator<Character> itr = aux.keySet().iterator();
        
        while(itr.hasNext()) {
            Character c = itr.next();

            if(aux.get(c).equals(check) == true) return c;
        }

        return '-';
    }

    AFD(AFN noDeterminista) {
        // Creamos un grafo con costo para representar el Automata Finito Determinista
        this.grafo = new Wgraph<Character, Character>(true);
        this.nodos_aceptacion = new HashSet<Character>();

        char nuevo_nodo = 'A';
        int objetivo = noDeterminista.getFin();
        String alfabeto = noDeterminista.getAlfabeto();
        
        Queue<Character> xVisitar = new LinkedList<Character>();
        HashMap<Character, HashSet<Integer>> equivalencias = new  HashMap<Character, HashSet<Integer>>();
        Iterator<Integer> itrEq; 

        // 1era cerradura
        equivalencias.put(nuevo_nodo, Wgraph.bfs(noDeterminista.getInicio(), Wgraph.EPSILON, noDeterminista.getGrafo()));        
        xVisitar.add(nuevo_nodo);
        nuevo_nodo++;

        if(equivalencias.get('A').contains(objetivo)){
            nodos_aceptacion.add('A');
        }
        
        while (!xVisitar.isEmpty()){
            char nodo_actual = xVisitar.poll();
            System.out.println();
            
            for(int i = 0; i < alfabeto.length(); i++) { // Iteramos sobre el alfabeto
                if (alfabeto.charAt(i) == Wgraph.EPSILON) continue;

                HashSet<Integer> alcance = new HashSet<Integer>();
                
                itrEq = equivalencias.get(nodo_actual).iterator();
                while(itrEq.hasNext()) {
                    HashSet<Integer> ayudante = Wgraph.bfs(itrEq.next(), alfabeto.charAt(i), noDeterminista.getGrafo());
                    alcance.addAll(ayudante);
                }

                if (alcance.isEmpty()) continue;

                HashSet<Integer> alcance_cerradura = cerradura(alcance, noDeterminista.getGrafo());
                char bandera = buscar_equivalencia(alcance_cerradura, equivalencias);

                if(bandera == '-') {
                    equivalencias.put(nuevo_nodo, alcance_cerradura);
                    xVisitar.add(nuevo_nodo);
                    bandera = nuevo_nodo;

                    this.grafo.addEdge(nodo_actual, nuevo_nodo, alfabeto.charAt(i));
                    nuevo_nodo++;
                    
                } else {
                    this.grafo.addEdge(nodo_actual, bandera, alfabeto.charAt(i));
                }

                if(alcance_cerradura.contains(objetivo)){
                    nodos_aceptacion.add(bandera);
                }
            }   
        }
        
    }

    public Wgraph<Character, Character> getGrafo() { return grafo; }

    public HashSet<Character> getAceptacion() { return nodos_aceptacion; }
    
    @Override
    public String toString() { return grafo.toString(); }
}



