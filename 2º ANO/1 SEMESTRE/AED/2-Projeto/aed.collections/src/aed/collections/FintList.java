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
    
    public static void main(String[] args)
    {
        System.out.println("=== FintList: testes funcionais e ensaios de razão dobrada ===");

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

        // 10) iterator usando add
        {
            System.out.println("\n-- Teste: iterator (com add) --");
            FintList L = new FintList();
            for (int i = 0; i < 4; i++) L.add(i + 1); // 1..4
            int sum = 0;
            for (int v : L) { sum += v; System.out.print(v + " "); }
            System.out.println("\niterator sum -> " + sum);
        }

        // 11) higher-order methods: forEach, map, reduce
        {
            System.out.println("\n-- Teste: métodos de ordem superior (forEach, map, reduce) --");
            FintList L = new FintList();
            for (int i = 1; i <= 5; i++) L.add(i); // 1..5
            final int[] s = {0};
            L.forEach(x -> s[0] += x);
            System.out.println("forEach sum -> " + s[0]);
            L.map(x -> x + 1);
            System.out.print("map(+1) -> ");
            for (int v : L) System.out.print(v + " ");
            System.out.println();
            int sum = L.reduce((a,b)->a+b, 0);
            int prod = L.reduce((a,b)->a*b, 1);
            System.out.println("reduce sum -> " + sum + "; prod -> " + prod);
        }

        // =========================
        // Ensaios de razão dobrada
        // =========================
        // Usamos dois estilos:
        //   (A) workload fixo sobre uma lista base para medir custo por operação;
        //   (B) criar listas do tamanho N (para medir custo de construção/destruição).

        // Lista base para workloads (A):
        FintList base = new FintList();
        for (int i = 0; i < 10_000; i++) base.add(i);

        final int ITER_INIT = 25_000;
        final int ROUNDS    = 7;

        long prevGetAsc = 1, prevGetDesc = 1, prevSetAsc = 1, prevFind = 1,
        	 prevAddAppend = 1, prevRemoveTail = 1, prevAddAtMid = 1,
        	 prevGetRand = 1, prevRemoveAtMid = 1, prevCopyRev = 1, 
        	 prevMut = 1;


        // --- (B) add append em massa (construção de lista do zero) ---
        System.out.println("\n--- Razão dobrada: add(N) append (construção) ---");
        for (int r = 0, N = 1_000; r < ROUNDS; r++, N <<= 1) {
            // aquecimento
            { FintList L = new FintList(); for (int i = 0; i < N; i++) L.add(i); }
            long t0 = System.currentTimeMillis();
            { FintList L = new FintList(); for (int i = 0; i < N; i++) L.add(i); }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("N=" + N + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevAddAppend));
            prevAddAppend = dt;
        }

        // --- (B) remove() em massa (encher e esvaziar) ---
        System.out.println("\n--- Razão dobrada: remove() em massa (encher->esvaziar) ---");
        for (int r = 0, N = 1_000; r < ROUNDS; r++, N <<= 1) {
            FintList L = new FintList();
            for (int i = 0; i < N; i++) L.add(i);
            // aquecimento
            { FintList W = L.deepCopy(); for (int i = 0; i < N; i++) W.remove(); }
            long t0 = System.currentTimeMillis();
            { FintList W = L.deepCopy(); for (int i = 0; i < N; i++) W.remove(); }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("N=" + N + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevRemoveTail));
            prevRemoveTail = dt;
        }

        // --- (B) addAt no meio em massa (lista cresce com inserções centrais) ---
        System.out.println("\n--- Razão dobrada: addAt(no meio) em massa ---");
        for (int r = 0, N = 1_000; r < ROUNDS; r++, N <<= 1) {
            // aquecimento
            {
                FintList L = new FintList();
                for (int i = 0; i < N; i++) L.addAt(i/2, i); // insere sempre no meio
            }
            long t0 = System.currentTimeMillis();
            {
                FintList L = new FintList();
                for (int i = 0; i < N; i++) L.addAt(i/2, i);
            }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("N=" + N + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevAddAtMid));
            prevAddAtMid = dt;
        }

        // --- (A) get(i) ascendente ---
        System.out.println("\n--- Razão dobrada: get(i) ascendente ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            int sz = base.size(), acc = 0;
            for (int k = 0; k < it; k++) acc += base.get(k % sz);
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevGetAsc));
            prevGetAsc = dt;
            if (acc == 42_424_242) System.out.print("");
        }

        // --- (A) get(i) descendente ---
        System.out.println("\n--- Razão dobrada: get(i) descendente ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            int sz = base.size(), acc = 0;
            for (int k = 0; k < it; k++) acc += base.get(sz - 1 - (k % sz));
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevGetDesc));
            prevGetDesc = dt;
            if (acc == 43_434_343) System.out.print("");
        }

        // --- (A) get(index) "aleatório determinístico" ---
        System.out.println("\n--- Razão dobrada: get(index) pseudo-aleatório ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            int sz = base.size(), acc = 0;
            int x = 1234567;
            for (int k = 0; k < it; k++) {
                x = (1103515245 * x + 12345); // LCG 32-bit
                int idx = (x >>> 1) % sz;
                acc += base.get(idx);
            }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevGetRand));
            prevGetRand = dt;
            if (acc == 0x7fffffff) System.out.print("");
        }

        // --- (A) set(i) ascendente ---
        System.out.println("\n--- Razão dobrada: set(i) ascendente ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            int sz = base.size();
            for (int k = 0; k < it; k++) base.set(k % sz, (k & 7));
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevSetAsc));
            prevSetAsc = dt;
        }

        // --- (A) indexOf/contains alternado ---
        System.out.println("\n--- Razão dobrada: indexOf/contains alternado ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            boolean flag = false;
            for (int k = 0; k < it; k++) {
                if ((k & 1) == 0) flag ^= (base.indexOf(k & 7) >= 0);
                else flag ^= base.contains(k & 7);
            }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevFind));
            prevFind = dt;
            if (flag) System.out.print("");
        }

     // --- (A) addAt(0) + removeAt(0) (mantém tamanho) ---
        System.out.println("\n--- Razão dobrada: addAt(0) + removeAt(0) (mantém tamanho) ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            int sink = 0;
            for (int k = 0; k < it; k++) {
                base.addAt(0, k);
                sink ^= base.removeAt(0);
            }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevMut));
            prevMut = dt;
            if (sink == 42) System.out.print("");
        }

        // --- (B) removeAt no meio em massa (lista enche e depois remove sempre ao meio) ---
        System.out.println("\n--- Razão dobrada: removeAt(no meio) em massa ---");
        for (int r = 0, N = 2_000; r < ROUNDS; r++, N <<= 1) {
            // preparar lista [0..N-1]
            FintList L = new FintList();
            for (int i = 0; i < N; i++) L.add(i);

            // aquecimento
            {
                FintList W = L.deepCopy();
                for (int i = N; i > 0; i--) W.removeAt(i/2);
            }

            long t0 = System.currentTimeMillis();
            {
                FintList W = L.deepCopy();
                for (int i = N; i > 0; i--) W.removeAt(i/2);
            }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("N=" + N + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevRemoveAtMid));
            prevRemoveAtMid = dt;
        }

        // --- (B) deepCopy + reverse (combinado) ---
        System.out.println("\n--- Razão dobrada: deepCopy + reverse ---");
        for (int r = 0, N = 2_000; r < ROUNDS; r++, N <<= 1) {
            // construir lista de tamanho N
            FintList L = new FintList();
            for (int i = 0; i < N; i++) L.add(i);

            // aquecimento
            { FintList d = L.deepCopy(); d.reverse(); }

            long t0 = System.currentTimeMillis();
            { FintList d = L.deepCopy(); d.reverse(); }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("N=" + N + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevCopyRev));
            prevCopyRev = dt;
        }
        
        
        /*
         * TESTES MAIS PESADOS(MAIOR TEMPO DE RESPOSTA)
     
        // --- (A) reverse ---
        System.out.println("\n--- Razão dobrada: reverse ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            for (int k = 0; k < it; k++) base.reverse();
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevRev));
            prevRev = dt;
        }

        // --- (A) map(+1) repetido ---
        System.out.println("\n--- Razão dobrada: map (+1) ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            for (int k = 0; k < it; k++) base.map(x -> x + 1);
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevMap));
            prevMap = dt;
        }

        // --- (A) reduce (soma) repetido ---
        System.out.println("\n--- Razão dobrada: reduce (soma) ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            int acc = 0;
            for (int k = 0; k < it; k++) acc ^= base.reduce((a,b)->a+b, 0);
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevReduce));
            prevReduce = dt;
            if (acc == 0xdeadbeef) System.out.print("");
        }

        // --- (A) iterator: varredura por iterador ---
        System.out.println("\n--- Razão dobrada: iterator (varredura) ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            long s = 0;
            for (int k = 0; k < it; k++) { for (int v : base) s += v; }
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevIter));
            prevIter = dt;
            if (s == 0xabcdef) System.out.print("");
        }

        // --- (A) forEach: varredura por forEach ---
        System.out.println("\n--- Razão dobrada: forEach (varredura) ---");
        for (int r = 0, it = ITER_INIT; r < ROUNDS; r++, it <<= 1) {
            long t0 = System.currentTimeMillis();
            final long[] s = {0};
            for (int k = 0; k < it; k++) base.forEach(x -> s[0] += x);
            long dt = System.currentTimeMillis() - t0;
            System.out.println("iters=" + it + ", ms=" + dt + (r==0?"":", ratio=" + (double)dt / (double)prevForEach));
            prevForEach = dt;
            if (s[0] == 0x1234) System.out.print("");
        }
        
        */
        
            // Bench para FintList: addAt, removeAt, contains, deepCopy
            java.util.Random RNG = new java.util.Random(42);

            // tamanhos (doubling test)
            int[] sizes = {1000, 2000, 4000, 8000, 16000, 32000, 64000};

            // tempos médios (ns) por tamanho
            double[] tAdd  = new double[sizes.length];
            double[] tRem  = new double[sizes.length];
            double[] tCon  = new double[sizes.length];
            double[] tCopy = new double[sizes.length];

            int trials = 50; // medições por tamanho

            // ---------- medir ----------
            for (int idx = 0; idx < sizes.length; idx++) {
                int n = sizes[idx];

                // constrói lista base com n elementos (0..n-1)
                FintList S = new FintList();
                for (int i = 0; i < n; i++) S.add(i);

                // addAt em índice aleatório
                {
                    long sum = 0L;
                    for (int t = 0; t < trials; t++) {
                        int i = RNG.nextInt(S.size() + 1); // 0..size
                        long t0 = System.nanoTime();
                        S.addAt(i, -1);
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
                        S.addAt(j, -2);                   // marcador temporário
                        int i = RNG.nextInt(S.size());    // 0..size-1
                        long t0 = System.nanoTime();
                        S.removeAt(i);
                        long t1 = System.nanoTime();
                        sum += (t1 - t0);
                    }
                    tRem[idx] = sum / (double) trials;
                }

                // contains (50% acerto / 50% falha)
                {
                    long sum = 0L;
                    for (int t = 0; t < trials; t++) {
                        int x = (RNG.nextBoolean() ? RNG.nextInt(n) : Integer.MIN_VALUE);
                        long t0 = System.nanoTime();
                        S.contains(x);
                        long t1 = System.nanoTime();
                        sum += (t1 - t0);
                    }
                    tCon[idx] = sum / (double) trials;
                }

                // deepCopy (3 repetições por tamanho)
                {
                    long acc = 0L;
                    for (int rep = 0; rep < 3; rep++) {
                        long t0 = System.nanoTime();
                        S.deepCopy();
                        long t1 = System.nanoTime();
                        acc += (t1 - t0);
                    }
                    tCopy[idx] = acc / 3.0;
                }
            }

            // ---------- saída no formato pedido ----------
            System.out.println();
            System.out.println("tempos médios por operação (µs) e razão dobrada r = T(2n)/T(n)");
            System.out.println();

            double sAdd=0, sRem=0, sCon=0, sCpy=0;
            int cAdd=0, cRem=0, cCon=0, cCpy=0;

            for (int idx = 0; idx < sizes.length; idx++) {
                // ns -> µs
                final double add_us = tAdd[idx]  / 1_000.0;
                final double rem_us = tRem[idx]  / 1_000.0;
                final double con_us = tCon[idx]  / 1_000.0;
                final double cpy_us = tCopy[idx] / 1_000.0;

                // r = T(2n)/T(n); primeira linha 0.00
                double rA=0.0, rR=0.0, rC=0.0, rP=0.0;
                if (idx > 0) {
                    if (tAdd[idx-1]  > 0.0) { rA = tAdd[idx]  / tAdd[idx-1];  sAdd += rA; cAdd++; }
                    if (tRem[idx-1]  > 0.0) { rR = tRem[idx]  / tRem[idx-1];  sRem += rR; cRem++; }
                    if (tCon[idx-1]  > 0.0) { rC = tCon[idx]  / tCon[idx-1];  sCon += rC; cCon++; }
                    if (tCopy[idx-1] > 0.0) { rP = tCopy[idx] / tCopy[idx-1]; sCpy += rP; cCpy++; }
                }

                System.out.printf(java.util.Locale.ROOT,
                    "n: %d; addAt (µs): %.2f; removeAt (µs): %.2f; contains (µs): %.2f; deepCopy (µs): %.2f; r_add: %.2f; r_rem: %.2f; r_con: %.2f; r_copy: %.2f%n",
                    sizes[idx], add_us, rem_us, con_us, cpy_us, rA, rR, rC, rP);
            }

            double avgAdd = (cAdd>0 ? sAdd/cAdd : 0.0);
            double avgRem = (cRem>0 ? sRem/cRem : 0.0);
            double avgCon = (cCon>0 ? sCon/cCon : 0.0);
            double avgCpy = (cCpy>0 ? sCpy/cCpy : 0.0);

            System.out.printf(java.util.Locale.ROOT,
                "%nMédias r: addAt=%.2f, removeAt=%.2f, contains=%.2f, deepCopy=%.2f%n",
                avgAdd, avgRem, avgCon, avgCpy);
        


        System.out.println("\n=== Fim dos testes ===");
    }

}
