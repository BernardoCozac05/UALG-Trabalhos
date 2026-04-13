import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Triangulo.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class TrianguloTests {
    @Test
    public void testIntersectNull() {
        Ponto[] v = {
                new Ponto(0.0, 0.0),
                new Ponto(1.0, 1.0),
                new Ponto(4.0, 1.0)
        };
        Triangulo t = new Triangulo(v);
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, 2.0), new Ponto(5.0, 2.0));

        assertNull(t.intersect(s));
    }

    @Test
    public void testIntersect() {
        Ponto[] v = {
                new Ponto(0.0, 0.0),
                new Ponto(0.0, 4.0),
                new Ponto(4.0, 0.0)
        };
        Triangulo t = new Triangulo(v);
        SegmentoReta s = new SegmentoReta(new Ponto(-1.0, 1.0), new Ponto(3.0, 1.0));

        Ponto[] inter = t.intersect(s);

        assertNotNull(inter);
        assertEquals(2, inter.length);
        assertEquals("(0.00,1.00)", inter[0].toString());
        assertEquals("(3.00,1.00)", inter[1].toString());
    }
}