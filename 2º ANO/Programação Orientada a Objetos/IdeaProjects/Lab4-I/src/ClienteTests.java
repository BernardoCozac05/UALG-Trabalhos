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

    @Test
    public void testCase1() {
        ByteArrayOutputStream os = setIOstreams("0 1 1 1 4 4 4 3\n2 1 2 4\n");
        Cliente.main();
        assertEquals(normalize("6.24\n(2.00,2.00)\n"), normalize(os.toString()));
    }

    @Test
    public void testCase2() {
        ByteArrayOutputStream os = setIOstreams("4 4 6 6 7 7\n2 2 4 2\n");
        Cliente.main();
        assertEquals(normalize("4.24\nnull\n"), normalize(os.toString()));
    }

    @Test
    public void testCase4() {
        ByteArrayOutputStream os = setIOstreams("0 0 2 2 6 2 6 4 11 4\n2 1 9 6\n");
        Cliente.main();
        assertEquals(normalize("13.83\n(3.40,2.00) (6.00,3.86) (6.20,4.00)\n"), normalize(os.toString()));
    }

    private String normalize(String s) {
        return s.replace("\r\n", "\n").replace("\r", "\n");
    }
}