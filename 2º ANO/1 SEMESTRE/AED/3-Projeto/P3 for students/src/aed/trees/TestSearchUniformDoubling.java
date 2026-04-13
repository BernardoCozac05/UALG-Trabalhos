package aed.trees;

import java.util.Random;

public class TestSearchUniformDoubling {

    private static UAlgTree<Integer, Integer> buildTree(int N, long seed) {
        UAlgTree<Integer, Integer> tree = new UAlgTree<>();
        int[] keys = KeyGen.shuffledRange(N, seed);
        for (int k : keys) tree.put(k, k);
        return tree;
    }

    private static double avgNsPerGet(UAlgTree<Integer, Integer> tree, int N, int queries, long seed) {
        Random rnd = new Random(seed);

        for (int i = 0; i < 10_000; i++) {
            tree.get(rnd.nextInt(N));
        }

        long start = System.nanoTime();
        for (int i = 0; i < queries; i++) {
            int k = rnd.nextInt(N);
            tree.get(k);
        }
        long end = System.nanoTime();

        return (double) (end - start) / queries;
    }

    public static void main(String[] args) {
        int[] Ns = {50_000, 100_000, 200_000, 400_000};
        int queries = 300_000;

        long buildSeed = 999L;
        long querySeed = 123L;

        System.out.println("Search Uniform (Doubling Ratio)");
        System.out.println("queries per N = " + queries);
        System.out.println();

        double prev = -1;

        for (int N : Ns) {
            UAlgTree<Integer, Integer> tree = buildTree(N, buildSeed);

            double avg = avgNsPerGet(tree, N, queries, querySeed);

            System.out.printf("N=%d | avg=%.1f ns/get", N, avg);

            if (prev > 0) {
                double R = avg / prev;
                System.out.printf(" | R=%.3f", R);
            }

            System.out.println();
            prev = avg;

            buildSeed += 1;
            querySeed += 1;
        }
    }
}
