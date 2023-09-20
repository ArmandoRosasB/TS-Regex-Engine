
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.Scanner;

public class Main {
    private static Scanner scanner = new Scanner(System.in);
    private static String alfabeto = "";
    private static String regex = "";
    public static TSregex tsregex;

    public static void main(String[] args) throws IOException {
        String nombre = "", line = "", cadena = "", reemplazo = "";
        Scanner scanner = new Scanner(System.in);
        Runtime rt = Runtime.getRuntime();
        StringBuilder sb;
        Process proceso;
        int opcion = -1;
        int[] aux;

        System.out.print("¡Hola! ¿Cuál es tu nombre? ");
        nombre = scanner.nextLine();
        
        System.out.println("\nHola " + nombre + ". ¡Bienvenid@ al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅!");
        System.out.println("Aquí podrás hacer uso de nuestro propio motor de expresiones regulares...\n");

        System.out.println("Para comenzar, ingresa la expresión regular con la que estarás trabajando");
        System.out.println("  📌 Esta versión solo soporta los siguientes operadores * + ? | ()");
        System.out.println("  📌 Epsilon está representado con el caracter '" + Wgraph.EPSILON + "'\n");

        System.out.print("Regex: ");
        regex = scanner.nextLine();

        System.out.println("\nAhora ingresa el alfabeto que la compone");
        desplegar_Alfabeto();
        
        menuInteractivo:
        do {
            try {
                proceso = rt.exec("clear"); 

                line = "";
                sb = new StringBuilder();
                BufferedReader br = new BufferedReader(new InputStreamReader(proceso.getInputStream()));

                while ((line = br.readLine()) != null){
                    sb.append(line);
                }

                int exitVal = proceso.waitFor();

                if (exitVal == 0) {
                    System.out.println(sb.toString());
                } else {
                    System.err.println("Something went wrong :(");
                }
            } catch (Exception e) {
                e.printStackTrace();
            }

            System.out.println("\n¡Bienvenid@ al Menú Interactivo para Expresiones Regulares de Tomato Stack 🍅!\n");

            System.out.println("Alfabeto: " + alfabeto);
            System.out.println("Regex: " + regex + '\n');

            System.out.println("Selecciona alguna de las siguientes opciones...\n");

            System.out.println("🔹FUNCIONES🔹");
            System.out.println("\t1. match\n\t2. search\n\t3. replace\n\t4. replace all\n");

            System.out.println("🔹AUTOMATAS🔹");
            System.out.println("\t5. Visualizar Automata Finito No Determinista\n\t6. Visualizar Automata Finito Determinista\n");

            System.out.println("🔹CONFIGURACIÓN🔹");
            System.out.println("\t7. Cambiar alfabeto\n\t8. Cambiar regex\n\t9. Cambiar mi nombre\n");

            System.out.println("🔹SALIR🔹");
            System.out.println("\t10. Salir\n");

            System.out.println("Elige tu opción " + nombre);
            System.out.print("Opción: ");
            opcion = scanner.nextInt();
            
            System.out.println("");
            scanner.nextLine();

            switch(opcion){
                case 1: 
                    System.out.print("Ingresa la cadena a verificar: ");
                    cadena = scanner.nextLine();

                    if(tsregex.match(cadena)) {
                        System.out.println("\nLa cadena ingresada cumple con la expresión regular\n");
                    } else {
                        System.out.println("\nLa cadena ingresada no cumple con la expresión regular\n");
                    }

                    break;
                case 2: 
                    System.out.print("Ingresa la cadena a verificar: ");
                    cadena = scanner.nextLine();

                    aux = tsregex.search(cadena);

                    if(aux[0] == -1) {
                        System.out.println("\nNo hay coincidencias en la cadena proporcionada\n");
                    } else {
                        System.out.print("\nLa subcadena \"");
                        System.out.print(cadena.substring(aux[0], aux[1]+1));
                        System.out.print("\" coincide con la expresión regular ");
                        System.out.println("y se encuentra en los índices [" + aux[0] + ", " + aux[1] + "]\n");
                    }

                    break;
                case 3: 
                    System.out.print("Ingresa la cadena a verificar: ");
                    cadena = scanner.nextLine();

                    System.out.print("Ingresa el reemplazo de la primera coincidencia: ");
                    reemplazo = scanner.nextLine();

                    System.out.println("\nLa nueva cadena sería: " + tsregex.replace(cadena, reemplazo) + '\n');
                    break;
                case 4:
                    System.out.print("Ingresa la cadena a verificar: ");
                    cadena = scanner.nextLine();

                    System.out.print("Ingresa el reemplazo de las coincidencias: ");
                    reemplazo = scanner.nextLine();

                    System.out.println("\nLa nueva cadena sería: " + tsregex.replaceAll(cadena, reemplazo) + '\n');
                    break;
                case 5:
                    System.out.println(tsregex.getAFN().toString());
                    break;
                case 6:
                    System.out.println(tsregex.getAFD().toString());

                    System.out.print("Nodos aceptación: ");
                    for(Character nodo : tsregex.getAceptacion()) {
                        System.out.print(" " + nodo + " ");
                    }
                    System.out.println("\n");

                    break;
                case 7:
                    desplegar_Alfabeto();
                    break;
                case 8:
                    System.out.print("Regex: ");
                    regex = scanner.nextLine();
                    
                    tsregex = new TSregex(alfabeto, regex);
                    break;
                case 9:
                    System.out.print("Nombre: ");
                    nombre = scanner.nextLine();
                    break;
                case 10:
                    break menuInteractivo;
                default:
                    break;
            }
            
            System.out.print("¡Listo! Presiona cualquier tecla para regresar al menú...");
            new java.util.Scanner(System.in).nextLine();
        } while(true);

        System.out.println("\n¡Vuelve pronto " + nombre + "!");
        System.out.println("Atte. Tomato Stack 🍅");

        scanner.close();
    }

    public static void desplegar_Alfabeto() {
        int opcion = -1;
        
        System.out.println("Selecciona alguna de las siguientes alternativas");
        System.out.println("\t1. A-Z\n\t2. a-z\n\t3. A-Za-Z\n\t4. Binario\n\t5. Digitos\n\t6. Números y letras\n\t7. Personalizado\n");
        
        System.out.print("Opción: ");
        opcion = scanner.nextInt();

        scanner.nextLine();
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
                System.out.print("Introduce las letras de tu alfabeto separadas sin espacios: ");
                alfabeto = scanner.nextLine();
                break;
            default:
                alfabeto = TSregex.ALL;
                if(opcion != 6) System.out.print("🚨 Opción inválida, se ha asignado el alfabeto de números y letras");
                break;
        }

        tsregex = new TSregex(alfabeto, regex);
    }
}
