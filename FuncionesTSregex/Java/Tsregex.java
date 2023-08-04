//import tsregex.*;

public class Tsregex {
    final static String A_Z = "abcdefghijklmnopqrstuvwxyz";
    final static String BIN = "01";
    public static void main(String[] args) {
        boolean m = match("mnpq", "(mn*|p?q)+", "mnnnnnnnnppqqqq");
        System.out.println("TSregex match: " + m);

        int [] substr;
        substr = search(A_Z, "class", "class public class HelloWorld {}");
        
        System.out.println("TSregex search: [" + substr[0] + ", " +  substr[1] + "]");
        
        String cadena = "0111";
        System.out.print("TSregex replace: " + cadena + " ---> ");
        
        cadena = replace(BIN, "101|0+", cadena, "9876");
        System.out.println(cadena);


        String cadena2 = "11100011101";
        System.out.print("TSregex replaceAll: " + cadena2 + " ---> ");
        
        cadena2 = replaceAll(BIN, "1+", cadena2, "1");
        System.out.println( cadena2 );
    }

    public static boolean match(String alfabeto, String regex, String cadena) {
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
    
    public static int [] search(String alfabeto, String regex, String cadena) {
        AFD afd = new AFD(new AFN(alfabeto, regex));
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
    
    public static int [] search(String alfabeto, String regex, String cadena, int i) {
        AFD afd = new AFD(new AFN(alfabeto, regex));
        Wgraph<Character, Character> camino = afd.getGrafo();
        int [] pos = {-1, -1}; 
        
        boolean aceptado = false;
        char nodo = 'A';
    
        for(; i < cadena.length(); i++) { // Comprobar un inicio viable 
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
    
    public static String replace(String alfabeto, String regex, String cadena, String reemplazo) {
        int [] pos = search(alfabeto, regex, cadena);
    
        if(pos[0] == -1) return cadena;
    
        cadena = (cadena.substring(0, pos[0])) + reemplazo + (cadena.substring(pos[1] + 1));

        return cadena;
    }
    
    public static String replaceAll(String alfabeto, String regex, String cadena, String reemplazo) {
        int [] pos = search(alfabeto, regex, cadena);
    
        while (pos[0] != -1 && pos[1] != -1) {
            cadena = (cadena.substring(0, pos[0])) + reemplazo + (cadena.substring(pos[1] + 1));
            pos = search(alfabeto, regex, cadena, pos[0] + reemplazo.length());
        }

        return cadena;
    }
}
