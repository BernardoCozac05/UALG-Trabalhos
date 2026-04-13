/**
 * Responsabilidade: representar uma rota definida por uma sequência ordenada de pontos.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv A rota tem pelo menos dois pontos.
 */
public class Route {
    private static final double EPS = 1e-9;

    private final Ponto[] pontos;

    /**
     * Responsabilidade: criar uma rota a partir de um conjunto ordenado de pontos.
     * @param pontos pontos da rota
     * @pre pontos != null && pontos.length >= 2
     * @post fica criada uma rota com os pontos recebidos
     */
    public Route(Ponto[] pontos) {
        this.pontos = new Ponto[pontos.length];
        for (int i = 0; i < pontos.length; i++) {
            this.pontos[i] = pontos[i];
        }
    }

    /**
     * Responsabilidade: calcular o comprimento da rota.
     * @return comprimento total da rota
     * @post resultado >= 0
     */
    public double comprimento() {
        double soma = 0.0;
        for (int i = 0; i < pontos.length - 1; i++) {
            soma += pontos[i].dist(pontos[i + 1]);
        }
        return soma;
    }

    /**
     * Responsabilidade: obter os pontos de interseção da rota com um segmento de reta.
     * @param s segmento de reta
     * @return array com os pontos de interseção ordenados desde o início da rota, ou null
     * @pre s != null
     */
    public Ponto[] intersect(SegmentoReta s) {
        Ponto[] intersecoes = new Ponto[pontos.length - 1];
        int n = 0;

        for (int i = 0; i < pontos.length - 1; i++) {
            SegmentoReta troco = new SegmentoReta(pontos[i], pontos[i + 1]);
            Ponto p = troco.intersect(s);

            if (p != null) {
                boolean repetido = false;
                for (int j = 0; j < n; j++) {
                    if (pontosIguais(intersecoes[j], p)) {
                        repetido = true;
                    }
                }
                if (!repetido) {
                    intersecoes[n] = p;
                    n++;
                }
            }
        }

        if (n == 0) {
            return null;
        }

        Ponto[] resultado = new Ponto[n];
        for (int i = 0; i < n; i++) {
            resultado[i] = intersecoes[i];
        }
        return resultado;
    }

    /**
     * Responsabilidade: obter os pontos de interseção da rota com um obstáculo.
     * @param o obstáculo
     * @return array com os pontos de interseção ordenados desde o início da rota, ou null
     * @pre o != null
     */
    public Ponto[] intersect(Obstaculo o) {
        Ponto[] intersecoes = new Ponto[pontos.length * 2];
        int n = 0;

        for (int i = 0; i < pontos.length - 1; i++) {
            SegmentoReta troco = new SegmentoReta(pontos[i], pontos[i + 1]);
            Ponto[] pts = o.intersect(troco);

            if (pts != null) {
                for (int k = 0; k < pts.length; k++) {
                    boolean repetido = false;
                    for (int j = 0; j < n; j++) {
                        if (pontosIguais(intersecoes[j], pts[k])) {
                            repetido = true;
                        }
                    }
                    if (!repetido) {
                        intersecoes[n] = pts[k];
                        n++;
                    }
                }
            }
        }

        if (n == 0) {
            return null;
        }

        Ponto[] resultado = new Ponto[n];
        for (int i = 0; i < n; i++) {
            resultado[i] = intersecoes[i];
        }
        return resultado;
    }

    /**
     * Responsabilidade: verificar se dois pontos são iguais com tolerância.
     * @param p1 primeiro ponto
     * @param p2 segundo ponto
     * @return true se os pontos forem iguais; false caso contrário
     */
    private boolean pontosIguais(Ponto p1, Ponto p2) {
        return Math.abs(p1.getX() - p2.getX()) < EPS &&
                Math.abs(p1.getY() - p2.getY()) < EPS;
    }
}