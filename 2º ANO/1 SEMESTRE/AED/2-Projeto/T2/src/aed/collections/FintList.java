/*Trabalho Realizado Por:
 * Diogo Carvalho - a90247
 * Diogo Freitas - a90147
 * 
 * PL1- G10
 */

package aed.collections;

/* import java.util.Arrays; /*NÃO USADO*/
import java.util.Iterator;
import java.util.function.BinaryOperator;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;
import aed.utils.TemporalAnalysisUtils;

public class FintList implements Iterable<Integer> {

    // Armazenamento contíguo em memória: vetor com buffer circular
    private int[] data;
    private int head;         // índice do primeiro elemento lógico
    private int size;         // número de elementos na lista
    private static final int INITIAL_CAPACITY = 10;


    public FintList()
    {
        data = new int[INITIAL_CAPACITY];
        head = 0;
        size = 0;
    }

    public boolean add(int item)
    {
        // coloca no fim; retorna true se modificou(retorna sempre true)
        ensureCapacity(size + 1);
        // uso de phys(size) para calcular a posição do novo "tail"
        data[phys(size)] = item;
        size++;
        return true;
    }

    public int get()
    {
        // último elemento; lança IndexOutOfBoundsException se vazia
        if (size == 0) throw new IndexOutOfBoundsException("List is empty");
        // uso de phys(size-1) para acesso ao último
        return data[phys(size - 1)];
    }

    public boolean isEmpty()
    {
        return size == 0;
    }

    public int size()
    {
        return size;
    }

    public int remove()
    {
        // remove e retorna o último; lança IndexOutOfBoundsException se vazia
        if (size == 0) throw new IndexOutOfBoundsException("List is empty");
        // uso de phys(size-1) para localizar o último sem recomputar wrap
        int pos = phys(size - 1);
        int val = data[pos];
        size--;
        if (size == 0) head = 0;
        return val;
    }

    public void addAt(int index, int item) {
        if (index < 0 || index > size)
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);

        ensureCapacity(size + 1);
        final int n = data.length;

        // ---- CASOS O(1), como numa lista ligada ----
        if (index == 0) {                  // inserir no início em O(1)
            head = (head == 0 ? n - 1 : head - 1);
            data[head] = item;
            size++;
            return;
        }
        if (index == size) {               // inserir no fim em O(1)
            data[phys(size)] = item;       // usa phys() para tail
            size++;
            return;
        }

        // ---- Inserção no meio: deslocar o lado mais curto ----
        if (index <= size / 2) {
            // desloca prefixo [head .. head+index) 1 à esquerda (equivale a "recuar" head)
            int oldHead = head;
            head = (head == 0 ? n - 1 : head - 1);
            copyWrap(oldHead, head, index);
        } else {
            // desloca sufixo [index .. size) 1 à direita
            int src = phys(index);
            int dst = src + 1; if (dst >= n) dst -= n;
            copyWrap(src, dst, size - index);
        }

