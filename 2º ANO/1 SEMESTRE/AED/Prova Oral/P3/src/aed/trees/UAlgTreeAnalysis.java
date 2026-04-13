package aed.trees;

public class UAlgTreeAnalysis {

    // Profundidade mínima (em nº de nós) de um nó até uma folha
    public static <Key extends Comparable<Key>, Value>
    int minDepth(IUAlgTreeNode<Key, Value> node) {
        if (node == null) return 0;

        IUAlgTreeNode<Key, Value> left = node.getLeft();
        IUAlgTreeNode<Key, Value> right = node.getRight();

        // folha
        if (left == null && right == null) return 1;

        // se só um lado existe, seguir por esse
        if (left == null) return 1 + minDepth(right);
        if (right == null) return 1 + minDepth(left);

        return 1 + Math.min(minDepth(left), minDepth(right));
    }

    // Profundidade máxima (altura)
    public static <Key extends Comparable<Key>, Value>
    int maxDepth(IUAlgTreeNode<Key, Value> node) {
        if (node == null) return 0;

        IUAlgTreeNode<Key, Value> left = node.getLeft();
        IUAlgTreeNode<Key, Value> right = node.getRight();

        return 1 + Math.max(maxDepth(left), maxDepth(right));
    }

    // Rácio max/min a partir da raiz
    public static <Key extends Comparable<Key>, Value>
    double depthRatio(UAlgTree<Key, Value> tree) {
        IUAlgTreeNode<Key, Value> root = tree.getRoot();
        int min = minDepth(root);
        int max = maxDepth(root);
        if (min == 0) return 0.0;
        return (double) max / (double) min;
    }
}
