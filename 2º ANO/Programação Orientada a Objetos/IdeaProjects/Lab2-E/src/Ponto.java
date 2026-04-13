/**
 * Responsabilidade: representar um ponto no plano.
 * @author Diogo Carvalho(a90247)
 * @version 19-02-2026
 * @inv As coordenadas x e y são valores double.
 */
public class Ponto {
    private final double x;
    private final double y;

    /**
     * Responsabilidade: criar um ponto a partir de duas coordenadas cartesianas.
     * @param x coordenada x
     * @param y coordenada y
     */
    public Ponto(double x, double y) {
        this.x = x;
        this.y = y;
    }

    /**
     * Responsabilidade: obter a coordenada x do ponto.
     * @return x
     */
    public double getX() {
        return x;
    }

    /**
     * Responsabilidade: obter a coordenada y do ponto.
     * @return y
     */
    public double getY() {
        return y;
    }

    /**
     * Responsabilidade: representar o ponto como texto no formato pedido.
     * @return string no formato (x,y)
     */
    @Override
    public String toString() {
        return String.format("(%.2f,%.2f)", x, y);
    }
}