import java.util.ArrayList;
import java.util.List;

/**
 * Classe genérica Node<T>
 * Representa um nó de um grafo, com valor e vizinhos.
 */
public class Node<T> {
    private final T value;
    private final List<Node<T>> neighbours;

    /** Construtor: cria nó com valor v e lista de vizinhos vazia */
    public Node(T v) {
        if (v == null) throw new IllegalArgumentException("Node value cannot be null");
        this.value = v;
        this.neighbours = new ArrayList<>();
    }

    /** Retorna o valor guardado neste nó */
    public T getValue() {
        return value;
    }

    /** Retorna os vizinhos deste nó */
    public Iterable<Node<T>> getNeighbours() {
        return neighbours;
    }

    /** Adiciona um vizinho */
    public void addNeighbour(Node<T> n) {
        if (n == null) throw new IllegalArgumentException("Null neighbour");
        neighbours.add(n);
    }
}
