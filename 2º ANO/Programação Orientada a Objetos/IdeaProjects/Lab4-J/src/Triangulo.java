/**
 * Responsabilidade: representar um triângulo definido por três vértices.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv O triângulo tem 3 vértices, estão no sentido horário e não são colineares.
 */
public class Triangulo extends Poligono {
    private static final double EPS = 1e-9;

    /**
     * Responsabilidade: criar um triângulo a partir de três vértices.
     * @param vertices vértices do triângulo
     * @pre vertices != null
     * @post fica criado o triângulo
     */
    public Triangulo(Ponto[] vertices) {
        super(vertices);
        verificaInvariante();
    }

    /**
     * Responsabilidade: verificar a invariante de instância do triângulo.
     */
    private void verificaInvariante() {
        if (getNumeroVertices() != 3 || !sentidoHorario()) {
            System.out.println("Triangulo:iv");
            System.exit(0);
        }

        Ponto a = getVertice(0);
        Ponto b = getVertice(1);
        Ponto c = getVertice(2);

        double area2 = (b.getX() - a.getX()) * (c.getY() - a.getY()) -
                (b.getY() - a.getY()) * (c.getX() - a.getX());

        if (Math.abs(area2) < EPS) {
            System.out.println("Triangulo:iv");
            System.exit(0);
        }
    }
}