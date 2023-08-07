//package tsregex;

import java.util.Objects;

public class Automata <First, Second> {
    public First first;
    public Second second;

    /*public*/ Automata(First first, Second second){
        this.first = first;
        this.second = second;
    }

    @Override
    public boolean equals(Object obj)
    {
        if (obj instanceof Automata< ? , ? >)
        {
            Automata< ? , ? > pair = (Automata< ? , ? >)obj;
            return this.first.equals(pair.first) && this.second.equals(pair.second);
        }
        return false;
    }

    @Override
    public int hashCode() {
        return Objects.hash(this.first, this.second);
    }
}
