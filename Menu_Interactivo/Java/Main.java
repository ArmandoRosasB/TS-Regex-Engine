
import java.util.Scanner;

public class Main {
    private static String alfabeto = "";

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String nombre = "", regex = "";
        int opcion = -1;

        System.out.println("¡Hola! ¿Cuál es tu nombre?");
        nombre = scanner.nextLine();
        
        System.out.println("\nHola " + nombre + ". Bienvenido al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅");
        System.out.println("Aquí podrás hacer uso de nuestro propio motor de expresiones regulares...\n");

        System.out.println("Para comenzar, ingresa la expresión regular con la que estarás trabajando");
        System.out.println(" NOTA Esta versión solo soporta los siguientes operadores * + ? | ()");
        System.out.println("      Epsilon está representado con " + Wgraph.EPSILON + "\n");

        System.out.print("Regex: ");
        regex = scanner.nextLine();

        System.out.println("\nAhora ingresa el alfabeto que la compone");
        desplegar_Alfabeto();
        
        System.out.print("\033[H\033[2J");
        System.out.flush();
    
        menuInteractivo:
        do {
            System.out.println("\n¡Bienvenido al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅!");

            System.out.println("Selecciona alguna de las siguientes opciones...");

            System.out.println("FUNCIONES");
            System.out.println("\t1. match\n\t2. search\n\t3. replace\n\t4. replace all\n");

            System.out.println("AUTOMATAS");
            System.out.println("\t5. Visualizar Automata Finito No Determinista\n\t6. Visualizar Automata Finito Determinista\n");

            System.out.println("CONFIGURACIÓN");
            System.out.println("\t7. Cambiar alfabeto\n\t8. Cambiar regex\n\t9. Cambiar mi nombre\n");

            System.out.println("10. Salir\n");

            System.out.print("Opción");
            opcion = scanner.nextInt();

            switch(opcion){
                case 1: 
                    // TO-DO
                    break;
                case 2: 
                    // TO-DO
                    break;
                case 3: 
                    // TO-DO
                    break;
                case 4:
                    // TO-DO
                    break;
                case 5:
                    // TO-DO
                    break;
                case 6:
                    // TO-DO
                    break;
                case 7:
                    desplegar_Alfabeto();
                    break;
                case 8:
                    // TO-DO
                    break;
                case 9:
                    // TO-DO
                    break;
                case 10:
                    break menuInteractivo;
                default:
                    break;
            }

            System.out.println("Elige tu opción " + nombre);
        } while(true);

        System.out.println("¡Vuelve pronto " + nombre + "!");
        System.out.println(" Atte. Tomato Stack 🍅");

        scanner.close();
    }

    public static void desplegar_Alfabeto() {
        Scanner scanner = new Scanner(System.in);
        int opcion = -1;
        
        System.out.println("Selecciona alguna de las siguientes alternativas");
        System.out.println("\t1. A-Z\n\t2. a-z\n\t3. A-Za-Z\n\t4. Binario\n\t5. Digitos\n\t6. Números y letras\n\t7. Personalizado\n");
        
        System.out.print("Opción: ");
        opcion = scanner.nextInt();

        switch(opcion){
            case 1:
                alfabeto = TSregex.MAYUS_A_Z;
                break;
            case 2:
                alfabeto = TSregex.MINUS_A_Z;
                break;
            case 3:
                alfabeto = TSregex.A_Z;
                break;
            case 4:
                alfabeto = TSregex.BIN;
                break;
            case 5:
                alfabeto = TSregex.DIGITS;
                break;
            case 7:
                System.out.println("Introduce las letras de tu alfabeto separadas sin espacios: ");
                alfabeto = scanner.nextLine();
                break;
            default:
                alfabeto = TSregex.ALL;
                if(opcion != 6) System.out.println("Opción inválida, se ha asignado el alfabeto de números y letras");
                break;
        }

        System.out.println("");
        scanner.close();
    }
}
