/*Trabalho Realizado Por:
 * Diogo Carvalho - a90247
 * Diogo Freitas - a90147
 *
 * PL1- G10
 */
package aed.tables;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

class UAlshBucket<Key, Value> implements IUAlshBucket<Key, Value> {
    Key key;
    Value value;
    int hashCode1;
    int hashCode2;
    int maxST;

    UAlshBucket() {
        this.key = null;
        this.value = null;
        this.hashCode1 = 0;
        this.hashCode2 = 0;
        this.maxST = 0;
    }

    @Override
    public Key getKey() {return key;}

    @Override
    public Value getValue() {return value;}

    @Override
    public boolean isEmpty() {return key == null;}

    @Override
    public boolean isDeleted() {return key != null && value == null;}
}

public class UAlshTable<Key, Value> {

    private static final int[] PRIMES = new int[]{
        5, 7, 11, 17, 37, 79, 163, 331, 673, 1361, 2729, 5471, 10949, 21911,
        43853, 87719, 175447, 350899, 701819, 1403641, 2807303, 5614657,
        11229331, 22458671, 44917381, 89834777, 179669557
    };

    private static final int NUM_TABLES = 5;

    private UAlshBucket<Key, Value>[][] tables;
    private int[] tableSizes;
    private int[] primeIndices;
    private final Function<Key, Integer> hashCode2Func;
    private int size;          
    private int deletedCount;  

    @SuppressWarnings("unchecked")
    public UAlshTable(Function<Key, Integer> hc2) {
        this.hashCode2Func = hc2;
        this.size = 0;
        this.deletedCount = 0;

        this.primeIndices = new int[]{4, 3, 2, 1, 0};
        this.tableSizes = new int[NUM_TABLES];
        this.tables = new UAlshBucket[NUM_TABLES][];

        initTables();
    }

    @SuppressWarnings("unchecked")
    private void initTables() {
        for (int i = 0; i < NUM_TABLES; i++) {
            tableSizes[i] = PRIMES[primeIndices[i]];
            tables[i] = new UAlshBucket[tableSizes[i]];
            for (int j = 0; j < tableSizes[i]; j++) {
                tables[i][j] = new UAlshBucket<>();
            }
        }
    }

    public int size() {return size;}

    public int getMainCapacity() {return tableSizes[0];}

    public int getTotalCapacity() {
        int total = 0;
        for (int i = 0; i < NUM_TABLES; i++) total += tableSizes[i];
        return total;
    }

    public float getLoadFactor() {return (float) (size + deletedCount) / getTotalCapacity();}

    public int getDeletedNotRemoved() {return deletedCount;}

    public IUAlshBucket<Key, Value>[] getSubTable(int i) {
        if (i < 1 || i > NUM_TABLES) return null;
        return tables[i - 1];
    }

    private int hash(int hc1, int hc2, int tableIndex) {
        int m = tableSizes[tableIndex];
        int i = tableIndex + 1;
        int sum = hc1 + i * hc2;
        int nonneg = sum & 0x7fffffff;
        return nonneg % m;
    }

    private UAlshBucket<Key, Value> getBucket(int hc1, int hc2, int tableIndex) {
        return tables[tableIndex][hash(hc1, hc2, tableIndex)];
    }

