
import java.util.HashSet;
import java.util.HashMap;

public class Wgraph <Vertex, Edge> {
    private boolean direction;
    private HashSet<Vertex> vertexes;
    private HashMap<Vertex, MultiMap<Vertex, Edge>> edges;

    Wgraph(boolean direction) {
        this.direction = direction;
    }

}
