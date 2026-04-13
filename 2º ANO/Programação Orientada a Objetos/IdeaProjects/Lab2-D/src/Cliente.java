import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 19-02-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema D.
     * @param args não utilizado
     */
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        double xo = sc.nextDouble();
        double yo = sc.nextDouble();

        double xv = sc.nextDouble();
        double yv = sc.nextDouble();

        Ponto extremo = new Ponto(xo, yo);
        Ponto pontoV = new Ponto(xv, yv);

        Vetor v = new Vetor(pontoV);
        SegmentoReta sr = new SegmentoReta(extremo, v);

        System.out.println(sr);

        sc.close();
    }
}