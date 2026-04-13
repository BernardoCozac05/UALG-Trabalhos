import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe Ponto.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 */
public class PontoTests {
    private static final double EPS = 1e-9;

    @Test
    public void testConstructorAndGetters() {
        Ponto p = new Ponto(2.5, -1.25);
        assertEquals(2.5, p.getX(), EPS);
        assertEquals(-1.25, p.getY(), EPS);
    }

    @Test
    public void testToString() {
        assertEquals("(2.00,3.50)", new Ponto(2.0, 3.5).toString());
        assertEquals("(-1.20,0.00)", new Ponto(-1.2, 0.0).toString());
    }
}