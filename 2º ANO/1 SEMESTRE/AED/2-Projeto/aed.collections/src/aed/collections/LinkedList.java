package aed.collections;

import java.util.Iterator;

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
        // Bench para LinkedList<T>: addAt / removeAt
        java.util.Random RNG = new java.util.Random(42);

        // tamanhos (doubling test)
        int[] sizes = {1000, 2000, 4000, 8000, 16000, 32000};

        // tempos médios (ns) por tamanho
        double[] tAdd = new double[sizes.length];
        double[] tRem = new double[sizes.length];

        int trials = 50; // medições por tamanho

        // ---------- medir ----------
        for (int idx = 0; idx < sizes.length; idx++) {
            int n = sizes[idx];

            // constrói lista base com n elementos (0..n-1)
            LinkedList<Integer> S = new LinkedList<>();
            for (int i = 0; i < n; i++) S.add(i);

            // addAt em índice aleatório
            {
                long sum = 0L;
                for (int t = 0; t < trials; t++) {
                    int i = RNG.nextInt(S.size() + 1); // 0..size
                    long t0 = System.nanoTime();
                    S.addAt(i, Integer.valueOf(-1));
                    long t1 = System.nanoTime();
                    sum += (t1 - t0);

                    // manter ~n estável (remove no tail)
                    S.removeAt(S.size() - 1);
                }
                tAdd[idx] = sum / (double) trials;
            }

            // removeAt em índice aleatório
            {
                long sum = 0L;
                for (int t = 0; t < trials; t++) {
                    int j = RNG.nextInt(S.size() + 1);
                    S.addAt(j, Integer.valueOf(-2));    // marcador temporário
                    int i = RNG.nextInt(S.size());      // 0..size-1
                    long t0 = System.nanoTime();
                    S.removeAt(i);
                    long t1 = System.nanoTime();
                    sum += (t1 - t0);
                }
                tRem[idx] = sum / (double) trials;
            }
        }

        // ---------- saída no formato pedido ----------
        System.out.println();
        System.out.println("tempos médios por operação (µs) e razão dobrada r = T(2n)/T(n)");
        System.out.println();

        double sumRAdd = 0.0, sumRRem = 0.0;
        int cntRAdd = 0, cntRRem = 0;

        for (int idx = 0; idx < sizes.length; idx++) {
            // ns -> µs
            final double add_us = tAdd[idx] / 1_000.0;
            final double rem_us = tRem[idx] / 1_000.0;

            // r = T(2n)/T(n); primeira linha 0.00
            double rA = 0.0, rR = 0.0;
            if (idx > 0) {
                if (tAdd[idx - 1] > 0.0) { rA = tAdd[idx] / tAdd[idx - 1]; sumRAdd += rA; cntRAdd++; }
                if (tRem[idx - 1] > 0.0) { rR = tRem[idx] / tRem[idx - 1]; sumRRem += rR; cntRRem++; }
            }

            System.out.printf(java.util.Locale.ROOT,
                "n: %d; addAt (µs): %.2f; removeAt (µs): %.2f; r_add: %.2f; r_rem: %.2f%n",
                sizes[idx], add_us, rem_us, rA, rR);
        }

        double avgAdd = (cntRAdd > 0 ? sumRAdd / cntRAdd : 0.0);
        double avgRem = (cntRRem > 0 ? sumRRem / cntRRem : 0.0);
        System.out.printf(java.util.Locale.ROOT,
            "%nMédias r: addAt=%.2f, removeAt=%.2f%n",
            avgAdd, avgRem);
    }

}
