import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;

public class Scene {
    /** Tipos de cena. */
    public enum Type { NORMAL, WON, FAILED }

    private final int id;
    private final String description;
    private final Type type;
    private final ArrayList<Option> options;

    /**
     * Construtor.
     */
    public Scene(int id, String description, Type type) {
        if (id < 0 || description == null || description.isEmpty() || type == null) {
            throw new IllegalArgumentException("Invalid Scene arguments");
        }
        this.id = id;
        this.description = description;
        this.type = type;
        this.options = new ArrayList<>();
    }

    /** Adiciona opção, não permite ids repetidos. */
    public boolean addOption(Option o) {
        if (o == null) throw new IllegalArgumentException("Null option");
        for (Option ex : options) {
            if (ex.getId() == o.getId()) return false;
        }
        options.add(o);
        return true;
    }

    public int getId() { return id; }
    public Type getType() { return type; }
    public String getDescription() { return description; }
    public Iterable<Option> getOptions() { return options; }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("[").append(id).append("]\n");
        sb.append(description).append("\n");
        if (type == Type.WON) {
            sb.append("<WON>");
        } else if (type == Type.FAILED) {
            sb.append("<FAILED>");
        } else {
            for (int i = 0; i < options.size(); i++) {
                sb.append(options.get(i).toString());
                if (i < options.size() - 1) sb.append("\n");
            }
        }
        return sb.toString();
    }

    /**
     * Lê uma cena do BufferedReader (não fecha o reader).
     * Formatos aceites:
     * [<id>]
     * <<<
     * <descrição (várias linhas)>
     * >>>                (NORMAL)
     * >>> <n> ou >>><n>  (NORMAL; n ignorado aqui)
     * ***                (início opções)
     * <opção 1>
     * ...
     * ***                (fim opções)
     *
     * ou finais:
     * >>>WON / >>> WON / >>><WON>
     * >>>FAILED / >>> FAILED / >>><FAILED>
     */
    public static Scene parseScene(BufferedReader br) throws IOException, NumberFormatException {
        // [<id>]
        String line = br.readLine();
        if (line == null) throw new IOException("Missing scene id line");
        String idLine = line.trim();
        if (!idLine.startsWith("[") || !idLine.endsWith("]") || idLine.length() < 2) {
            throw new NumberFormatException("Invalid scene id format");
        }
        int sceneId = Integer.parseInt(idLine.substring(1, idLine.length() - 1).trim());

        // <<<  (permitir espaços)
        line = br.readLine();
        if (line == null) throw new IOException("Missing <<< marker");
        if (!line.trim().equals("<<<")) throw new IOException("Expected '<<<'");

        // Descrição até um marcador que comece por ">>>"
        StringBuilder desc = new StringBuilder();
        Type sceneType = null;
        while (true) {
            line = br.readLine();
            if (line == null) throw new IOException("Unterminated description");
            String trimmed = line.trim();
            if (trimmed.startsWith(">>>")) {
                // Sufixo após ">>>"
                String suffix = trimmed.substring(3).trim();

                // Se vier entre ângulos, remover <...>
                if (suffix.startsWith("<") && suffix.endsWith(">") && suffix.length() >= 2) {
                    suffix = suffix.substring(1, suffix.length() - 1).trim();
                }

                if (suffix.isEmpty()) {
                    sceneType = Type.NORMAL;
                } else {
                    String up = suffix.toUpperCase();
                    if (up.equals("WON")) {
                        sceneType = Type.WON;
                    } else if (up.equals("FAILED")) {
                        sceneType = Type.FAILED;
                    } else {
                        // Se for um número (ex: >>><2> ou >>> 3), tratar como NORMAL
                        boolean allDigits = true;
                        for (int i = 0; i < suffix.length(); i++) {
                            if (!Character.isDigit(suffix.charAt(i))) { allDigits = false; break; }
                        }
                        if (allDigits) {
                            sceneType = Type.NORMAL; // número de opções é irrelevante no B
                        } else {
                            throw new IOException("Invalid end-of-description marker: " + line);
                        }
                    }
                }
                break; // fim da descrição
            } else {
                // preservar linha tal como está
                desc.append(line).append("\n");
            }
        }

        // remover último '\n' extra
        if (desc.length() > 0 && desc.charAt(desc.length() - 1) == '\n') {
            desc.deleteCharAt(desc.length() - 1);
        }

        Scene scene = new Scene(sceneId, desc.toString(), sceneType);

        // Para NORMAL, ler bloco de opções entre *** ... ***
        if (sceneType == Type.NORMAL) {
            // *** (início)
            line = br.readLine();
            if (line == null) throw new IOException("Missing options start marker");
            if (!line.trim().equals("***")) throw new IOException("Expected '***' (start options)");

            // opções até *** (fim)
            while (true) {
                line = br.readLine();
                if (line == null) throw new IOException("Missing options end marker");
                String trimmed = line.trim();
                if (trimmed.equals("***")) break;
                // cada linha é uma opção no formato "+ <id>. <descrição>"
                scene.addOption(Option.parseOption(line)); // pode lançar NumberFormatException
            }
        }

        return scene;
    }
}
