
public class Automata <First, Second> {
    public First first;
    public Second second;

    Automata(First first, Second second){
        this.first = first;
        this.second = second;
    }

    public boolean equals (Automata<First, Second> second){
        return (second.first == this.first && second.second == this.second)? true : false;
    }
}
