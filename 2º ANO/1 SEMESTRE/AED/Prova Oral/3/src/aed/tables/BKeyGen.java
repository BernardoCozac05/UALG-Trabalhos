package aed.tables;

import java.util.Random;

public class BKeyGen {

    public static int[] shuffledRange(int n, long seed) {
        int[] a = new int[n];
        for (int i = 0; i < n; i++) a[i] = i;

        Random rnd = new Random(seed);
        for (int i = n - 1; i > 0; i--) {
            int j = rnd.nextInt(i + 1);
            int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
        }
        return a;
    }
}
