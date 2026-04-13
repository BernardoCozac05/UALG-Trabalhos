/**
 * Responsabilidade: representar um retângulo definido por quatro vértices.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv O retângulo tem 4 vértices, está no sentido horário, os lados opostos são iguais e as diagonais têm o mesmo comprimento.
 */
public class Retangulo extends Poligono {
    private static final double EPS = 1e-9;

    /**
     * Responsabilidade: criar um retângulo a partir de quatro vértices.
     * @param vertices vértices do retângulo
     * @pre vertices != null
     * @post fica criado o retângulo
     */
    public Retangulo(Ponto[] vertices) {
        super(vertices);
        verificaInvariante();
    }

    /**
     * Responsabilidade: verificar a invariante de instância do retângulo.
     */
    private void verificaInvariante() {
        if (getNumeroVertices() != 4 || !sentidoHorario()) {
            System.out.println("Retangulo:iv");
            System.exit(0);
        }

        Ponto a = getVertice(0);
        Ponto b = getVertice(1);
        Ponto c = getVertice(2);
        Ponto d = getVertice(3);

        double ab = a.dist(b);
        double bc = b.dist(c);
        double cd = c.dist(d);
        double da = d.dist(a);
        double ac = a.dist(c);
        double bd = b.dist(d);

        if (Math.abs(ab - cd) >= EPS ||
                Math.abs(bc - da) >= EPS ||
                Math.abs(ac - bd) >= EPS) {
            System.out.println("Retangulo:iv");
            System.exit(0);
        }
    }
}