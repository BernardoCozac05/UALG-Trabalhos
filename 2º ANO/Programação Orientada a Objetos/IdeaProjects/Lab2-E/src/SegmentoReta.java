/**
 * Responsabilidade: representar um segmento de reta definido por dois extremos.
 * @author Diogo Carvalho(a90247)
 * @version 19-02-2026
 * @inv Os extremos do segmento não coincidem.
 */
public class SegmentoReta {
    private static final double EPS = 1e-9;

    private final Ponto a;
    private final Ponto b;

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
     * Responsabilidade: criar um segmento de reta a partir de dois pontos.
     * @param a primeiro extremo
     * @param b segundo extremo
     */
    public SegmentoReta(Ponto a, Ponto b) {
        this.a = a;
        this.b = b;
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
     * Responsabilidade: obter o ponto de interseção entre este SegmentoReta e um Vetor.
     * O Vetor é considerado como o segmento entre a origem (0,0) e o ponto (xv,yv).
     * @param v vetor
     * @return ponto de interseção ou null
     */
    public Ponto intersect(Vetor v) {
        double ax = a.getX();
        double ay = a.getY();
        double bx = b.getX();
        double by = b.getY();

        double cx = 0.0;
        double cy = 0.0;
        double dx = v.getX();
        double dy = v.getY();

        double rX = bx - ax;
        double rY = by - ay;
        double sX = dx - cx;
        double sY = dy - cy;

        double rxs = rX * sY - rY * sX;
        double qpx = cx - ax;
        double qpy = cy - ay;
        double qpxr = qpx * rY - qpy * rX;

        if (Math.abs(rxs) <= EPS) {
            if (Math.abs(qpxr) > EPS) {
                return null;
            }

            double rr = rX * rX + rY * rY;
            if (rr <= EPS * EPS) {
                return null;
            }

            double t0 = (qpx * rX + qpy * rY) / rr;
            double t1 = t0 + (sX * rX + sY * rY) / rr;

            double tmin = Math.min(t0, t1);
            double tmax = Math.max(t0, t1);

            double lo = Math.max(0.0, tmin);
            double hi = Math.min(1.0, tmax);

            if (hi + EPS < lo) {
                return null;
            }

            double t = lo;
            return new Ponto(ax + t * rX, ay + t * rY);
        }

        double t = (qpx * sY - qpy * sX) / rxs;
        double u = (qpx * rY - qpy * rX) / rxs;

        if (t < -EPS || t > 1.0 + EPS || u < -EPS || u > 1.0 + EPS) {
            return null;
        }

        return new Ponto(ax + t * rX, ay + t * rY);
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