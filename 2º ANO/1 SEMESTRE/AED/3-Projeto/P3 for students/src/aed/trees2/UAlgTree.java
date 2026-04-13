/*Trabalho Realizado Por:
 * Diogo Carvalho - a90247
 * Diogo Freitas - a90147
 * 
 * PL1- G10
 */

package aed.trees2;

import java.util.ArrayList;
import java.util.List;

class UAlgTreeNode<Key extends Comparable<Key>, Value> implements IUAlgTreeNode<Key, Value> {
    Key key;
    Value value;
    UAlgTreeNode<Key, Value> left, right;
    int size = 1;
    int weight = 1;

    UAlgTreeNode(Key key, Value value) {
        this.key = key;
        this.value = value;
    }

    // Getters do nó
    public IUAlgTreeNode<Key, Value> getLeft() {return left;}
    public IUAlgTreeNode<Key, Value> getRight() {return right;}
    public int getSize() {return size;}
    public int getWeight() {return weight;}
    public Key getKey() {return key;}
    public Value getValue() {return value;}
}

public class UAlgTree<Key extends Comparable<Key>, Value> {
    private UAlgTreeNode<Key, Value> root;

    // Flags de apoio à promoção segura no get
    private boolean rotated;
    private Value foundValue;

    // Construtor
    public UAlgTree() {}

    // Retorna a raiz
    public IUAlgTreeNode<Key, Value> getRoot() {return root;}

    // Número de pares chave-valor
    public int size() {return root == null ? 0 : root.size;}

    private int size(UAlgTreeNode<Key, Value> node) {return node == null ? 0 : node.size;}

    // Peso com w(null)=1
    private int weight(UAlgTreeNode<Key, Value> node) {return node == null ? 0 : node.weight;}

    // Atualiza size e weight de um nó
    private void update(UAlgTreeNode<Key, Value> node) {
        if (node == null) return;
        node.size = 1 + size(node.left) + size(node.right);
        node.weight = 1 + weight(node.left) + weight(node.right);
    }

    // Verifica existência de chave
    public boolean containsKey(Key key) {return find(root, key) != null;}

    // Alias
    public boolean contains(Key key) { return containsKey(key); }

    // Procura simples (sem promoções)
    private Value find(UAlgTreeNode<Key, Value> node, Key key) {
        if (node == null) return null;
        int cmp = key.compareTo(node.key);
        if (cmp == 0) return node.value;
        else if (cmp < 0) return find(node.left, key);
        else return find(node.right, key);
    }

    // Pesquisa com promoção segura de 1 nível
    public Value get(Key key) {
        if (root == null) return null;

        rotated = false;
        foundValue = null;

        root = getAndPromote(root, key);

        // Caso em que o pai do nó encontrado é a raiz e ainda não houve rotação
        if (foundValue != null && !rotated && root != null) {
            int cmp = key.compareTo(root.key);
            if (cmp < 0 && root.left != null) root = rotateRight(root);
            else if (cmp > 0 && root.right != null) root = rotateLeft(root);
        }

        return foundValue;
    }

    // Procura recursiva e tenta uma rotação segura no caminho de retorno
    private UAlgTreeNode<Key, Value> getAndPromote(UAlgTreeNode<Key, Value> node, Key key) {
        if (node == null) return null;

        int cmp = key.compareTo(node.key);

        if (cmp == 0) {
            foundValue = node.value;
            return node;
        }

        if (cmp < 0) {
            node.left = getAndPromote(node.left, key);

            if (foundValue != null) {
                // Não atualizamos pesos aqui para evitar custo extra em muitos gets
                if (!rotated && node.left != null && canRotateRight(node)) {
                    node = rotateRight(node);
                    rotated = true;
                }
            }

        } else {
            node.right = getAndPromote(node.right, key);

            if (foundValue != null) {
                // Não atualizamos pesos aqui para evitar custo extra em muitos gets
                if (!rotated && node.right != null && canRotateLeft(node)) {
                    node = rotateLeft(node);
                    rotated = true;
                }
            }
        }

        return node;
    }

    // Condição de rotação segura à direita
    private boolean canRotateRight(UAlgTreeNode<Key, Value> node) {
        int wParent = weight(node);
        int wLeft = weight(node.left.left);
        int wRight = weight(node.left.right);
        return wParent <= 3.5 * wLeft && wParent <= 3.5 * (wRight + wLeft);
    }

    // Condição de rotação segura à esquerda
    private boolean canRotateLeft(UAlgTreeNode<Key, Value> node) {
        int wParent = weight(node);
        int wLeft = weight(node.right.left);
        int wRight = weight(node.right.right);
        return wParent <= 3.5 * wRight && wParent <= 3.5 * (wLeft + wRight);
    }

    // Inserção/atualização
    public void put(Key key, Value value) {
        if (value == null) { delete(key); return; }
        root = put(root, key, value);
    }

    private UAlgTreeNode<Key, Value> put(UAlgTreeNode<Key, Value> node, Key key, Value value) {
        if (node == null) return new UAlgTreeNode<>(key, value);

        int cmp = key.compareTo(node.key);
        if (cmp == 0) node.value = value;
        else if (cmp < 0) node.left = put(node.left, key, value);
        else node.right = put(node.right, key, value);

        update(node);
        return rebalance(node);
    }

