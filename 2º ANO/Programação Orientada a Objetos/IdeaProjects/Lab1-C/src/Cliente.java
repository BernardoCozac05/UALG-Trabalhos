import java.util.Scanner;

public class Cliente {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        double x0 = sc.nextDouble();
        double y0 = sc.nextDouble();
        double x1 = sc.nextDouble();
        double y1 = sc.nextDouble();

        Vetor u = new Vetor(x0, y0);
        Vetor v = new Vetor(x1, y1);

        double cs = u.cosineSimilarity(v);
        System.out.println(String.format("%.2f", cs));

        sc.close();
    }
}
