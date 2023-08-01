
import java.util.HashSet;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Queue;

public class Wgraph <Vertex, Edge> {
    private boolean direction;
    private HashSet<Vertex> vertexes;
    private HashMap<Vertex, MultiMap<Vertex, Edge>> edges;

    public static char EPSILON = '$';

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

    /***********************************************************/
    /**************************** BFS **************************/
    /***********************************************************/

    public static <Vertex, Edge> HashSet<Vertex> bfs(Vertex start, Edge cost, Wgraph<Vertex, Edge> graph) {
        HashSet<Vertex> visited = new HashSet<Vertex>();
        Queue<Vertex> xVisit =  new LinkedList<Vertex>();

        if( (char) cost != '$') {
            MultiMap<Vertex, Edge> connected = graph.getConnectionsFrom(start);
            for (Vertex temporal : connected.keySet()) {
                if(connected.get(temporal).contains(cost)) visited.add(temporal);
            }
        } else {
            xVisit.add(start);
            while (!xVisit.isEmpty()) {
                Vertex v = xVisit.remove();
                
                if (!visited.contains(v)) {
                    visited.add(v);
                    MultiMap<Vertex, Edge> connected = graph.getConnectionsFrom(v);
                    for (Vertex temporal : connected.keySet()) {
                        if(connected.get(temporal).contains(cost)) xVisit.add(temporal);
                    }
                }
            }
        }
        
        return visited;
    }
}
