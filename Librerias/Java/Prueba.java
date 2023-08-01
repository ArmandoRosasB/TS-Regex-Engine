
public class Prueba {
    public static void main(String[] args) {
        AFN prueba = new AFN("ab", "(ab)|(ab*)");
        AFD prueba2 = new AFD(prueba);

        System.out.println("Ini:" + prueba.getInicio());
        System.out.println("Fin: " + prueba.getFin() + '\n');
        System.out.println(prueba);
        
        System.out.println("\n\n" + prueba2);
        System.out.println("\nNodos aceptación");
        for(Character c : prueba2.getAceptacion()){
            System.out.print(c + " ");
        }
        System.out.println("");
    }
}