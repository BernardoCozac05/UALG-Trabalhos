/*Trabalho Realizado Por:
 * Diogo Carvalho - a90247
 * Diogo Freitas - a90147
 * 
 * PL1- G10.
 */

package aed.tables;

import java.util.ArrayList;
import java.util.List;
import java.util.function.Function;

class UAlshBucket<Key,Value> implements IUAlshBucket<Key,Value> {
    Key key;
    Value value;
    int hashCode1;
    int hashCode2;
    int maxST;

    UAlshBucket() {
        this.key = null;
        this.value = null;
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

public class UAlshTable<Key,Value> {

    private static final int[] primes = {
            5, 7, 11, 17, 37, 79, 163, 331,
            673, 1361, 2729, 5471, 10949,
            21911, 43853, 87719, 175447, 350899,
            701819, 1403641, 2807303, 5614657,
            11229331, 22458671, 44917381, 89834777, 179669557
    };

    private static final int NUM_TABLES = 5;

    private UAlshBucket<Key,Value>[][] tables;
    private int[] tableSizes;
    private int[] primeIndices;
    private Function<Key,Integer> hashCode2Func;
    private int size;
    private int deletedCount;

    @SuppressWarnings("unchecked")
    public UAlshTable(Function<Key,Integer> hc2) {
        this.hashCode2Func = hc2;
        this.size = 0;
        this.deletedCount = 0;
        this.primeIndices = new int[]{4, 3, 2, 1, 0}; // 37,17,11,7,5
        this.tableSizes = new int[NUM_TABLES];
        this.tables = new UAlshBucket[NUM_TABLES][];
        initTables();
    }

    @SuppressWarnings("unchecked")
    private void initTables() {
        for (int i = 0; i < NUM_TABLES; i++) {
            tableSizes[i] = primes[primeIndices[i]];
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
    public IUAlshBucket<Key,Value>[] getSubTable(int i) {
        if (i < 1 || i > NUM_TABLES) return null;
        return tables[i - 1];
    }

    // hi(x) = (hc1 + i*hc2) & 0x7fffffff % mi  (mascara conforme enunciado)
    private int hash(int hc1, int hc2, int tableIndex) {
        int m = tableSizes[tableIndex];
        int i = tableIndex + 1;
        int h = ((hc1 + i * hc2) & 0x7fffffff) % m;
        return h;
    }

    private UAlshBucket<Key,Value> getBucket(int hc1, int hc2, int tableIndex) {
        int pos = hash(hc1, hc2, tableIndex);
        return tables[tableIndex][pos];
    }

    // z = min maxST; se algum maxST==0 -> 0
    private int calculateZ(int hc1, int hc2) {
        int z = Integer.MAX_VALUE;
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> b = getBucket(hc1, hc2, i);
            if (b.maxST == 0) return 0;
            z = Math.min(z, b.maxST);
        }
        return z;
    }

    private UAlshBucket<Key,Value> findBucket(Key k) {
        int hc1 = k.hashCode();
        int hc2 = hashCode2Func.apply(k);
        int z = calculateZ(hc1, hc2);
        if (z == 0) return null;
        for (int i = z - 1; i >= 0; i--) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (!bucket.isEmpty() && !bucket.isDeleted()) {
                if (bucket.hashCode1 == hc1 && bucket.hashCode2 == hc2 && bucket.key.equals(k)) {
                    return bucket;
                }
            }
        }
        return null;
    }

    private UAlshBucket<Key,Value> findBucketIncludingDeleted(Key k, int hc1, int hc2) {
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (!bucket.isEmpty()) {
                if (bucket.hashCode1 == hc1 && bucket.hashCode2 == hc2 && bucket.key.equals(k)) {
                    return bucket;
                }
            }
        }
        return null;
    }

    public boolean containsKey(Key k) {return findBucket(k) != null;}
    public Value get(Key k) {
        UAlshBucket<Key,Value> bucket = findBucket(k);
        return bucket == null ? null : bucket.value;
    }

    public void put(Key k, Value v) {
        if (v == null) { delete(k); return; }

        int hc1 = k.hashCode();
        int hc2 = hashCode2Func.apply(k);

        UAlshBucket<Key,Value> existing = findBucketIncludingDeleted(k, hc1, hc2);
        if (existing != null) {
            if (existing.isDeleted()) {
                existing.value = v;
                deletedCount--;
                size++;
            } else {
                existing.value = v;
            }
            return;
        }

        if (size >= tableSizes[0]) {
            resize(true);
            hc1 = k.hashCode();
            hc2 = hashCode2Func.apply(k);
        }

        int insertTable = -1;
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (bucket.isEmpty()) { insertTable = i; break; }
        }
        if (insertTable == -1) {
            resize(true);
            put(k, v);
            return;
        }

        int pos = hash(hc1, hc2, insertTable);
        UAlshBucket<Key,Value> bucket = tables[insertTable][pos];
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;
        size++;

        int maxSTValue = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int p = hash(hc1, hc2, i);
            tables[i][p].maxST = Math.max(tables[i][p].maxST, maxSTValue);
        }
    }

    public void fastPut(Key k, Value v) {
        int hc1 = k.hashCode();
        int hc2 = hashCode2Func.apply(k);

        if (size >= tableSizes[0]) {
            resize(true);
            hc1 = k.hashCode();
            hc2 = hashCode2Func.apply(k);
        }

        UAlshBucket<Key,Value> existing = findBucketIncludingDeleted(k, hc1, hc2);
        if (existing != null && existing.isDeleted()) {
            existing.value = v;
            deletedCount--;
            size++;
            return;
        }

        int insertTable = -1;
        for (int i = 0; i < NUM_TABLES; i++) {
            UAlshBucket<Key,Value> bucket = getBucket(hc1, hc2, i);
            if (bucket.isEmpty()) { insertTable = i; break; }
        }
        if (insertTable == -1) {
            resize(true);
            fastPut(k, v);
            return;
        }

        int pos = hash(hc1, hc2, insertTable);
        UAlshBucket<Key,Value> bucket = tables[insertTable][pos];
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;
        size++;

        int maxSTValue = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int p = hash(hc1, hc2, i);
            tables[i][p].maxST = Math.max(tables[i][p].maxST, maxSTValue);
        }
    }

    public void delete(Key k) {
        UAlshBucket<Key,Value> bucket = findBucket(k);
        if (bucket == null) return;

        bucket.value = null;
        size--;
        deletedCount++;

        if (size < tableSizes[0] / 4 && primeIndices[0] > 4) {
            resize(false);
        }
    }

    private void resize(boolean grow) {
        List<Key> keys = new ArrayList<>();
        List<Value> values = new ArrayList<>();
        List<Integer> hc1s = new ArrayList<>();
        List<Integer> hc2s = new ArrayList<>();

        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key,Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    keys.add(b.key);
                    values.add(b.value);
                    hc1s.add(b.hashCode1);
                    hc2s.add(b.hashCode2);
                }
            }
        }

        if (grow) {
            for (int i = 0; i < NUM_TABLES; i++) {
                if (primeIndices[i] < primes.length - 1) primeIndices[i]++;
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
            reinsert(keys.get(idx), values.get(idx), hc1s.get(idx), hc2s.get(idx));
        }
    }

    private void reinsert(Key k, Value v, int hc1, int hc2) {
        int insertTable = -1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int pos = hash(hc1, hc2, i);
            if (tables[i][pos].isEmpty()) { insertTable = i; break; }
        }
        if (insertTable == -1) {
            resize(true);
            reinsert(k, v, hc1, hc2);
            return;
        }

        int pos = hash(hc1, hc2, insertTable);
        UAlshBucket<Key,Value> bucket = tables[insertTable][pos];
        bucket.key = k;
        bucket.value = v;
        bucket.hashCode1 = hc1;
        bucket.hashCode2 = hc2;
        size++;

        int maxSTValue = insertTable + 1;
        for (int i = 0; i < NUM_TABLES; i++) {
            int p = hash(hc1, hc2, i);
            tables[i][p].maxST = Math.max(tables[i][p].maxST, maxSTValue);
        }
    }

    public Iterable<Key> keys() {
        List<Key> lista = new ArrayList<>();
        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key,Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    lista.add(b.key);
                }
            }
        }
        return lista;
    }

    public Iterable<Value> values() {
        List<Value> result = new ArrayList<>();
        for (int i = 0; i < NUM_TABLES; i++) {
            for (int j = 0; j < tableSizes[i]; j++) {
                UAlshBucket<Key,Value> b = tables[i][j];
                if (!b.isEmpty() && !b.isDeleted()) {
                    result.add(b.value);
                }
            }
        }
        return result;
    }

    /* === Testes locais (único main) para aproximar testB05 e testB07 === */

    private static void assertTrue(boolean cond, String msg) {
        if (!cond) throw new AssertionError(msg);
    }

    private static void runLocalTest5() {
        Function<String,Integer> hc2 = s -> {
            int h = 7;
            for (int i = 0; i < s.length(); i++) h = 37 * h + s.charAt(i);
            return h;
        };
        UAlshTable<String,Integer> t = new UAlshTable<>(hc2);

        String[] expected = {
            "kfM1Scs","xe7LvVc","BjeWFZ5","AaBBBBBB","0ZGfmKF",
            "R8yil9o","AaBBAaAa","zdTd2eU","8iEI26c","kGmAcga",
            "OGYBx0F","AaAaBBAa","BBAaAaAa","IOC0A9l","a2cBREr",
            "AaBBAaBB","BaVDIcP","72BsWLF","AaBBBBAa","B5NBtXK",
            "hC8xsJj","AaAaBBBB","HAv2ien","BBAaAaBB","yAhaBlD",
            "wlHaB8g","llLymNY","sx0cvvi","p5tvusa","t00w3EP",
            "0hJp3yu","5vqgTcX","AaAaAaAa","AaAaAaBB","srg30vn"
        };
        for (int i = 0; i < expected.length; i++) t.put(expected[i], i);

        List<String> got = new ArrayList<>();
        for (String k : t.keys()) got.add(k);

        assertTrue(got.size() == expected.length,
                "Test5: tamanho diferente: got=" + got.size() + " exp=" + expected.length);

        int mism = 0;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < expected.length; i++) {
            if (!expected[i].equals(got.get(i))) {
                mism++;
                if (mism <= 10) sb.append("\n idx ").append(i).append(" exp=").append(expected[i])
                                  .append(" got=").append(got.get(i));
            }
        }
        assertTrue(mism == 0, "Test5: ordem divergente, mismatches=" + mism + sb);
        System.out.println("Local Test5 OK (ordem keys)");
    }

    private static void runLocalTest7() {
        Function<String,Integer> hc2 = s -> {
            int h = 7;
            for (int i = 0; i < s.length(); i++) h = 37 * h + s.charAt(i);
            return h;
        };
        UAlshTable<String,Integer> t = new UAlshTable<>(hc2);

        for (int i = 0; i < 100; i++) t.put("K" + i, i);
        assertTrue(t.getMainCapacity() >= 79, "Test7: esperava crescimento, main=" + t.getMainCapacity());

        String[] del = {"K1","K3","K5","K7","K9","K11","K13","K15","K17","K19"};
        for (String d : del) t.delete(d);
        assertTrue(t.getDeletedNotRemoved() == 10,
                "Test7: deletedNotRemoved esperado 10, got=" + t.getDeletedNotRemoved());
        assertTrue(t.size() == 90, "Test7: size esperado 90, got=" + t.size());

        for (int i = 100; i < 130; i++) t.put("K" + i, i);
        assertTrue(t.getDeletedNotRemoved() == 10,
                "Test7: deletedNotRemoved deve manter 10, got=" + t.getDeletedNotRemoved());
        System.out.println("Local Test7 OK (deletedNotRemoved e upsizing)");
    }

    public static void main(String[] args) {
        runLocalTest5();
        runLocalTest7();
        System.out.println("Smoke/local tests concluídos.");
    }
}