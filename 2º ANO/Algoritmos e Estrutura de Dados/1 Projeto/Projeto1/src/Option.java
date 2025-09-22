public class Option {
    private final int id;
    private final String description;

    public Option(int id, String description) {
        if (id < 0) throw new IllegalArgumentException("id cannot be negative");
        if (description == null || description.trim().isEmpty())
            throw new IllegalArgumentException("description cannot be null or empty");
        this.id = id;
        this.description = description;
    }

    public int getId() { return id; }
    public String getDescription() { return description; }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Option)) return false;
        Option other = (Option) o;
        return id == other.id && description.equals(other.description);
    }

    @Override
    public int hashCode() {
        int r = Integer.hashCode(id);
        r = 31 * r + description.hashCode();
        return r;
    }

    /** Formato textual: "+ <id>. <description>" */
    @Override
    public String toString() {
        return "+ " + id + ". " + description;
    }

    /**
     * Lê uma Option a partir de "+ <id>. <descricao>".
     * - espaços variáveis são aceites;
     * - se a descrição terminar com '.', esse ponto final é removido;
     * - se o id não for inteiro, é lançada NumberFormatException (mensagem padrão).
     */
    public static Option parseOption(String s) {
        if (s == null) throw new NumberFormatException("null string");
        String str = s.trim();

        // Grupo 1: id (qualquer token — parseInt decide se é número)
        // Grupo 2: descrição
        java.util.regex.Pattern p =
            java.util.regex.Pattern.compile("^\\+\\s*(\\S+)\\s*\\.\\s*(.+)\\s*$");
        java.util.regex.Matcher m = p.matcher(str);
        if (!m.matches()) {
            throw new NumberFormatException("Invalid option format or id");
        }

        int parsedId = Integer.parseInt(m.group(1)); // pode lançar NFE
        String desc = m.group(2).trim();

        // Ajuste: remover UM ponto final se estiver no fim da descrição
        if (!desc.isEmpty() && desc.charAt(desc.length() - 1) == '.') {
            desc = desc.substring(0, desc.length() - 1);
        }

        return new Option(parsedId, desc);
    }
}
