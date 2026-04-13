import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 19-02-2026
 * @inv Não aplicável.
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema E.
     * @param args não utilizado
     */
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        double xA = sc.nextDouble();
        double yA = sc.nextDouble();
        double xB = sc.nextDouble();
        double yB = sc.nextDouble();

        double xv = sc.nextDouble();
        double yv = sc.nextDouble();

        Ponto a = new Ponto(xA, yA);
        Ponto b = new Ponto(xB, yB);
        SegmentoReta s = new SegmentoReta(a, b);

        Vetor v = new Vetor(new Ponto(xv, yv));

        Ponto p = s.intersect(v);
        if (p == null) {
            System.out.println("null");
        } else {
            System.out.println(p);
        }

        sc.close();
    }
}