        data[phys(index)] = item;  // escreve no buraco
        size++;
    }

    public int getFirst()
    {
        // 1.º elemento; lança IndexOutOfBoundsException se vazia
        if (size == 0) throw new IndexOutOfBoundsException("List is empty");
        return data[head];
    }

    public int get(int index) {
        if (index < 0 || index >= size)
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        return data[phys(index)];  // índice lógico -> físico
    }

    public void set(int index, int item) {
        if (index < 0 || index >= size)
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        data[phys(index)] = item;  // índice lógico -> físico
    }

    public int removeAt(int index) {
        if (index < 0 || index >= size)
            throw new IndexOutOfBoundsException("Index: " + index + ", Size: " + size);
        final int n = data.length;

        // ---- CASOS O(1), como numa lista ligada ----
        if (index == 0) {                   // remover do início em O(1)
            int val = data[head];
            head++; if (head >= n) head = 0;
            if (--size == 0) head = 0;
            return val;
        }
        if (index == size - 1) {            // remover do fim em O(1)
            return remove();                // já tens O(1) no teu remove()
        }

        int removed = data[phys(index)];

        // ---- Remoção no meio: deslocar o lado mais curto ----
        if (index <= size / 2) {
            // fecha buraco no prefixo: shift 1 à direita e avança head
            int newHead = head + 1; if (newHead >= n) newHead = 0;
            copyWrap(head, newHead, index);
            head = newHead;
        } else {
            // fecha buraco no sufixo: shift 1 à esquerda
            int src = phys(index + 1);
            int dst = phys(index);
            copyWrap(src, dst, size - 1 - index);
        }

        if (--size == 0) head = 0;
        return removed;
    }

    public int indexOf(int item)
    {
        int n = data.length, pos = head;
        for (int i = 0; i < size; i++) {
            if (data[pos] == item) return i;
            pos++; if (pos >= n) pos = 0;
        }
        return -1;
    }

    public boolean contains(int item)
    {
        return indexOf(item) != -1;
    }

    public boolean remove(int item)
    {
        int idx = indexOf(item);
        if (idx == -1) return false;
        removeAt(idx);
        return true;
    }

    public void reverse()
    {
        // usar phys() para endereçar as extremidades lógicas i e j
        for (int i = 0, j = size - 1; i < j; i++, j--) {
            int ai = phys(i);
            int aj = phys(j);
            int tmp = data[ai];
            data[ai] = data[aj];
            data[aj] = tmp;
        }
    }

    public FintList deepCopy()
    {
        FintList c = new FintList();
        if (this.size > c.data.length) {
            c.data = new int[this.size];
        }
        int n = this.data.length;
        int right = Math.min(this.size, n - this.head);
        if (right > 0) System.arraycopy(this.data, this.head, c.data, 0, right);
        if (this.size > right) System.arraycopy(this.data, 0, c.data, right, this.size - right);
        c.head = 0;
        c.size = this.size;
        return c;
    }

    public Iterator<Integer> iterator()
    {
        return new Iterator<Integer>() {
            private int i = 0;
            public boolean hasNext() { return i < size; }
            public Integer next() {
                if (!hasNext()) throw new java.util.NoSuchElementException();
                int v = data[phys(i)]; // usar phys() para mapear i lógico → físico
                i++;
                return v;
            }
        };
    }

    public void forEach(Consumer<? super Integer> c)
    {
        if (c == null) throw new NullPointerException("Consumer is null");
        int n = data.length, pos = head;
        for (int i = 0; i < size; i++) {
            c.accept(data[pos]);
            pos++; if (pos >= n) pos = 0;
        }
    }

    public void map(UnaryOperator<Integer> op)
    {
        if (op == null) throw new NullPointerException("UnaryOperator is null");
        int n = data.length, pos = head;
        for (int i = 0; i < size; i++) {
            Integer nv = op.apply(data[pos]);
            if (nv == null) throw new NullPointerException("UnaryOperator returned null");
            data[pos] = nv;
            pos++; if (pos >= n) pos = 0;
        }
    }

    public int reduce(BinaryOperator<Integer> op, int defaultValue)
    {
        if (op == null) throw new NullPointerException("BinaryOperator is null");
        int acc = defaultValue;
        int n = data.length, pos = head;
        for (int i = 0; i < size; i++) {
            acc = op.apply(acc, data[pos]);
            pos++; if (pos >= n) pos = 0;
        }
        return acc;
    }
    
 // ---------- auxiliar privado ----------
    private void ensureCapacity(int needed) {
        if (data.length >= needed) return;
        int newCap = Math.max(needed, data.length == 0 ? INITIAL_CAPACITY : data.length * 2);
        int[] nd = new int[newCap];
        if (size > 0) {
            if (head + size <= data.length) {
                System.arraycopy(data, head, nd, 0, size);
            } else {
                int right = data.length - head;
                System.arraycopy(data, head, nd, 0, right);
                System.arraycopy(data, 0, nd, right, size - right);
            }
        }
        data = nd;
        head = 0;
    }
    
 // índice lógico -> posição física no array, sem usar % no caminho comum
    private int phys(int index) {
        final int n = data.length, h = head, limit = n - h;
        return (h == 0) ? index : (index < limit ? h + index : index - limit);
    }	

 // copia com wrap em blocos (sempre seguro; 1..N blocos conforme necessário)
    private void copyWrap(int src, int dst, int len) {
        if (len <= 0) return;
        final int n = data.length;
        while (len > 0) {
            int max = Math.min(len, Math.min(n - src, n - dst));
            System.arraycopy(data, src, data, dst, max);
            src += max; if (src >= n) src -= n;
            dst += max; if (dst >= n) dst -= n;
            len -= max;
        }
    }
    

    public static void main(String[] args) {
    	
        // =================
        // Testes funcionais
        // =================

        // 1) add & get (último e por índice), size & isEmpty
        {
            System.out.println("\n-- Teste: add, get() (último), get(index), size, isEmpty --");
            FintList L = new FintList();
            System.out.println("isEmpty -> " + L.isEmpty() + "; size -> " + L.size());
            for (int i = 1; i <= 5; i++) {
                boolean ok = L.add(i * 10); // 10,20,30,40,50
                System.out.println("isEmpty -> " + L.isEmpty() + "; add(" + (i*10) + ") -> " + ok + "; size -> " + L.size());
            }
            System.out.println("isEmpty -> " + L.isEmpty());
            System.out.println("get() (last) -> " + L.get());       // 50
            System.out.println("get(0) -> " + L.get(0));            // 10
            System.out.println("get(2) -> " + L.get(2));            // 30
            System.out.println("get(4) -> " + L.get(4));            // 50
        }

        // 2) remove (último)
        {
            System.out.println("\n-- Teste: remove() (último) --");
            FintList L = new FintList();
            for (int i = 1; i <= 4; i++) L.add(i); // 1..4
            int rem = L.remove(); // remove 4
            System.out.println("remove() -> " + rem + "; size -> " + L.size() + "; last -> " + L.get()); // last 3
        }

        // 3) add & get(index)
        {
            System.out.println("\n-- Teste: add & get(index) --");
            FintList L = new FintList();
            for (int i = 0; i < 6; i++) L.add(i * 2); // 0,2,4,6,8,10
            System.out.println("get(0) -> " + L.get(0));
            System.out.println("get(3) -> " + L.get(3));
            System.out.println("get(5) -> " + L.get(5));
        }

        // 4) addAt & get(index)
        {
            System.out.println("\n-- Teste: addAt & get(index) --");
            FintList L = new FintList();
            L.add(1); L.add(3); L.add(5); // [1,3,5]
            L.addAt(1, 2);                // [1,2,3,5]
            L.addAt(3, 4);                // [1,2,3,4,5]
            L.addAt(0, 0);                // [0,1,2,3,4,5]
            for (int i = 0; i < L.size(); i++) System.out.println("get(" + i + ") -> " + L.get(i));
        }

        // 5) add + addAt + getFirst
        {
            System.out.println("\n-- Teste: add, addAt e getFirst --");
            FintList L = new FintList();
            L.add(2); L.add(3);           // [2,3]
            L.addAt(0, 1);                // [1,2,3]
            System.out.println("getFirst -> " + L.getFirst());
        }

        // 6) set usando add & get(index)
        {
            System.out.println("\n-- Teste: set (com add & get(index)) --");
            FintList L = new FintList();
            for (int i = 0; i < 5; i++) L.add(0); // [0,0,0,0,0]
            L.set(2, 99);
            System.out.println("set(2,99); get(2) -> " + L.get(2));
        }

        // 7) removeAt(index) usando add & get(index)
        {
            System.out.println("\n-- Teste: removeAt(index) (com add & get(index)) --");
            FintList L = new FintList();
            for (int i = 0; i < 6; i++) L.add(i + 1); // [1,2,3,4,5,6]
            int r1 = L.removeAt(0);                   // remove 1
            int r2 = L.removeAt(L.size() - 1);        // remove 6
            int r3 = L.removeAt(2);                   // remove 4 (índice 2 actual)
            System.out.println("removeAt(0) -> " + r1 + "; removeAt(last) -> " + r2 + "; removeAt(2) -> " + r3);
            System.out.println("size -> " + L.size());
            System.out.print("restante -> ");
            for (int i = 0; i < L.size(); i++) System.out.print(L.get(i) + " ");
            System.out.println();
        }

        // 8) indexOf, contains & remove(int)
        {
            System.out.println("\n-- Teste: indexOf, contains e remove(int) --");
            FintList L = new FintList();
            L.add(10); L.add(20); L.add(30);
            System.out.println("indexOf(20) -> " + L.indexOf(20));
            System.out.println("contains(30) -> " + L.contains(30));
            System.out.println("remove(20) -> " + L.remove(20));
            System.out.println("contains(20) -> " + L.contains(20) + "; indexOf(20) -> " + L.indexOf(20));
        }

        // 9) deepCopy & reverse (com add & get(index))
        {
            System.out.println("\n-- Teste: deepCopy e reverse --");
            FintList L = new FintList();
            for (int i = 1; i <= 5; i++) L.add(i); // 1..5
            FintList C = L.deepCopy();
            System.out.print("original -> ");
            for (int i = 0; i < L.size(); i++) System.out.print(L.get(i) + " ");
            System.out.println();
            System.out.print("cópia -> ");
            for (int i = 0; i < C.size(); i++) System.out.print(C.get(i) + " ");
            System.out.println();
            L.set(0, 99);
            System.out.println("após set(0,99): original.get(0)=" + L.get(0) + "; copia.get(0)=" + C.get(0));
            C.reverse();
            System.out.print("cópia após reverse -> ");
            for (int i = 0; i < C.size(); i++) System.out.print(C.get(i) + " ");
            System.out.println();
        }

        final int ITER = 10;           // n, 2n, 4n, ...
        final int REPS_MUT = 1500;      // add/remove (altera a lista → reconstruímos)
        final int REPS_CONTAINS = 20000; // contém (não altera a lista)
        final int REPS_COPY = 10000;     // deepCopy (não altera a lista)

        // addAt (meio)
        System.out.println("\nMétodo: addAt (meio)");
        TemporalAnalysisUtils.runDoublingRatioTest(
            (Integer n) -> { FintList b = new FintList(); for (int i=0;i<n;i++) b.add(i); return b; },
            (FintList b) -> {
                int n = b.size();
                for (int r=0; r<REPS_MUT; r++) {
                    FintList l = new FintList();
                    for (int i=0;i<n;i++) l.add(i);
                    l.addAt(l.size()/2, -1);
                }
            },
            ITER
        );

        // removeAt (meio)
        System.out.println("\nMétodo: removeAt (meio)");
        TemporalAnalysisUtils.runDoublingRatioTest(
            (Integer n) -> { FintList b = new FintList(); for (int i=0;i<n;i++) b.add(i); return b; },
            (FintList b) -> {
                int n = b.size();
                for (int r=0; r<REPS_MUT; r++) {
                    FintList l = new FintList();
                    for (int i=0;i<n;i++) l.add(i);
                    l.removeAt(l.size()/2);
                }
            },
            ITER
        );

        // contains (procura o último)
        System.out.println("\nMétodo: contains (procura último)");
        TemporalAnalysisUtils.runDoublingRatioTest(
            (Integer n) -> { FintList l = new FintList(); for (int i=0;i<n;i++) l.add(i); return l; },
            (FintList l) -> {
                int target = l.get(l.size()-1);
                for (int r=0; r<REPS_CONTAINS; r++) l.contains(target);
            },
            ITER
        );

        // deepCopy
        System.out.println("\nMétodo: deepCopy");
        TemporalAnalysisUtils.runDoublingRatioTest(
            (Integer n) -> { FintList l = new FintList(); for (int i=0;i<n;i++) l.add(i); return l; },
            (FintList l) -> {
                for (int r=0; r<REPS_COPY; r++) {
                    FintList c = l.deepCopy();
                    if (c == null) System.out.print(""); // evita eliminação pelo JIT
                }
            },
            ITER
        );

        System.out.println("\n=== Fim dos testes (FintList) ===");
    }

}
