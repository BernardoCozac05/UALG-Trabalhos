package aed.tables;

import java.util.Random;

public class TestBSearchParetoDoubling {

    private static boolean contains(UAlshTable<Integer,Integer> t, int k) {
        return t.get(k) != null;
    }

    private static int pickPareto(Random rnd, int n) {
        int hot = Math.max(1, (int)(0.2 * n));
        if (rnd.nextDouble() < 0.8) return rnd.nextInt(hot);
        return hot + rnd.nextInt(n - hot);
    }

    private static double avgNsContainsPareto(UAlshTable<Integer,Integer> t, int n, int queries, long seed) {
        Random rnd = new Random(seed);

        for (int i = 0; i < 10_000; i++) contains(t, pickPareto(rnd, n));

        long start = System.nanoTime();
        for (int i = 0; i < queries; i++) {
            contains(t, pickPareto(rnd, n));
        }
        long end = System.nanoTime();

        return (double)(end - start) / queries;
    }

    public static void main(String[] args) {
        int[] Ns = {50_000, 100_000, 200_000, 400_000};
        int queries = 300_000;

        System.out.println("B - Search Pareto 80/20 (Doubling)");
        System.out.println("queries per N = " + queries);
        System.out.println();

        double prev = -1;
        long buildSeed = 1000, querySeed = 2000;

        for (int n : Ns) {
            UAlshTable<Integer,Integer> t = BTableBuilder.buildIntTable(n, buildSeed);

            double avg = avgNsContainsPareto(t, n, queries, querySeed);

            System.out.printf("N=%d | avg=%.1f ns/contains", n, avg);
            if (prev > 0) System.out.printf(" | R=%.3f", avg / prev);
            System.out.println();

            prev = avg;
            buildSeed++; querySeed++;
        }
    }
}
