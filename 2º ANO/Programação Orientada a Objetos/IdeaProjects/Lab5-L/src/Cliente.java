import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema L.
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);

        String[] rotaTokens = sc.nextLine().trim().split("\\s+");
        String[] ventoTokens = sc.nextLine().trim().split("\\s+");
        double vl = Double.parseDouble(sc.nextLine().trim());
        double t = Double.parseDouble(sc.nextLine().trim());
        sc.close();

        if (rotaTokens.length % 2 != 0) {
            IO.println("Rota:iv");
            return;
        }

        Ponto[] pontos = new Ponto[rotaTokens.length / 2];
        int k = 0;
        for (int i = 0; i < rotaTokens.length; i += 2) {
            pontos[k] = new Ponto(Double.parseDouble(rotaTokens[i]),
                    Double.parseDouble(rotaTokens[i + 1]));
            k++;
        }

        Route rota = new Route(pontos);
        Vetor w = new Vetor(Double.parseDouble(ventoTokens[0]),
                Double.parseDouble(ventoTokens[1]));

        IO.println(String.format("%.2f", rota.comprimento()));
        IO.println(String.format("%.2f", rota.time(vl)));
        IO.println(rota.position(vl, t));

        Vetor[] velocidades = rota.speed(w, vl);
        String out = "";
        for (int i = 0; i < velocidades.length; i++) {
            if (i > 0) {
                out += " ";
            }
            out += velocidades[i].toString();
        }
        IO.println(out);
    }
}