import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, criar os objetos necessários e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class Cliente {
    /**
     * Responsabilidade: executar o programa do Problema J.
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);

        String[] rotaTokens = sc.nextLine().trim().split("\\s+");
        String[] obstTokens = sc.nextLine().trim().split("\\s+");
        sc.close();

        Ponto[] pontosRota = new Ponto[rotaTokens.length / 2];
        int k = 0;
        for (int i = 0; i < rotaTokens.length; i += 2) {
            pontosRota[k] = new Ponto(Double.parseDouble(rotaTokens[i]),
                    Double.parseDouble(rotaTokens[i + 1]));
            k++;
        }

        Route rota = new Route(pontosRota);
        Obstaculo obstaculo;
        char tipo = obstTokens[0].charAt(0);

        if (tipo == 'C') {
            Ponto centro = new Ponto(Double.parseDouble(obstTokens[1]), Double.parseDouble(obstTokens[2]));
            double raio = Double.parseDouble(obstTokens[3]);
            obstaculo = new Circulo(centro, raio);
        } else {
            Ponto[] vertices = new Ponto[(obstTokens.length - 1) / 2];
            int n = 0;
            for (int i = 1; i < obstTokens.length; i += 2) {
                vertices[n] = new Ponto(Double.parseDouble(obstTokens[i]),
                        Double.parseDouble(obstTokens[i + 1]));
                n++;
            }

            if (tipo == 'P') {
                obstaculo = new Poligono(vertices);
            } else if (tipo == 'T') {
                obstaculo = new Triangulo(vertices);
            } else if (tipo == 'R') {
                obstaculo = new Retangulo(vertices);
            } else {
                obstaculo = new Quadrado(vertices);
            }
        }

        Ponto[] intersecoes = rota.intersect(obstaculo);

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