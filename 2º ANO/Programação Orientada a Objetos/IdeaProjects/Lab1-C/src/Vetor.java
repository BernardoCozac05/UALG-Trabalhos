public class Vetor {
    private final double x, y;

    public Vetor(double x, double y) {
        this.x = x;
        this.y = y;

        // Invariante: ||v|| > 0  <=>  x^2 + y^2 > 0
        double sq = x * x + y * y;
        if (!(sq > 0.0)) {
            System.out.println("iv");
            System.exit(0);
        }
    }

    public double modulo() {
        return Math.sqrt(x * x + y * y);
    }

    public double produtoInterno(Vetor other) {
        return this.x * other.x + this.y * other.y;
    }

    public double cosineSimilarity(Vetor other) {
        // Sc(u,v) = (u·v) / (||u||*||v||)
        return this.produtoInterno(other) / (this.modulo() * other.modulo());
    }
}
