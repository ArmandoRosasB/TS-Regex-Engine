
import java.util.HashSet;
import java.util.HashMap;
import java.util.Iterator;

public class Wgraph <Vertex, Edge> {
    private boolean direction;
    private HashSet<Vertex> vertexes;
    private HashMap<Vertex, MultiMap<Vertex, Edge>> edges;

    Wgraph(boolean direction) {
        vertexes = new HashSet<Vertex>();
        edges = new HashMap<Vertex, MultiMap<Vertex, Edge>>();
        this.direction = direction;
    }

    public final boolean containsVertex (Vertex v) {
        return vertexes.contains(v);
    }

    public void addEdge(Vertex from, Vertex to, Edge cost) {
        if(containsVertex(from) == false) {
            vertexes.add(from);
            edges.put(from, new MultiMap<Vertex, Edge>());
        }

        if(containsVertex(to) == false) {
            vertexes.add(to);
            edges.put(to, new MultiMap<Vertex, Edge>());
        }

        edges.get(from).put(to, cost);

        if (!direction){
            edges.get(to).put(from, cost);
        }
    }

    public final MultiMap<Vertex, Edge> getConnectionsFrom (Vertex v) {

        MultiMap<Vertex, Edge> result = new MultiMap<Vertex, Edge>();

        try {
            result = edges.get(v);

        } catch (Exception e) {
            e.printStackTrace(); // throw NoSuchElement();
        }

        return result;
    }

    public void deleteFrom(Vertex v) {
        vertexes.remove(v);
        edges.remove(v);
    }

    public final String toString() {
        StringBuilder sb = new StringBuilder();
        Iterator<Vertex> i = vertexes.iterator();

        while (i.hasNext()){
            Vertex aux = i.next();
            sb.append(aux).append('\t');
            sb.append(edges.get(aux).toStringValues());
            sb.append(System.lineSeparator());
        }

        return sb.toString();
    }
}
