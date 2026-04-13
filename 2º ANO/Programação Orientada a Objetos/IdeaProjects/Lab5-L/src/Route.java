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
     * Responsabilidade: calcular o tempo total necessário para percorrer a rota.
     * @param vl velocidade linear constante
     * @return tempo total
     */
    public double time(double vl) {
        double soma = 0.0;
        for (int i = 0; i < pontos.length - 1; i++) {
            AutoPilot ap = new AutoPilot(pontos[i], pontos[i + 1]);
            soma += ap.time(vl);
        }
        return soma;
    }

    /**
     * Responsabilidade: calcular as velocidades vetoriais necessárias para cada segmento da rota.
     * @param w vetor velocidade do vento
     * @param vl velocidade linear constante
     * @return array de vetores velocidade, um por segmento
     */
    public Vetor[] speed(Vetor w, double vl) {
        Vetor[] velocidades = new Vetor[pontos.length - 1];
        for (int i = 0; i < pontos.length - 1; i++) {
            AutoPilot ap = new AutoPilot(pontos[i], pontos[i + 1]);
            double t = ap.time(vl);
            velocidades[i] = ap.speed(w, t);
        }
        return velocidades;
    }

    /**
     * Responsabilidade: calcular a posição do avião ao fim de um dado tempo após o início da viagem.
     * @param vl velocidade linear constante
     * @param t tempo desde o início da viagem
     * @return posição do avião
     */
    public Ponto position(double vl, double t) {
        if (t <= 0.0) {
            return pontos[0];
        }

        double tempoDecorrido = 0.0;

        for (int i = 0; i < pontos.length - 1; i++) {
            AutoPilot ap = new AutoPilot(pontos[i], pontos[i + 1]);
            double tempoSegmento = ap.time(vl);

            if (t <= tempoDecorrido + tempoSegmento + EPS) {
                double dt = t - tempoDecorrido;
                Vetor r = new Vetor(pontos[i + 1]).sub(new Vetor(pontos[i]));
                Vetor vg = r.mult(1.0 / tempoSegmento);
                Vetor deslocamento = vg.mult(dt);

                return new Ponto(pontos[i].getX() + deslocamento.getX(),
                        pontos[i].getY() + deslocamento.getY());
            }

            tempoDecorrido += tempoSegmento;
        }

        return pontos[pontos.length - 1];
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