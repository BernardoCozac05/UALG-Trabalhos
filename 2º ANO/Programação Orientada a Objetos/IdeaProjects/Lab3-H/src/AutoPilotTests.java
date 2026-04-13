import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;

/**
 * Responsabilidade: testar a classe AutoPilot.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 */
public class AutoPilotTests {
    private static final double EPS = 1e-9;

    @Test
    public void testConstructorAndGetters() {
        Ponto start = new Ponto(2.0, 2.0);
        Ponto finish = new Ponto(2.0, 4.0);
        AutoPilot ap = new AutoPilot(start, finish);

        assertEquals(2.0, ap.getStart().getX(), EPS);
        assertEquals(2.0, ap.getStart().getY(), EPS);
        assertEquals(2.0, ap.getFinish().getX(), EPS);
        assertEquals(4.0, ap.getFinish().getY(), EPS);
    }

    @Test
    public void testSpeedExample1() {
        AutoPilot ap = new AutoPilot(new Ponto(2.0, 2.0), new Ponto(2.0, 4.0));
        Vetor v = ap.speed(new Vetor(1.0, 1.0), 5.0);
        assertEquals("[-0.20,0.20]", v.toString());
    }

    @Test
    public void testTimeExample1() {
        AutoPilot ap = new AutoPilot(new Ponto(2.0, 2.0), new Ponto(2.0, 4.0));
        double t = ap.time(new Vetor(1.0, 1.0), 0.283);
        assertEquals(5.0, t, 0.01);
    }

    @Test
    public void testSpeedExample2() {
        AutoPilot ap = new AutoPilot(new Ponto(2.0, 4.0), new Ponto(2.0, 2.0));
        Vetor v = ap.speed(new Vetor(1.0, 1.0), 5.0);
        assertEquals("[-0.20,-0.60]", v.toString());
    }

    @Test
    public void testTimeExample2() {
        AutoPilot ap = new AutoPilot(new Ponto(2.0, 4.0), new Ponto(2.0, 2.0));
        double t = ap.time(new Vetor(1.0, 1.0), 0.632);
        assertEquals(5.0, t, 0.01);
    }
}