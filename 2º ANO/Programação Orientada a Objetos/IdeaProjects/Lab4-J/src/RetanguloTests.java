import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Retangulo.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class RetanguloTests {
    @Test
    public void testIntersect() {
        Ponto[] v = {
                new Ponto(1.0, 0.0),
                new Ponto(1.0, 3.0),
                new Ponto(4.0, 3.0),
                new Ponto(4.0, 0.0)
        };
        Retangulo r = new Retangulo(v);
        SegmentoReta s = new SegmentoReta(new Ponto(0.0, 0.0), new Ponto(2.0, 2.0));

        Ponto[] inter = r.intersect(s);

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
        Retangulo r = new Retangulo(v);
        SegmentoReta s = new SegmentoReta(new Ponto(-3.0, -1.0), new Ponto(-1.0, -1.0));

        assertNull(r.intersect(s));
    }
}