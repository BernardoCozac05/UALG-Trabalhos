import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe SegmentoReta.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 */
public class SegmentoRetaTests {
    private static final double EPS = 1e-9;

    @Test
    public void testConstructorPontoVetorAndGetters() {
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, 2.0), new Vetor(-1.0, 0.0));
        assertEquals(2.0, s.getA().getX(), EPS);
        assertEquals(2.0, s.getA().getY(), EPS);
        assertEquals(1.0, s.getB().getX(), EPS);
        assertEquals(2.0, s.getB().getY(), EPS);
    }

    @Test
    public void testConstructorPontoPontoAndGetters() {
        SegmentoReta s = new SegmentoReta(new Ponto(1.0, 1.0), new Ponto(3.0, 4.0));
        assertEquals(1.0, s.getA().getX(), EPS);
        assertEquals(1.0, s.getA().getY(), EPS);
        assertEquals(3.0, s.getB().getX(), EPS);
        assertEquals(4.0, s.getB().getY(), EPS);
    }

    @Test
    public void testIntersectWithIntersection() {
        SegmentoReta s = new SegmentoReta(new Ponto(1.0, -1.0), new Ponto(1.0, 2.0));
        Ponto p = s.intersect(new Vetor(2.0, 0.0));
        assertNotNull(p);
        assertEquals("(1.00,0.00)", p.toString());
    }

    @Test
    public void testIntersectWithoutIntersection() {
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, 2.0), new Ponto(3.0, 3.0));
        assertNull(s.intersect(new Vetor(1.0, 0.0)));
    }

    @Test
    public void testToString() {
        assertEquals("sr((1.00,2.00); (2.00,2.00))",
                new SegmentoReta(new Ponto(2.0, 2.0), new Vetor(-1.0, 0.0)).toString());
        assertEquals("sr((3.00,3.00); (5.00,7.50))",
                new SegmentoReta(new Ponto(3.0, 3.0), new Vetor(2.0, 4.5)).toString());
    }
}