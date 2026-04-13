/**
 * Responsabilidade: representar um círculo definido por centro e raio.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv O raio do círculo é maior que zero.
 */
public class Circulo extends Obstaculo {
    private static final double EPS = 1e-9;

    private final Ponto centro;
    private final double raio;

    /**
     * Responsabilidade: criar um círculo a partir do centro e do raio.
     * @param centro centro do círculo
     * @param raio raio do círculo
     * @pre centro != null
     * @post fica criado o círculo
     */
    public Circulo(Ponto centro, double raio) {
        this.centro = centro;
        this.raio = raio;
        verificaInvariante();
    }

    /**
     * Responsabilidade: obter o centro do círculo.
     * @return centro
     */
    public Ponto getCentro() {
        return centro;
    }

    /**
     * Responsabilidade: obter o raio do círculo.
     * @return raio
     */
    public double getRaio() {
        return raio;
    }

    /**
     * Responsabilidade: determinar os pontos de interseção entre o círculo e um segmento de reta.
     * @param s segmento de reta
     * @return pontos de interseção ou null
     * @pre s != null
     */
    @Override
    public Ponto[] intersect(SegmentoReta s) {
        double ax = s.getA().getX();
        double ay = s.getA().getY();
        double bx = s.getB().getX();
        double by = s.getB().getY();

        double dx = bx - ax;
        double dy = by - ay;

        double fx = ax - centro.getX();
        double fy = ay - centro.getY();

        double qa = dx * dx + dy * dy;
        double qb = 2.0 * (fx * dx + fy * dy);
        double qc = fx * fx + fy * fy - raio * raio;

        double delta = qb * qb - 4.0 * qa * qc;

        if (delta < -EPS) {
            return null;
        }

        if (Math.abs(delta) < EPS) {
            double t = -qb / (2.0 * qa);
            if (t >= -EPS && t <= 1.0 + EPS) {
                return new Ponto[] { new Ponto(ax + t * dx, ay + t * dy) };
            }
            return null;
        }

        double raiz = Math.sqrt(delta);
        double t1 = (-qb - raiz) / (2.0 * qa);
        double t2 = (-qb + raiz) / (2.0 * qa);

        Ponto[] pontos = new Ponto[2];
        int n = 0;

        if (t1 >= -EPS && t1 <= 1.0 + EPS) {
            pontos[n] = new Ponto(ax + t1 * dx, ay + t1 * dy);
            n++;
        }

        if (t2 >= -EPS && t2 <= 1.0 + EPS) {
            Ponto p = new Ponto(ax + t2 * dx, ay + t2 * dy);
            boolean repetido = false;

            for (int i = 0; i < n; i++) {
                if (Math.abs(pontos[i].getX() - p.getX()) < EPS &&
                        Math.abs(pontos[i].getY() - p.getY()) < EPS) {
                    repetido = true;
                }
            }

            if (!repetido) {
                pontos[n] = p;
                n++;
            }
        }

        if (n == 0) {
            return null;
        }

        Ponto[] resultado = new Ponto[n];
        for (int i = 0; i < n; i++) {
            resultado[i] = pontos[i];
        }
        return resultado;
    }

    /**
     * Responsabilidade: verificar a invariante de instância do círculo.
     */
    private void verificaInvariante() {
        if (raio <= 0.0) {
            System.out.println("Circulo:iv");
            System.exit(0);
        }
    }
}