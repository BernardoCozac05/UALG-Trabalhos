/**
 * Responsabilidade: representar um polígono definido por uma lista ordenada de vértices.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv O polígono tem pelo menos 3 vértices e estes estão ordenados no sentido dos ponteiros do relógio.
 */
public class Poligono extends Obstaculo {
    private static final double EPS = 1e-9;

    private final Ponto[] vertices;

    /**
     * Responsabilidade: criar um polígono a partir de uma lista ordenada de vértices.
     * @param vertices vértices do polígono
     * @pre vertices != null
     * @post fica criado o polígono
     */
    public Poligono(Ponto[] vertices) {
        this.vertices = new Ponto[vertices.length];
        for (int i = 0; i < vertices.length; i++) {
            this.vertices[i] = vertices[i];
        }

        if (getClass() == Poligono.class) {
            verificaInvariante();
        }
    }

    /**
     * Responsabilidade: obter o número de vértices.
     * @return número de vértices
     */
    public int getNumeroVertices() {
        return vertices.length;
    }

    /**
     * Responsabilidade: obter um vértice pela sua posição.
     * @param i índice do vértice
     * @return vértice na posição i
     */
    public Ponto getVertice(int i) {
        return vertices[i];
    }

    /**
     * Responsabilidade: determinar os pontos de interseção entre o polígono e um segmento de reta.
     * @param s segmento de reta
     * @return pontos de interseção ou null
     * @pre s != null
     */
    @Override
    public Ponto[] intersect(SegmentoReta s) {
        Ponto[] intersecoes = new Ponto[vertices.length];
        int n = 0;

        for (int i = 0; i < vertices.length; i++) {
            SegmentoReta aresta = new SegmentoReta(vertices[i], vertices[(i + 1) % vertices.length]);
            Ponto p = aresta.intersect(s);

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
     * @return true se forem iguais; false caso contrário
     */
    protected boolean pontosIguais(Ponto p1, Ponto p2) {
        return Math.abs(p1.getX() - p2.getX()) < EPS &&
                Math.abs(p1.getY() - p2.getY()) < EPS;
    }

    /**
     * Responsabilidade: verificar se o polígono tem número válido de vértices.
     * @return true se tiver pelo menos 3 vértices; false caso contrário
     */
    protected boolean temNumeroValidoVertices() {
        return vertices.length >= 3;
    }

    /**
     * Responsabilidade: verificar se os vértices estão ordenados no sentido horário.
     * @return true se estiverem em sentido horário; false caso contrário
     */
    protected boolean sentidoHorario() {
        double soma = 0.0;

        for (int i = 0; i < vertices.length; i++) {
            Ponto a = vertices[i];
            Ponto b = vertices[(i + 1) % vertices.length];
            soma += (b.getX() - a.getX()) * (b.getY() + a.getY());
        }

        return soma > EPS;
    }

    /**
     * Responsabilidade: verificar a invariante de instância do polígono.
     */
    private void verificaInvariante() {
        if (!temNumeroValidoVertices() || !sentidoHorario()) {
            System.out.println("Poligono:iv");
            System.exit(0);
        }
    }
}