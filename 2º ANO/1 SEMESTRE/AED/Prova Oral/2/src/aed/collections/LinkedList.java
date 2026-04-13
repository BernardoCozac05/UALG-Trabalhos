package aed.collections;

import java.util.Iterator;
import aed.utils.TemporalAnalysisUtils;

public class LinkedList<T> {
    private class Node  {
        T item;
        Node next;

        public Node shallowCopy() {

            Node n = new Node();
            n.item = this.item;
            if(this.next != null)
            {
                n.next = this.next.shallowCopy();
            }
            else {
                n.next = null;
            }
            return n;
        }
    }
    private Node first;
    private int size;

	private class LinkedListIterator implements Iterator<T>
    {
        Node it;
        LinkedListIterator()
        {
            it = first;
        }
        public boolean hasNext() {
            return it != null;
        }

        public T next()
        {
            T result = it.item;
            it = it.next;
            return result;
        }

        public void remove(){
            throw new UnsupportedOperationException("Iterator doesn't support removal");
        }
    }

    public LinkedList()
    {
        this.first = null;
        this.size = 0;
    }
    
    public LinkedList<T> reverse()
    {
    	if(this.size <= 1) return this;
    	
    	Node currentNode = this.first.next;
    	Node previousNode = this.first;
    	Node nextNode;
    	
    	previousNode.next = null;
    	
    	while(currentNode.next != null)
    	{
    		nextNode = currentNode.next;
    		currentNode.next = previousNode;
    		
    		previousNode = currentNode;
    		
    		currentNode = nextNode;
    		
    	}
    	
    	currentNode.next = previousNode;
    	
    	this.first = currentNode;
    	
    	return this;
    }

    //adds to beginning
    public void add(T item)
    {
        Node newNode = new Node();
        newNode.item = item;
        newNode.next = this.first;

        this.first = newNode;
        this.size++;
    }

    //removes from beginning
    public T remove()
    {
    	if(this.size == 0) return null;
        T result = this.first.item;
        this.first = this.first.next;

        this.size--;
        return result;
    }

    public boolean isEmpty()
    {
        return this.size == 0;
    }

    public int size()
    {
        return this.size;
    }

    public void addAt(int index, T item) {
        if(index == 0) {
            add(item);
            return;
        }

        Node newNode = new Node();
        newNode.item = item;

        Node n = this.first.next;
        Node previous = this.first;
        index--;

        while(index != 0)
        {
            previous = n;
            n = n.next;
            index--;
        }

        newNode.next = n;
        previous.next = newNode;
    }

    public T removeAt(int index) {
        if(index == 0) {
            return remove();
        }

        Node n = this.first.next;
        Node previous = this.first;
        index--;

        while(index != 0)
        {
            previous = n;
            n = n.next;
            index--;
        }

        previous.next = n.next;

        return n.item;
    }

    public Iterator<T> iterator()
    {
        return new LinkedListIterator();
    }

    public T get(int index) {
        Node n = this.first;
        while(index != 0)
        {
            n = n.next;
            index--;
        }

        return n.item;
    }

    public void set(int index, T element) {
        Node n = this.first;
        while(index != 0)
        {
            n = n.next;
            index--;
        }
        n.item = element;
    }

    public LinkedList<T> shallowCopy() {
        LinkedList<T> copy = new LinkedList<>();
        copy.size = this.size;
        if(this.first != null)
        {
            copy.first = this.first.shallowCopy();
        }
        return copy;
    }
    


    public static void main(String[] args) {
        System.out.println("=== Testes de razão dobrada — LinkedList<T> ===");

        final int ITER = 10;
        final int REPS_MUT = 1500;


        // addAt (meio)
        System.out.println("\nMétodo: addAt (meio)");
        TemporalAnalysisUtils.runDoublingRatioTest(
            (Integer n) -> { LinkedList<Integer> b = new LinkedList<>(); for (int i=0;i<n;i++) b.add(i); return b; },
            (LinkedList<Integer> b) -> {
                int n = b.size();
                for (int r=0; r<REPS_MUT; r++) {
                    LinkedList<Integer> l = new LinkedList<>();
                    for (int i=0;i<n;i++) l.add(i);
                    l.addAt(l.size()/2, -1);
                }
            },
            ITER
        );

        // removeAt (meio)
        System.out.println("\nMétodo: removeAt (meio)");
        TemporalAnalysisUtils.runDoublingRatioTest(
            (Integer n) -> { LinkedList<Integer> b = new LinkedList<>(); for (int i=0;i<n;i++) b.add(i); return b; },
            (LinkedList<Integer> b) -> {
                int n = b.size();
                for (int r=0; r<REPS_MUT; r++) {
                    LinkedList<Integer> l = new LinkedList<>();
                    for (int i=0;i<n;i++) l.add(i);
                    l.removeAt(l.size()/2);
                }
            },
            ITER
        );

        System.out.println("\n=== Fim dos testes (LinkedList) ===");
    }
}
