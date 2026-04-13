public class Vetor {
    private final double x, y;

    public Vetor(double x, double y) {
        this.x = x;
        this.y = y;

        // Invariante: modulo > 0
        // Como modulo = sqrt(x^2 + y^2), basta verificar x^2 + y^2 > 0
        double sq = x * x + y * y;

        if (!(sq > 0.0)) {
            System.out.println("iv");
            System.exit(0);
        }
    }

    public double produtoInterno(Vetor other) {
        return this.x * other.x + this.y * other.y;
    }
}
