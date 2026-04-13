/**
 * Responsabilidade: representar um vetor e disponibilizar operações básicas sobre vetores.
 * @author Diogo Carvalho(a90247)
 * @version 19-02-2026
 * @inv O módulo do vetor é diferente de zero.
 */
public class Vetor {
    private static final double EPS = 1e-9;

    private final double x, y;

    /**
     * Responsabilidade: criar um vetor a partir das suas componentes.
     * @param x componente x
     * @param y componente y
     */
    public Vetor(double x, double y) {
        this.x = x;
        this.y = y;
        verificaInvariante();
    }

    /**
     * Responsabilidade: criar um vetor posição a partir de um ponto.
     * @param p ponto que define o vetor posição
     */
    public Vetor(Ponto p) {
        this.x = p.getX();
        this.y = p.getY();
        verificaInvariante();
    }

    /**
     * Responsabilidade: obter a componente x do vetor.
     * @return x
     */
    public double getX() {
        return x;
    }

    /**
     * Responsabilidade: obter a componente y do vetor.
     * @return y
     */
    public double getY() {
        return y;
    }

    /**
     * Responsabilidade: calcular o módulo do vetor.
     * @return módulo
     */
    public double modulo() {
        return Math.sqrt(x * x + y * y);
    }

    /**
     * Responsabilidade: calcular o produto interno entre este vetor e outro.
     * @param other outro vetor
     * @return produto interno
     */
    public double produtoInterno(Vetor other) {
        return this.x * other.x + this.y * other.y;
    }

    /**
     * Responsabilidade: calcular a similaridade cosseno entre este vetor e outro.
     * @param other outro vetor
     * @return similaridade
     */
    public double cosineSimilarity(Vetor other) {
        return this.produtoInterno(other) / (this.modulo() * other.modulo());
    }

    /**
     * Responsabilidade: verificar a invariante de instância do vetor.
     */
    private void verificaInvariante() {
        if ((x * x + y * y) <= EPS * EPS) {
            System.out.println("Vetor:iv");
            System.exit(0);
        }
    }
}