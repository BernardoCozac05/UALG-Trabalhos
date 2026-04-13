package aed.tables;

import java.util.function.Function;

public class BTableBuilder {

    private static final Function<Integer, Integer> HC2_INT = k -> {
        int h = k;
        h ^= (h >>> 16);
        h *= 0x7feb352d;
        h ^= (h >>> 15);
        h *= 0x846ca68b;
        h ^= (h >>> 16);
        return (h == 0) ? 1 : h;
    };

    public static UAlshTable<Integer, Integer> buildIntTable(int n, long seed) {
        UAlshTable<Integer, Integer> t = new UAlshTable<>(HC2_INT);

        int[] keys = BKeyGen.shuffledRange(n, seed);
        for (int k : keys) {
            t.put(k, k);
        }

        return t;
    }
}
