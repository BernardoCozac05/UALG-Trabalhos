import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

public class OptionFileTester {

    public static void main(String[] args) {
        String file = (args.length > 0) ? args[0] : "options.txt";
        int ok = 0, fail = 0, total = 0;

        System.out.println("A ler: " + file);
        try (var br = Files.newBufferedReader(Path.of(file), StandardCharsets.UTF_8)) {
            String line;
            int lineNo = 0;
            while ((line = br.readLine()) != null) {
                lineNo++;
                String trimmed = line.trim();
                // Ignorar vazias e comentários (# ...)
                if (trimmed.isEmpty() || trimmed.startsWith("#")) continue;

                total++;
                try {
                    Option o = Option.parseOption(line);
                    ok++;
                    System.out.printf("L%d ✔  id=%d  desc=\"%s\"%n",
                            lineNo, o.getId(), o.getDescription());
                } catch (Exception e) {
                    fail++;
                    System.out.printf("L%d ✘  %s: %s%n", lineNo,
                            e.getClass().getSimpleName(),
                            e.getMessage() == null ? "(sem mensagem)" : e.getMessage());
                }
            }
        } catch (IOException ioe) {
            System.err.println("Erro a ler o ficheiro: " + ioe.getMessage());
            System.exit(1);
        }

        System.out.printf("%nResumo: %d analisadas | %d OK | %d falharam%n", total, ok, fail);
    }
}
