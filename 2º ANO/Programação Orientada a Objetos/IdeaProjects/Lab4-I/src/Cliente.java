import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema I.
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);

        String[] rotaTokens = sc.nextLine().trim().split("\\s+");
        String[] segTokens = sc.nextLine().trim().split("\\s+");
        sc.close();

        Ponto[] pontos = new Ponto[rotaTokens.length / 2];
        int k = 0;
        for (int i = 0; i < rotaTokens.length; i += 2) {
            pontos[k] = new Ponto(Double.parseDouble(rotaTokens[i]),
                    Double.parseDouble(rotaTokens[i + 1]));
            k++;
        }

        Route rota = new Route(pontos);
        IO.println(String.format("%.2f", rota.comprimento()));

        Ponto a = new Ponto(Double.parseDouble(segTokens[0]), Double.parseDouble(segTokens[1]));
        Ponto b = new Ponto(Double.parseDouble(segTokens[2]), Double.parseDouble(segTokens[3]));
        SegmentoReta s = new SegmentoReta(a, b);

        Ponto[] intersecoes = rota.intersect(s);

        if (intersecoes == null) {
            IO.println("null");
        } else {
            String out = "";
            for (int i = 0; i < intersecoes.length; i++) {
                if (i > 0) {
                    out += " ";
                }
                out += intersecoes[i].toString();
            }
            IO.println(out);
        }
    }
}