    // Rebalanceamento por pesos (após put/delete)
    private UAlgTreeNode<Key, Value> rebalance(UAlgTreeNode<Key, Value> node) {
        if (node == null) return null;

        while (true) {
            int wLeft = weight(node.left);
            int wRight = weight(node.right);

            // Direita demasiado pesada
            if (wLeft < 0.4 * wRight && node.right != null) {
                if (weight(node.right.left) > 1.5 * weight(node.right.right)) {
                    node.right = rotateRight(node.right);
                    update(node);
                }
                node = rotateLeft(node);

            // Esquerda demasiado pesada
            } else if (wRight < 0.4 * wLeft && node.left != null) {
                if (weight(node.left.right) > 1.5 * weight(node.left.left)) {
                    node.left = rotateLeft(node.left);
                    update(node);
                }
                node = rotateRight(node);

            } else break;
        }

        return node;
    }

    // Rotação à direita
    private UAlgTreeNode<Key, Value> rotateRight(UAlgTreeNode<Key, Value> parent) {
        UAlgTreeNode<Key, Value> child = parent.left;
        parent.left = child.right;
        child.right = parent;
        update(parent);
        update(child);
        return child;
    }

    // Rotação à esquerda
    private UAlgTreeNode<Key, Value> rotateLeft(UAlgTreeNode<Key, Value> parent) {
        UAlgTreeNode<Key, Value> child = parent.right;
        parent.right = child.left;
        child.left = parent;
        update(parent);
        update(child);
        return child;
    }

    // Remoção
    public void delete(Key key) { root = delete(root, key); }

    private UAlgTreeNode<Key, Value> delete(UAlgTreeNode<Key, Value> node, Key key) {
        if (node == null) return null;

        int cmp = key.compareTo(node.key);
        if (cmp < 0) node.left = delete(node.left, key);
        else if (cmp > 0) node.right = delete(node.right, key);
        else {
            if (node.left == null) return node.right;
            if (node.right == null) return node.left;

            UAlgTreeNode<Key, Value> successor = min(node.right);
            node.key = successor.key;
            node.value = successor.value;
            node.right = delete(node.right, successor.key);
        }

        update(node);
        return rebalance(node);
    }

    // Menor nó numa sub-árvore
    private UAlgTreeNode<Key, Value> min(UAlgTreeNode<Key, Value> node) {
        return node.left == null ? node : min(node.left);
    }

    // Itera chaves por ordem
    public Iterable<Key> keys() {
        List<Key> list = new ArrayList<>();
        inorderKeys(root, list);
        return list;
    }

    private void inorderKeys(UAlgTreeNode<Key, Value> node, List<Key> list) {
        if (node == null) return;
        inorderKeys(node.left, list);
        list.add(node.key);
        inorderKeys(node.right, list);
    }

    // Itera valores pela ordem das chaves
    public Iterable<Value> values() {
        List<Value> list = new ArrayList<>();
        inorderValues(root, list);
        return list;
    }

    private void inorderValues(UAlgTreeNode<Key, Value> node, List<Value> list) {
        if (node == null) return;
        inorderValues(node.left, list);
        list.add(node.value);
        inorderValues(node.right, list);
    }

    // Cópia superficial
    public UAlgTree<Key, Value> shallowCopy() {
        UAlgTree<Key, Value> copy = new UAlgTree<>();
        copy.root = copyNode(root);
        return copy;
    }

    private UAlgTreeNode<Key, Value> copyNode(UAlgTreeNode<Key, Value> node) {
        if (node == null) return null;
        UAlgTreeNode<Key, Value> newNode = new UAlgTreeNode<>(node.key, node.value);
        newNode.left = copyNode(node.left);
        newNode.right = copyNode(node.right);
        newNode.size = node.size;
        newNode.weight = node.weight;
        return newNode;
    }

    // Testes manuais
    public static void main(String[] args) {
        System.out.println("=== Testes UAlgTree ===\n");

        UAlgTree<Integer, String> tree = new UAlgTree<>();

        tree.put(50, "cinquenta");
        tree.put(30, "trinta");
        tree.put(70, "setenta");
        tree.put(20, "vinte");
        tree.put(40, "quarenta");
        System.out.println("Após inserir 5 elementos: size=" + tree.size());

        System.out.println("get(50)=" + tree.get(50) + ", containsKey(99)=" + tree.containsKey(99));

        tree.put(50, "ATUALIZADO");
        System.out.println("Após update: get(50)=" + tree.get(50));

        tree.delete(20);
        System.out.println("Após delete(20): containsKey(20)=" + tree.containsKey(20));

        tree.put(30, null);
        System.out.println("Após put(30,null): size=" + tree.size());

        System.out.print("Keys: ");
        for (Integer k : tree.keys()) System.out.print(k + " ");
        System.out.println();

        UAlgTree<Integer, String> copia = tree.shallowCopy();
        copia.put(999, "novo");
        System.out.println("Cópia tem 999? " + copia.containsKey(999) + ", Original? " + tree.containsKey(999));

        UAlgTree<Integer, Integer> seq = new UAlgTree<>();
        for (int i = 1; i <= 15; i++) seq.put(i, i);
        System.out.println("Inserção 1-15 sequencial, raiz=" + seq.getRoot().getKey());

        for (int i = 0; i < 5; i++) seq.get(14);
        System.out.println("Após 5 gets(14), raiz=" + seq.getRoot().getKey());

        System.out.println("\n=== Testes OK ===");
    }
}
