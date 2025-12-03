
import java.util.ArrayList;

public class Node<T> {
    private T content;
    private ArrayList<Node<T>> neighbours;

    public Node(T content)
    {
        this.content = content;
        this.neighbours = new ArrayList<>();
    }

    public T getContent()
    {
        return this.content;
    }

    public void addNeighbour(Node n)
    {
        this.neighbours.add(n);
    }

    public Iterable<Node<T>> getNeighbours()
    {
        return this.neighbours;
    }
}
