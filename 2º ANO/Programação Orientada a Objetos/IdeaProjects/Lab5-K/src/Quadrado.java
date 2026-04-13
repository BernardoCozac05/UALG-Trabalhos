/**
 * Responsabilidade: representar um quadrado definido por quatro vértices.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv O quadrado tem 4 vértices, está no sentido horário, todos os lados são iguais e as diagonais têm o mesmo comprimento.
 */
public class Quadrado extends Poligono {
    private static final double EPS = 1e-9;

    /**
     * Responsabilidade: criar um quadrado a partir de quatro vértices.
     * @param vertices vértices do quadrado
     * @pre vertices != null
     * @post fica criado o quadrado
     */
    public Quadrado(Ponto[] vertices) {
        super(vertices);
        verificaInvariante();
    }

    /**
     * Responsabilidade: verificar a invariante de instância do quadrado.
     */
    private void verificaInvariante() {
        if (getNumeroVertices() != 4 || !sentidoHorario()) {
            System.out.println("Quadrado:iv");
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

        if (Math.abs(ab - bc) >= EPS ||
                Math.abs(bc - cd) >= EPS ||
                Math.abs(cd - da) >= EPS ||
                Math.abs(ac - bd) >= EPS) {
            System.out.println("Quadrado:iv");
            System.exit(0);
        }
    }
}