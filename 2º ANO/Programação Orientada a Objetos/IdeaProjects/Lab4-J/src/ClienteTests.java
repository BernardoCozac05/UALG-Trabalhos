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
    public void testCase6() {
        ByteArrayOutputStream os = setIOstreams("0 0 2 2 2 6\nS 1 0 1 3 4 3 4 0\n");
        Cliente.main();
        assertEquals(normalize("(1.00,1.00) (2.00,3.00)\n"), normalize(os.toString()));
    }

    @Test
    public void testCase7() {
        ByteArrayOutputStream os = setIOstreams("2 2 3 4 5 2\nT 0 0 1 1 4 1\n");
        Cliente.main();
        assertEquals(normalize("null\n"), normalize(os.toString()));
    }
}