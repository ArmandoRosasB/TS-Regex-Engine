// =================================================================
//
// Archivo: tsregex.java
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

//package tsregex;

import java.util.HashSet;

public class TSregex {
    public final static String A_Z = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    public final static String MINUS_A_Z = "abcdefghijklmnopqrstuvwxyz";
    public final static String MAYUS_A_Z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    public final static String DIGITS = "0123456789";
    public final static String BIN = "01";

    public final static String ALL = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    private AFN automata_finito_no_determinista;
    private AFD automata_finito_determinista;
    private String alfabeto;
    private String regex;


    /*public*/ TSregex(String alfabeto, String regex){
        automata_finito_no_determinista = new AFN(alfabeto, regex);
        automata_finito_determinista = new AFD(automata_finito_no_determinista);
    
        this.alfabeto = alfabeto;
        this.regex = regex;
    }

    public boolean match(String cadena) {
        AFD afd = new AFD(new AFN(alfabeto, regex));
        Wgraph<Character, Character> camino = afd.getGrafo();
        Character nodo = 'A';
    
        for(int i = 0; i < cadena.length(); i++) {
            if (alfabeto.contains( Character.toString( cadena.charAt(i) ) )) {
                nodo = camino.getConnectionsFrom(nodo, cadena.charAt(i));
                if (nodo.equals('-')) return false;
                
            } else {
                System.err.println("IllegalCharacter");
                System.exit(1);
            }
        }
    
        if (afd.getAceptacion().contains(nodo)){
            return true;
        }
        return false;
    }
    
    public int [] search(String cadena) {
        AFD afd = new AFD(new AFN(this.alfabeto, this.regex));
        Wgraph<Character, Character> camino = afd.getGrafo();
        int [] pos = {-1, -1}; 
        
        boolean aceptado = false;
        char nodo = 'A';
    
        for(int i = 0; i < cadena.length(); i++) { // Comprobar un inicio viable 
            nodo = 'A';
            
            if (!alfabeto.contains(Character.toString(cadena.charAt(i)))) continue; // Verficar si el caracter pertenece al alfabeto

            if (camino.getConnectionsFrom(nodo, cadena.charAt(i)).equals('-') &&
                !afd.getAceptacion().contains(nodo)) continue; // Verficar que no tenga conexion y si no es de aceptacion
            
            pos[0] = i;
            
            for(int j = i; j < cadena.length(); j++) {
                char movimiento = camino.getConnectionsFrom(nodo, cadena.charAt(j));
                
                if (movimiento == '-') break;
                nodo = movimiento;
    
                
                if (afd.getAceptacion().contains(nodo)) {
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
    
    public int [] search(String cadena, int i) {
        AFD afd = new AFD(new AFN(this.alfabeto, this.regex));
        Wgraph<Character, Character> camino = afd.getGrafo();
        int [] pos = {-1, -1}; 
        
        boolean aceptado = false;
        char nodo = 'A';
    
        for(; i < cadena.length(); i++) { // Comprobar un inicio viable 
            nodo = 'A';
            
            if (!this.alfabeto.contains(Character.toString(cadena.charAt(i)))) continue; // Verficar si el caracter pertenece al alfabeto
            
            if (camino.getConnectionsFrom(nodo, cadena.charAt(i)).equals('-') &&
                !afd.getAceptacion().contains(nodo)) continue; // Verficar que no tenga conexion y si no es de aceptacion
            
            pos[0] = i;
            
            for(int j = i; j < cadena.length(); j++) {
                char movimiento = camino.getConnectionsFrom(nodo, cadena.charAt(j));
                
                if (movimiento == '-') break;
                nodo = movimiento;
    
                
                if (afd.getAceptacion().contains(nodo)) {
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
    
    public String replace(String cadena, String reemplazo) {
        int [] pos = search(cadena);
    
        if(pos[0] == -1) return cadena;
    
        cadena = (cadena.substring(0, pos[0])) + reemplazo + (cadena.substring(pos[1] + 1));

        return cadena;
    }
    
    public String replaceAll(String cadena, String reemplazo) {
        int [] pos = search(cadena);
    
        while (pos[0] != -1 && pos[1] != -1) {
            cadena = (cadena.substring(0, pos[0])) + reemplazo + (cadena.substring(pos[1] + 1));
            pos = search(cadena, pos[0] + reemplazo.length());
        }

        return cadena;
    }

    public Wgraph<Integer, Character> getAFN(){
        return automata_finito_no_determinista.getGrafo();
    }

    public Wgraph<Character, Character> getAFD(){
        return automata_finito_determinista.getGrafo();
    }

    public HashSet<Character> getAceptacion() { return automata_finito_determinista.getAceptacion(); }
}
