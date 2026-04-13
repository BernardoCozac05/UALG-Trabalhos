import java.util.Scanner;

public class Cliente {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        double x = sc.nextDouble();
        double y = sc.nextDouble();

        Vetor v = new Vetor(x, y);

        System.out.println(String.format("%.2f", v.modulo()));

        sc.close();
    }
}
