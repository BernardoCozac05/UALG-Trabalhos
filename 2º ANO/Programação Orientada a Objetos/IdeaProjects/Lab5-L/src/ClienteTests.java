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
        ByteArrayOutputStream os = setIOstreams("5 2 3\n2 3\n0.5\n2\n");
        Cliente.main();
        assertEquals(normalize("Rota:iv\n"), normalize(os.toString()));
    }

    @Test
    public void testCase2() {
        ByteArrayOutputStream os = setIOstreams("5 1 5 5 7 5\n1 1\n2\n2.25\n");
        Cliente.main();
        assertEquals(normalize("6.00\n3.00\n(5.50,5.00)\n[-1.00,1.00] [1.00,-1.00]\n"),
                normalize(os.toString()));
    }
}