package aed.trees;

import java.util.Random;

public class TestBalanceRandom {

    public static void main(String[] args) {
        int N = 100_000;
        long seed = 12345L;

        UAlgTree<Integer, Integer> tree = new UAlgTree<>();
        Random rnd = new Random(seed);

        for (int i = 0; i < N; i++) {
            int k = rnd.nextInt();
            tree.put(k, i);
        }

        int min = UAlgTreeAnalysis.minDepth(tree.getRoot());
        int max = UAlgTreeAnalysis.maxDepth(tree.getRoot());
        double ratio = (double) max / min;

        System.out.println("Random insert (single run)");
        System.out.println("N = " + N);
        System.out.println("seed = " + seed);
        System.out.println("minDepth = " + min);
        System.out.println("maxDepth = " + max);
        System.out.println("ratio    = " + ratio);
    }
}
