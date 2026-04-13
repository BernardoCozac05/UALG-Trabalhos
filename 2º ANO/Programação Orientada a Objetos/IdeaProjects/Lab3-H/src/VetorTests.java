import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Vetor.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 */
public class VetorTests {
    private static final double EPS = 1e-9;

    @Test
    public void testConstructorXYAndGetters() {
        Vetor v = new Vetor(3.0, 4.0);
        assertEquals(3.0, v.getX(), EPS);
        assertEquals(4.0, v.getY(), EPS);
    }

    @Test
    public void testConstructorPonto() {
        Vetor v = new Vetor(new Ponto(2.0, -3.0));
        assertEquals(2.0, v.getX(), EPS);
        assertEquals(-3.0, v.getY(), EPS);
    }

    @Test
    public void testModulo() {
        assertEquals(5.0, new Vetor(3.0, 4.0).modulo(), EPS);
    }

    @Test
    public void testProdutoInterno() {
        assertEquals(11.0, new Vetor(1.0, 2.0).produtoInterno(new Vetor(3.0, 4.0)), EPS);
    }

    @Test
    public void testCosineSimilarity() {
        assertEquals(0.0, new Vetor(1.0, 0.0).cosineSimilarity(new Vetor(0.0, 2.0)), EPS);
        assertEquals(1.0, new Vetor(1.0, 1.0).cosineSimilarity(new Vetor(2.0, 2.0)), EPS);
    }

    @Test
    public void testIntersect() {
        SegmentoReta s = new SegmentoReta(new Ponto(1.0, -1.0), new Ponto(1.0, 2.0));
        Ponto p = new Vetor(2.0, 0.0).intersect(s);
        assertNotNull(p);
        assertEquals("(1.00,0.00)", p.toString());
    }

    @Test
    public void testSub() {
        Vetor v = new Vetor(5.0, 3.0).sub(new Vetor(2.0, 1.0));
        assertEquals(3.0, v.getX(), EPS);
        assertEquals(2.0, v.getY(), EPS);
    }

    @Test
    public void testAdd() {
        Vetor v = new Vetor(5.0, 3.0).add(new Vetor(2.0, 1.0));
        assertEquals(7.0, v.getX(), EPS);
        assertEquals(4.0, v.getY(), EPS);
    }

    @Test
    public void testMult() {
        Vetor v = new Vetor(2.0, -3.0).mult(0.5);
        assertEquals(1.0, v.getX(), EPS);
        assertEquals(-1.5, v.getY(), EPS);
    }

    @Test
    public void testToString() {
        assertEquals("[2.00,3.50]", new Vetor(2.0, 3.5).toString());
        assertEquals("[-1.20,4.00]", new Vetor(-1.2, 4.0).toString());
    }
}