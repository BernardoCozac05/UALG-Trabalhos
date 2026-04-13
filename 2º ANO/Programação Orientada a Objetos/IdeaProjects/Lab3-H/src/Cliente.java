import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema H.
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);

        //Get start and finish points
        Ponto start = new Ponto(sc.nextDouble(), sc.nextDouble());
        Ponto finish = new Ponto(sc.nextDouble(), sc.nextDouble());

        //Get wind speed and direction
        Vetor w = new Vetor(sc.nextDouble(), sc.nextDouble());

        //Get linear speed
        double s = sc.nextDouble();
        sc.close();

        //Setup auto pilot and compute:
        // i) desired time to reach the finish point
        // ii) vectorial speed required
        AutoPilot ap = new AutoPilot(start, finish);
        double t = ap.time(w, s);
        IO.println(String.format("%.2f", t));
        IO.println(ap.speed(w, t));
    }
}