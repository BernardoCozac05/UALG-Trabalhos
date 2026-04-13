package aed.tables;

import java.util.function.Function;

public class TestBInsertionLoad {

    private static final Function<Integer, Integer> HC2_INT = k -> {
        int h = k;
        h ^= (h >>> 16);
        h *= 0x7feb352d;
        h ^= (h >>> 15);
        h *= 0x846ca68b;
        h ^= (h >>> 16);
        return (h == 0) ? 1 : h;
    };

    private static long timeInsertBlock(UAlshTable<Integer, Integer> t, int startKey, int count) {
        long start = System.nanoTime();
        for (int i = 0; i < count; i++) {
            int k = startKey + i;
            t.put(k, k);
        }
        long end = System.nanoTime();
        return end - start;
    }

    public static void main(String[] args) {
        int total = 200_000;
        int block = 20_000;

        UAlshTable<Integer, Integer> t = new UAlshTable<>(HC2_INT);

        System.out.println("B - Insertion Load (by blocks)");
        System.out.println("total inserts = " + total + ", block = " + block);
        System.out.println();

        int inserted = 0;
        int key = 0;

        while (inserted < total) {
            long ns = timeInsertBlock(t, key, block);
            inserted += block;
            key += block;

            double avg = (double) ns / block;

            System.out.printf("inserted=%d | avg=%.1f ns/put | size=%d%n",
                    inserted, avg, t.size());
        }
    }
}
