import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema K.
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);

        Ponto start = new Ponto(sc.nextDouble(), sc.nextDouble());
        Ponto finish = new Ponto(sc.nextDouble(), sc.nextDouble());

        Vetor w = new Vetor(sc.nextDouble(), sc.nextDouble());

        double s = sc.nextDouble();
        sc.close();

        AutoPilot ap = new AutoPilot(start, finish);
        double t = ap.time(s);

        IO.println(String.format("%.2f", t));
        IO.println(ap.speed(w, t));
    }
}