    private void updateMaxST(int hc1, int hc2, int insertTable) {
        int v = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key, Value> b = getBucket(hc1, hc2, i);
            if (b.maxST < v) b.maxST = v;
        }
    }

    private UAlshBucket<Key, Value> findLiveBucket(Key k, int hc1, int hc2) {
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key, Value> b = getBucket(hc1, hc2, i);
            if (b.isEmpty() || b.isDeleted()) continue;
            if (b.hashCode1 != hc1 || b.hashCode2 != hc2) continue;
            if (k.equals(b.key)) return b;
        }
        return null;
    }

    private int findDeletedSameKeyTable(Key k, int hc1, int hc2) {
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key, Value> b = getBucket(hc1, hc2, i);
            if (!b.isDeleted()) continue;
            if (b.hashCode1 != hc1 || b.hashCode2 != hc2) continue;
            if (k.equals(b.key)) return i;
        }
        return -1;
    }

    public boolean containsKey(Key k) {
        int hc1 = k.hashCode() & 0x7fffffff;
        int hc2 = hashCode2Func.apply(k) & 0x7fffffff;
        return findLiveBucket(k, hc1, hc2) != null;
    }

    public Value get(Key k) {
        int hc1 = k.hashCode() & 0x7fffffff;
        int hc2 = hashCode2Func.apply(k) & 0x7fffffff;
        UAlshBucket<Key, Value> b = findLiveBucket(k, hc1, hc2);
        return (b == null) ? null : b.value;
    }

    public void put(Key k, Value v) {
        if (v == null) {
            delete(k);
            return;
        }

        int hc1 = k.hashCode() & 0x7fffffff;
        int hc2 = hashCode2Func.apply(k) & 0x7fffffff;

        int limit = (tableSizes[0] * 85) / 100;
        if ((size + 1) >= limit) {
            resize(true);
            hc1 = k.hashCode() & 0x7fffffff;
            hc2 = hashCode2Func.apply(k) & 0x7fffffff;
        }

        UAlshBucket<Key, Value> live = findLiveBucket(k, hc1, hc2);
        if (live != null) {
            live.value = v;
            return;
        }

        int delTable = findDeletedSameKeyTable(k, hc1, hc2);
        if (delTable != -1) {
            UAlshBucket<Key, Value> b = getBucket(hc1, hc2, delTable);
            b.value = v;
            b.hashCode1 = hc1;
            b.hashCode2 = hc2;
            size++;
            deletedCount--;
            updateMaxST(hc1, hc2, delTable);
            return;
        }

        int emptyTable = -1;
        for (int i = 0; i < NUM_TABLES; i++) {
            if (getBucket(hc1, hc2, i).isEmpty()) {
                emptyTable = i;
                break;
            }
        }

        if (emptyTable == -1) {
            resize(true);
            put(k, v);
            return;
        }

        UAlshBucket<Key, Value> bucket = tables[emptyTable][hash(hc1, hc2, emptyTable)];
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;

        size++;
        updateMaxST(hc1, hc2, emptyTable);
    }

    public void fastPut(Key k, Value v) {
        if (v == null) {
            delete(k);
            return;
        }

        int hc1 = k.hashCode() & 0x7fffffff;
        int hc2 = hashCode2Func.apply(k) & 0x7fffffff;

        int limit = (tableSizes[0] * 85) / 100;
        if ((size + 1) >= limit) {
            resize(true);
            hc1 = k.hashCode() & 0x7fffffff;
            hc2 = hashCode2Func.apply(k) & 0x7fffffff;
        }

        int emptyTable = -1;
        for (int i = 0; i < NUM_TABLES; i++) {
            if (getBucket(hc1, hc2, i).isEmpty()) {
                emptyTable = i;
                break;
            }
        }

        if (emptyTable == -1) {
            resize(true);
            fastPut(k, v);
            return;
        }

        UAlshBucket<Key, Value> bucket = tables[emptyTable][hash(hc1, hc2, emptyTable)];
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;

        size++;
        updateMaxST(hc1, hc2, emptyTable);
    }

    public void delete(Key k) {
        int hc1 = k.hashCode() & 0x7fffffff;
        int hc2 = hashCode2Func.apply(k) & 0x7fffffff;

        UAlshBucket<Key, Value> target = null;

        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key, Value> b = getBucket(hc1, hc2, i);
            if (b.isEmpty() || b.isDeleted()) continue;
            if (b.hashCode1 != hc1 || b.hashCode2 != hc2) continue;
            if (k.equals(b.key)) { target = b; break; }
        }

        if (target == null) return;

        target.value = null;
        size--;
        deletedCount++;

        if (size < tableSizes[0] / 4 && primeIndices[0] > 4) {
            resize(false);
        }
    }

    private void resize(boolean grow) {
        List<Key> keys = new ArrayList<>();
        List<Value> values = new ArrayList<>();

        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key, Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    keys.add(b.key);
                    values.add(b.value);
                }
            }
        }

        if (grow) {
            for (int i = 0; i < NUM_TABLES; i++) {
                if (primeIndices[i] < PRIMES.length - 1) primeIndices[i]++;
            }
        } else {
            for (int i = 0; i < NUM_TABLES; i++) {
                int minIdx = (i == 0) ? 4 : 0;
                if (primeIndices[i] > minIdx) primeIndices[i]--;
            }
        }

        initTables();
        size = 0;
        deletedCount = 0;

        for (int idx = 0; idx < keys.size(); idx++) {
            Key k = keys.get(idx);
            Value v = values.get(idx);

            int hc1 = k.hashCode() & 0x7fffffff;
            int hc2 = hashCode2Func.apply(k) & 0x7fffffff;

            int insertTable = -1;
            for (int i = 0; i < NUM_TABLES; i++) {
                if (tables[i][hash(hc1, hc2, i)].isEmpty()) {
                    insertTable = i;
                    break;
                }
            }

            if (insertTable == -1) {
                resize(true);
                idx--;
                continue;
            }

            UAlshBucket<Key, Value> bucket = tables[insertTable][hash(hc1, hc2, insertTable)];
            bucket.key = k;
            bucket.value = v;
            bucket.hashCode1 = hc1;
            bucket.hashCode2 = hc2;

            size++;
            updateMaxST(hc1, hc2, insertTable);
        }
    }

    
    public Iterable<Key> keys() {
        return () -> new java.util.Iterator<Key>() {
            private int ti = 0;
            private int bi = -1;
            private Key nextKey = advance();

            private Key advance() {
                while (ti < NUM_TABLES) {
                    bi++;
                    if (bi >= tableSizes[ti]) { ti++; bi = -1; continue; }
                    UAlshBucket<Key, Value> b = tables[ti][bi];
                    if (!b.isEmpty() && !b.isDeleted()) return b.key;
                }
                return null;
            }

            @Override
            public boolean hasNext() { return nextKey != null; }

            @Override
            public Key next() {
                if (nextKey == null) throw new java.util.NoSuchElementException();
                Key cur = nextKey;
                nextKey = advance();
                return cur;
            }
        };
        
    }
    
    public static void main(String[] args) {
        // Função hashCode2 simples para testes
        Function<String, Integer> hc2 = s -> (s == null) ? 0 : (s.hashCode() * 31 + 17);

        UAlshTable<String, Integer> t = new UAlshTable<>(hc2);

        System.out.println("== INIT ==");
        System.out.println("size: " + t.size());
        System.out.println("mainCap: " + t.getMainCapacity());
        System.out.println("totalCap: " + t.getTotalCapacity());
        System.out.println("loadFactor: " + t.getLoadFactor());
        System.out.println("deletedNotRemoved: " + t.getDeletedNotRemoved());
        System.out.println("subTable(1) null? " + (t.getSubTable(1) == null));

        System.out.println("\n== PUT ==");
        t.put("a", 1);
        t.put("b", 2);
        t.put("c", 3);
        System.out.println("size: " + t.size());
        System.out.println("contains a? " + t.containsKey("a"));
        System.out.println("get a: " + t.get("a"));

        System.out.println("\n== UPDATE ==");
        t.put("a", 10);
        System.out.println("get a: " + t.get("a"));
        System.out.println("size (should be 3): " + t.size());

        System.out.println("\n== DELETE ==");
        t.delete("b");
        System.out.println("contains b? " + t.containsKey("b"));
        System.out.println("get b: " + t.get("b"));
        System.out.println("size (should be 2): " + t.size());
        System.out.println("deletedNotRemoved: " + t.getDeletedNotRemoved());

        System.out.println("\n== REINSERT SAME KEY ==");
        t.put("b", 20);
        System.out.println("get b: " + t.get("b"));
        System.out.println("size (should be 3): " + t.size());
        System.out.println("deletedNotRemoved: " + t.getDeletedNotRemoved());

        System.out.println("\n== FASTPUT (assume new keys) ==");
        t.fastPut("d", 4);
        t.fastPut("e", 5);
        System.out.println("size: " + t.size());

        System.out.println("\n== KEYS ITERATION ==");
        for (String k : t.keys()) {
            System.out.println(k + " -> " + t.get(k));
        }

        System.out.println("\n== FINAL STATS ==");
        System.out.println("size: " + t.size());
        System.out.println("mainCap: " + t.getMainCapacity());
        System.out.println("totalCap: " + t.getTotalCapacity());
        System.out.println("loadFactor: " + t.getLoadFactor());
        System.out.println("deletedNotRemoved: " + t.getDeletedNotRemoved());
        
        System.out.println("\n== STRESS SMALL ==");
        UAlshTable<Integer, Integer> x = new UAlshTable<>(k -> k * 1337 + 123);

        int n = 200;
        for (int i = 0; i < n; i++) x.put(i, i);

        System.out.println("after insert:");
        System.out.println("size: " + x.size());
        System.out.println("mainCap: " + x.getMainCapacity());
        System.out.println("totalCap: " + x.getTotalCapacity());
        System.out.println("deletedNotRemoved: " + x.getDeletedNotRemoved());

        for (int i = 0; i < n; i += 2) x.delete(i); // apaga metade

        System.out.println("after delete half:");
        System.out.println("size: " + x.size());
        System.out.println("mainCap: " + x.getMainCapacity());
        System.out.println("totalCap: " + x.getTotalCapacity());
        System.out.println("deletedNotRemoved: " + x.getDeletedNotRemoved());

        // reinsere alguns pares apagados (mesma chave)
        for (int i = 0; i < 20; i++) x.put(i * 2, 999);

        System.out.println("after reinsert some:");
        System.out.println("size: " + x.size());
        System.out.println("mainCap: " + x.getMainCapacity());
        System.out.println("totalCap: " + x.getTotalCapacity());
        System.out.println("deletedNotRemoved: " + x.getDeletedNotRemoved());

        System.out.println("\n== FORCE SHRINK CHECK ==");

        // apaga mais até ficar claramente abaixo de 331/4
        for (int i = 1; i < n; i += 2) {   // apaga os ímpares também
        	x.delete(i);
        }

        System.out.println("after delete almost all:");
        System.out.println("size: " + x.size());
        System.out.println("mainCap: " + x.getMainCapacity());
        System.out.println("totalCap: " + x.getTotalCapacity());
        System.out.println("deletedNotRemoved: " + x.getDeletedNotRemoved());
    }
}