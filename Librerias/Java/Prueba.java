//package tsregex;

public class Prueba {
    public static void main(String[] args) {
        AFN prueba = new AFN("tsregex", "(ts)+((regex)*)");
        AFD prueba2 = new AFD(prueba);

        System.out.println("Ini:" + prueba.getInicio());
        System.out.println("Fin: " + prueba.getFin() + '\n');
        System.out.println(prueba);
        
        System.out.println("\n\n" + prueba2);
        System.out.println("\nNodos aceptación");
        for(Character c : prueba2.getAceptacion()){
            System.out.print(c + " ");
        }
        System.out.println("\n\n");

        System.out.println("------------ Prueba tsregex -------------");

        TSregex caso1 = new TSregex("mnpq", "(mn*|p?q)+");
        System.out.println("TSregex match: "+ caso1.match("mnnnnnnnnpqqqq"));

        TSregex caso2 = new TSregex(TSregex.A_Z, "class");
        int[] substr;

        substr = caso2.search("public class HelloWorld {}");
        System.out.println( "TSregex search: [" + substr[0] + ", " +  substr[1] + "]");

        TSregex caso3 = new TSregex(TSregex.BIN, "1*01|0+");

        String cadena = "0111";
        System.out.print( "TSregex replace: " + cadena + " ---> ");
        
        cadena = caso3.replace(cadena, "9876");
        System.out.println(cadena);

        TSregex caso4 = new TSregex(TSregex.BIN, "1+");

        String cadena2 = "11100011101";
        System.out.print( "TSregex replaceAll: " + cadena2 + " ---> ");
        
        cadena2 = caso4.replaceAll(cadena2, "1");
        System.out.println(cadena2 + '\n');
    }
}