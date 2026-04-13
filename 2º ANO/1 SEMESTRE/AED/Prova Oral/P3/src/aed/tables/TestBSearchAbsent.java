package aed.tables;

import java.util.Random;

public class TestBSearchAbsent {

    private static boolean contains(UAlshTable<Integer,Integer> t, int k) {
        return t.get(k) != null;
    }

    private static double avgNsAbsent(UAlshTable<Integer,Integer> t, int n, int queries, long seed) {
        Random rnd = new Random(seed);
        int base = n + 1_000_000;

        for (int i = 0; i < 10_000; i++) contains(t, base + rnd.nextInt(n));

        long start = System.nanoTime();
        for (int i = 0; i < queries; i++) {
            contains(t, base + rnd.nextInt(n));
        }
        long end = System.nanoTime();

        return (double)(end - start) / queries;
    }

    public static void main(String[] args) {
        int[] Ns = {50_000, 100_000, 200_000, 400_000};
        int queries = 300_000;

        System.out.println("B - Search Absent Keys");
        System.out.println("queries per N = " + queries);
        System.out.println();

        long buildSeed = 555, querySeed = 777;

        for (int n : Ns) {
            UAlshTable<Integer,Integer> t = BTableBuilder.buildIntTable(n, buildSeed);
            double avg = avgNsAbsent(t, n, queries, querySeed);

            System.out.printf("N=%d | avg=%.1f ns/contains(absent)%n", n, avg);

            buildSeed++; querySeed++;
        }
    }
}
