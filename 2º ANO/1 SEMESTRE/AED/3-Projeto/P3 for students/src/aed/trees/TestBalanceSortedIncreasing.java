package aed.trees;

public class TestBalanceSortedIncreasing {

    public static void main(String[] args) {
        int N = 100_000;

        UAlgTree<Integer, Integer> tree = new UAlgTree<>();

        for (int i = 0; i < N; i++) {
            tree.put(i, i);
        }

        int min = UAlgTreeAnalysis.minDepth(tree.getRoot());
        int max = UAlgTreeAnalysis.maxDepth(tree.getRoot());
        double ratio = (double) max / min;

        System.out.println("Sorted increasing insert");
        System.out.println("N = " + N);
        System.out.println("minDepth = " + min);
        System.out.println("maxDepth = " + max);
        System.out.println("ratio    = " + ratio);
    }
}
