package aed.trees;

import java.util.Random;

public class TestSearchParetoDoubling {

    private static UAlgTree<Integer, Integer> buildTree(int N, long seed) {
        UAlgTree<Integer, Integer> tree = new UAlgTree<>();
        int[] keys = KeyGen.shuffledRange(N, seed);
        for (int k : keys) tree.put(k, k);
        return tree;
    }

    private static int pickParetoKey(Random rnd, int N) {
        int hotSize = Math.max(1, (int) (0.2 * N));

        if (rnd.nextDouble() < 0.8) {
            // 80% das vezes escolhe dentro do hot set
            return rnd.nextInt(hotSize);
        } else {
            // resto das vezes escolhe fora do hot set
            return hotSize + rnd.nextInt(N - hotSize);
        }
    }

    private static double avgNsPerGetPareto(UAlgTree<Integer, Integer> tree, int N, int queries, long seed) {
        Random rnd = new Random(seed);

        // warm-up
        for (int i = 0; i < 10_000; i++) {
            tree.get(pickParetoKey(rnd, N));
        }

        long start = System.nanoTime();
        for (int i = 0; i < queries; i++) {
            int k = pickParetoKey(rnd, N);
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

        System.out.println("Search Pareto 80/20 (Doubling Ratio)");
        System.out.println("queries per N = " + queries);
        System.out.println();

        double prev = -1;

        for (int N : Ns) {
            UAlgTree<Integer, Integer> tree = buildTree(N, buildSeed);

            double avg = avgNsPerGetPareto(tree, N, queries, querySeed);

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
