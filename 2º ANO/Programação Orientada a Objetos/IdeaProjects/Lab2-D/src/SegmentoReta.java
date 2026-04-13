/**
 * Responsabilidade: representar um segmento de reta definido por um extremo e um vetor posição.
 * @author Diogo Carvalho(a90247)
 * @version 19-02-2026
 * @inv Os extremos do segmento não coincidem.
 */
public class SegmentoReta {
    private static final double EPS = 1e-9;

    private final Ponto a, b;

    /**
     * Responsabilidade: criar um segmento de reta a partir de um extremo e de um vetor posição.
     * @param a extremo do segmento
     * @param v vetor posição
     */
    public SegmentoReta(Ponto a, Vetor v) {
        this.a = a;
        this.b = new Ponto(a.getX() + v.getX(), a.getY() + v.getY());
        verificaInvariante();
    }

    /**
     * Responsabilidade: obter o extremo A do segmento.
     * @return A
     */
    public Ponto getA() {
        return a;
    }

    /**
     * Responsabilidade: obter o extremo B do segmento.
     * @return B
     */
    public Ponto getB() {
        return b;
    }

    /**
     * Responsabilidade: representar o segmento de reta como texto no formato pedido,
     * colocando primeiro o extremo mais próximo da origem.
     * @return string no formato sr((xA,yA); (xB,yB))
     */
    @Override
    public String toString() {
        double da2 = a.getX() * a.getX() + a.getY() * a.getY();
        double db2 = b.getX() * b.getX() + b.getY() * b.getY();

        if (db2 + EPS < da2) {
            return "sr(" + b.toString() + "; " + a.toString() + ")";
        }
        return "sr(" + a.toString() + "; " + b.toString() + ")";
    }

    /**
     * Responsabilidade: verificar a invariante de instância do segmento de reta.
     */
    private void verificaInvariante() {
        double dx = a.getX() - b.getX();
        double dy = a.getY() - b.getY();
        if ((dx * dx + dy * dy) <= EPS * EPS) {
            System.out.println("SegmentoReta:iv");
            System.exit(0);
        }
    }
}