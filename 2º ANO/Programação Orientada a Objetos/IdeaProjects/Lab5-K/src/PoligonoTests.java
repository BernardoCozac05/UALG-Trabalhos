import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Poligono.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class PoligonoTests {
    private static final double EPS = 1e-9;

    @Test
    public void testGetNumeroVertices() {
        Ponto[] v = {
                new Ponto(0.0, 0.0),
                new Ponto(0.0, 2.0),
                new Ponto(2.0, 2.0),
                new Ponto(2.0, 0.0)
        };
        Poligono p = new Poligono(v);
        assertEquals(4, p.getNumeroVertices());
    }

    @Test
    public void testGetVertice() {
        Ponto[] v = {
                new Ponto(0.0, 0.0),
                new Ponto(0.0, 2.0),
                new Ponto(2.0, 2.0)
        };
        Poligono p = new Poligono(v);
        assertEquals(0.0, p.getVertice(1).getX(), EPS);
        assertEquals(2.0, p.getVertice(1).getY(), EPS);
    }

    @Test
    public void testIntersect() {
        Ponto[] v = {
                new Ponto(1.0, 0.0),
                new Ponto(1.0, 3.0),
                new Ponto(4.0, 3.0),
                new Ponto(4.0, 0.0)
        };
        Poligono p = new Poligono(v);
        SegmentoReta s = new SegmentoReta(new Ponto(0.0, 0.0), new Ponto(2.0, 2.0));

        Ponto[] inter = p.intersect(s);

        assertNotNull(inter);
        assertEquals(1, inter.length);
        assertEquals("(1.00,1.00)", inter[0].toString());
    }

    @Test
    public void testIntersectNull() {
        Ponto[] v = {
                new Ponto(1.0, 0.0),
                new Ponto(1.0, 3.0),
                new Ponto(4.0, 3.0),
                new Ponto(4.0, 0.0)
        };
        Poligono p = new Poligono(v);
        SegmentoReta s = new SegmentoReta(new Ponto(-2.0, 0.0), new Ponto(-1.0, 0.0));

        assertNull(p.intersect(s));
    }
}