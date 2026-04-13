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

            double ax = troco.getA().getX();
            double ay = troco.getA().getY();
            double bx = troco.getB().getX();
            double by = troco.getB().getY();

            double cx = s.getA().getX();
            double cy = s.getA().getY();
            double dx = s.getB().getX();
            double dy = s.getB().getY();

            double rX = bx - ax;
            double rY = by - ay;
            double sX = dx - cx;
            double sY = dy - cy;

            double rxs = rX * sY - rY * sX;
            double qpx = cx - ax;
            double qpy = cy - ay;
            double qpxr = qpx * rY - qpy * rX;

            Ponto p = null;

            if (Math.abs(rxs) <= EPS) {
                if (Math.abs(qpxr) <= EPS) {
                    double rr = rX * rX + rY * rY;
                    double t0 = (qpx * rX + qpy * rY) / rr;
                    double t1 = t0 + (sX * rX + sY * rY) / rr;

                    double lo = Math.max(0.0, Math.min(t0, t1));
                    double hi = Math.min(1.0, Math.max(t0, t1));

                    if (hi + EPS >= lo) {
                        double t = lo;
                        p = new Ponto(ax + t * rX, ay + t * rY);
                    }
                }
            } else {
                double t = (qpx * sY - qpy * sX) / rxs;
                double u = (qpx * rY - qpy * rX) / rxs;

                if (t >= -EPS && t <= 1.0 + EPS && u >= -EPS && u <= 1.0 + EPS) {
                    p = new Ponto(ax + t * rX, ay + t * rY);
                }
            }

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