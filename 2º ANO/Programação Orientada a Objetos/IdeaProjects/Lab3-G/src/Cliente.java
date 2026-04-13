import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema G.
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);

        //Get start and finish points
        Ponto start = new Ponto(sc.nextDouble(), sc.nextDouble());
        Ponto finish = new Ponto(sc.nextDouble(), sc.nextDouble());
        Vetor r = new Vetor(finish).sub(new Vetor(start));

        //Get wind speed and direction
        Vetor w = new Vetor(sc.nextDouble(), sc.nextDouble());

        //Get the required time to reach the finish point
        double t = sc.nextDouble();
        sc.close();

        //Compute the required speed to reach the finish point in time t
        Vetor s = r.sub(w).mult(1 / t);
        IO.println(s);
    }
}