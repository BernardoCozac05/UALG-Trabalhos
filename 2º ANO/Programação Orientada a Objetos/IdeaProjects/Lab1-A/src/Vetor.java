public class Vetor {
    private final double x, y;

    public Vetor(double x, double y) {
        this.x = x;
        this.y = y;
    }

    public double modulo() {
        return Math.sqrt(x * x + y * y);
    }
}
