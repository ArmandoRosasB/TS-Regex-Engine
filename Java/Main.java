import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        Wgraph<Character, Integer> test = new Wgraph<Character, Integer>(true);
        BufferedWriter writer;
        
        try {   
            writer = new BufferedWriter(new FileWriter("test.txt", true));

            test.addEdge('A', 'B', 1);
            test.addEdge('A', 'C', 1);
            test.addEdge('B', 'D', 2);
            test.addEdge('C', 'D', 2);
            test.addEdge('A', 'A', 2);
            test.addEdge('C', 'C', 3);

            writer.write(test.toString());
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }  
    }
}
