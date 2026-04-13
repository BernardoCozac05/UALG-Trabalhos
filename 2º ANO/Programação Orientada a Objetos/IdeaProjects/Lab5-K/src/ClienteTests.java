import static org.junit.jupiter.api.Assertions.*;
import org.junit.jupiter.api.Test;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;

/**
 * Responsabilidade: testar a classe Cliente.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 */
public class ClienteTests {
    private ByteArrayOutputStream setIOstreams(String input) {
        System.setIn(new ByteArrayInputStream(input.getBytes()));
        ByteArrayOutputStream os = new ByteArrayOutputStream();
        PrintStream ps = new PrintStream(os);
        System.setOut(ps);
        return os;
    }

    private String normalize(String s) {
        return s.replace("\r\n", "\n").replace("\r", "\n");
    }

    @Test
    public void testCase1() {
        ByteArrayOutputStream os = setIOstreams("3 2 3 4\n0.2 0.2\n0.4\n");
        Cliente.main();
        assertEquals(normalize("5.00\n[-0.20,0.20]\n"), normalize(os.toString()));
    }

    @Test
    public void testCase2() {
        ByteArrayOutputStream os = setIOstreams("3 4 3 2\n0.2 0.2\n0.4\n");
        Cliente.main();
        assertEquals(normalize("5.00\n[-0.20,-0.60]\n"), normalize(os.toString()));
    }
}