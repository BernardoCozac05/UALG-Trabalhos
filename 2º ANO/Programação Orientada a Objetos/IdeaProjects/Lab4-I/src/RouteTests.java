import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Route.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class RouteTests {
    private static final double EPS = 1e-9;

    @Test
    public void testComprimentoExemplo1() {
        Ponto[] pontos = {
                new Ponto(0.0, 1.0),
                new Ponto(1.0, 1.0),
                new Ponto(4.0, 4.0),
                new Ponto(4.0, 3.0)
        };
        Route r = new Route(pontos);

        assertEquals(6.242640687, r.comprimento(), 1e-6);
    }

    @Test
    public void testIntersectExemplo1() {
        Ponto[] pontos = {
                new Ponto(0.0, 1.0),
                new Ponto(1.0, 1.0),
                new Ponto(4.0, 4.0),
                new Ponto(4.0, 3.0)
        };
        Route r = new Route(pontos);
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, 1.0), new Ponto(2.0, 4.0));

        Ponto[] inter = r.intersect(s);

        assertNotNull(inter);
        assertEquals(1, inter.length);
        assertEquals("(2.00,2.00)", inter[0].toString());
    }

    @Test
    public void testIntersectExemplo2() {
        Ponto[] pontos = {
                new Ponto(4.0, 4.0),
                new Ponto(6.0, 6.0),
                new Ponto(7.0, 7.0)
        };
        Route r = new Route(pontos);
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, 2.0), new Ponto(4.0, 2.0));

        assertNull(r.intersect(s));
    }

    @Test
    public void testIntersectExemplo4() {
        Ponto[] pontos = {
                new Ponto(0.0, 0.0),
                new Ponto(2.0, 2.0),
                new Ponto(6.0, 2.0),
                new Ponto(6.0, 4.0),
                new Ponto(11.0, 4.0)
        };
        Route r = new Route(pontos);
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, 1.0), new Ponto(9.0, 6.0));

        Ponto[] inter = r.intersect(s);

        assertNotNull(inter);
        assertEquals(3, inter.length);
        assertEquals("(3.40,2.00)", inter[0].toString());
        assertEquals("(6.00,3.86)", inter[1].toString());
        assertEquals("(6.20,4.00)", inter[2].toString());
    }

    @Test
    public void testIntersectNoDuplicateAtCommonPoint() {
        Ponto[] pontos = {
                new Ponto(0.0, 0.0),
                new Ponto(2.0, 2.0),
                new Ponto(4.0, 0.0)
        };
        Route r = new Route(pontos);
        SegmentoReta s = new SegmentoReta(new Ponto(2.0, -1.0), new Ponto(2.0, 3.0));

        Ponto[] inter = r.intersect(s);

        assertNotNull(inter);
        assertEquals(1, inter.length);
        assertEquals("(2.00,2.00)", inter[0].toString());
    }
}