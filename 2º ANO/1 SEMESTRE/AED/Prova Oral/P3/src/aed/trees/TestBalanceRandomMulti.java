package aed.trees;

import java.util.Random;

public class TestBalanceRandomMulti {

    private static void runOnce(int N, long seed) {
        UAlgTree<Integer, Integer> tree = new UAlgTree<>();
        Random rnd = new Random(seed);

        for (int i = 0; i < N; i++) {
            int k = rnd.nextInt();
            tree.put(k, i);
        }

        int min = UAlgTreeAnalysis.minDepth(tree.getRoot());
        int max = UAlgTreeAnalysis.maxDepth(tree.getRoot());
        double ratio = (double) max / min;

        System.out.println("seed = " + seed);
        System.out.println("minDepth = " + min);
        System.out.println("maxDepth = " + max);
        System.out.println("ratio    = " + ratio);
        System.out.println();
    }

    public static void main(String[] args) {
        int N = 100_000;
        long baseSeed = 12345L;
        int runs = 5;

        System.out.println("Random insert (multiple runs)");
        System.out.println("N = " + N + ", runs = " + runs);
        System.out.println();

        for (int i = 0; i < runs; i++) {
            runOnce(N, baseSeed + i);
        }
    }
}
