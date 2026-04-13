import java.io.BufferedReader;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;

/**
 * Lê cenas de um ficheiro (por omissão "scenes.txt") e imprime info de cada uma.
 * - Procura automaticamente por linhas que comecem por '[' (início de cena),
 *   ignorando linhas vazias e comentários começados por '#'.
 * - Compatível com Scene.parse(BufferedReader) OU Scene.parseScene(BufferedReader).
 * - Assume que Scene tem: getId(), getType(), getDescription(), getOptions()
 *   e que getOptions() devolve Iterable<Option>.
 */
public class SceneFileTester {

    /** Procura Scene.parse(...) ou Scene.parseScene(...) via reflection. */
    private static Method findParseMethod() {
        Class<?> cls = Scene.class;
        Class<?>[] sig = new Class<?>[]{BufferedReader.class};

        try {
            Method m = cls.getDeclaredMethod("parse", sig);
            if (!java.lang.reflect.Modifier.isStatic(m.getModifiers()))
                throw new NoSuchMethodException("parse existe mas não é static");
            m.setAccessible(true);
            return m;
        } catch (NoSuchMethodException e) {
            try {
                Method m2 = cls.getDeclaredMethod("parseScene", sig);
                if (!java.lang.reflect.Modifier.isStatic(m2.getModifiers()))
                    throw new NoSuchMethodException("parseScene existe mas não é static");
                m2.setAccessible(true);
                return m2;
            } catch (NoSuchMethodException ex) {
                throw new RuntimeException(
                    "Não encontrei Scene.parse(BufferedReader) nem Scene.parseScene(BufferedReader).", ex);
            }
        }
    }

    /** Invoca o método estático de parse encontrado. */
    private static Scene callParse(Method parseMethod, BufferedReader br) throws Exception {
        try {
            Object ret = parseMethod.invoke(null, br);
            if (ret == null) return null; // EOF (convencional)
            if (!(ret instanceof Scene))
                throw new IllegalStateException("O método de parse não devolveu Scene.");
            return (Scene) ret;
        } catch (InvocationTargetException ite) {
            Throwable cause = ite.getCause();
            if (cause instanceof Exception) throw (Exception) cause;
            throw new RuntimeException(cause);
        }
    }

    /** Avança o reader até à próxima linha que começa por '[' (ignora vazias e linhas '#'). */
    private static boolean positionAtNextSceneStart(BufferedReader br) throws IOException {
        String line;
        while (true) {
            br.mark(1_000_000);
            line = br.readLine();
            if (line == null) return false; // EOF
            String t = line.trim();
            if (t.isEmpty() || t.startsWith("#")) {
                // ignora e continua
                continue;
            }
            if (t.startsWith("[")) {
                // recua para que o parse leia esta linha como 1ª da cena
                br.reset();
                return true;
            }
            // linha lixo: continua à procura do próximo '['
        }
    }

    public static void main(String[] args) {
        String file = (args.length > 0) ? args[0] : "scenes.txt";
        System.out.println("A ler cenas de: " + file);

        Method parseMethod = findParseMethod();

        int total = 0, ok = 0, fail = 0;

        try (BufferedReader br = Files.newBufferedReader(Path.of(file), StandardCharsets.UTF_8)) {

            // posiciona no início da 1ª cena
            if (!positionAtNextSceneStart(br)) {
                System.out.println("Nenhuma cena encontrada.");
                System.out.printf("Resumo: %d cenas processadas | %d OK | %d falharam%n", total, ok, fail);
                return;
            }

            while (true) {
                Scene scn;
                try {
                    scn = callParse(parseMethod, br);
                } catch (IOException ioe) {
                    System.out.println("✘ IOException: " + ioe.getMessage());
                    break; // erro de IO: termina
                } catch (IllegalArgumentException | IllegalStateException e) {
                    fail++;
                    total++;
                    System.out.println("✘ Parse falhou: " + e.getClass().getSimpleName() + " - " +
                            (e.getMessage() == null ? "(sem mensagem)" : e.getMessage()));
                    // tenta continuar para a próxima cena, se existir
                    if (!positionAtNextSceneStart(br)) break;
                    else continue;
                } catch (Exception e) {
                    fail++;
                    total++;
                    System.out.println("✘ Erro inesperado: " + e.getClass().getSimpleName() + " - " +
                            (e.getMessage() == null ? "(sem mensagem)" : e.getMessage()));
                    if (!positionAtNextSceneStart(br)) break;
                    else continue;
                }

                if (scn == null) { // EOF típico
                    break;
                }

                total++;
                ok++;

                // --- Imprime informações da cena ---
                System.out.println("✔ Cena ID " + scn.getId());
                System.out.println("  Tipo: " + scn.getType());
                String desc = scn.getDescription();
                System.out.println("  Descrição (" + (desc == null ? 0 : desc.length()) + " chars):");
                System.out.println("  \"" + (desc == null ? "" : desc) + "\"");

                Iterable<Option> opts = scn.getOptions();
                if (opts == null) {
                    System.out.println("  Opções: 0");
                } else {
                    int count = 0;
                    for (Option o : opts) {
                        if (count == 0) System.out.println("  Opções:");
                        System.out.println("    - [" + o.getId() + "] " + o.getDescription());
                        count++;
                    }
                    if (count == 0) System.out.println("  Opções: 0");
                    else System.out.println("  Opções: " + count);
                }

                System.out.println();

                // posiciona no início da próxima cena, se existir
                if (!positionAtNextSceneStart(br)) break;
            }
        } catch (IOException e) {
            System.err.println("Erro a abrir/ler o ficheiro: " + e.getMessage());
            System.exit(1);
        }

        System.out.printf("Resumo: %d cenas processadas | %d OK | %d falharam%n", total, ok, fail);
    }
}
