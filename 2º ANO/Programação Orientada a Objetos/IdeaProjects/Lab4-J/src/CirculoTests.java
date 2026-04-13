import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Circulo.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class CirculoTests {
    private static final double EPS = 1e-9;

    @Test
    public void testGetCentro() {
        Circulo c = new Circulo(new Ponto(1.0, 2.0), 3.0);
        assertEquals(1.0, c.getCentro().getX(), EPS);
        assertEquals(2.0, c.getCentro().getY(), EPS);
    }

    @Test
    public void testGetRaio() {
        Circulo c = new Circulo(new Ponto(1.0, 2.0), 3.0);
        assertEquals(3.0, c.getRaio(), EPS);
    }

    @Test
    public void testIntersectTwoPoints() {
        Circulo c = new Circulo(new Ponto(0.0, 0.0), 1.0);
        SegmentoReta s = new SegmentoReta(new Ponto(-2.0, 0.0), new Ponto(2.0, 0.0));

        Ponto[] inter = c.intersect(s);

        assertNotNull(inter);
        assertEquals(2, inter.length);
        assertEquals("(-1.00,0.00)", inter[0].toString());
        assertEquals("(1.00,0.00)", inter[1].toString());
    }

    @Test
    public void testIntersectTangent() {
        Circulo c = new Circulo(new Ponto(0.0, 0.0), 1.0);
        SegmentoReta s = new SegmentoReta(new Ponto(-2.0, 1.0), new Ponto(2.0, 1.0));

        Ponto[] inter = c.intersect(s);

        assertNotNull(inter);
        assertEquals(1, inter.length);
        assertEquals("(0.00,1.00)", inter[0].toString());
    }

    @Test
    public void testIntersectNull() {
        Circulo c = new Circulo(new Ponto(0.0, 0.0), 1.0);
        SegmentoReta s = new SegmentoReta(new Ponto(-2.0, 2.0), new Ponto(2.0, 2.0));

        assertNull(c.intersect(s));
    }
}