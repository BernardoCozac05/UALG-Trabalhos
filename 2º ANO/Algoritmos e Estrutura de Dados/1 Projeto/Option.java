// Option.java
// TAI Option — AED 2025/26 Projeto 1 – Problema A

public class Option {

    private final int id;
    private final String description;

    /**
     * Construtor que cria uma opção com a informação recebida.
     * @param id identificador da opção (não pode ser negativo)
     * @param description descrição textual (não pode ser nula nem vazia)
     * @throws IllegalArgumentException se id < 0 ou descrição inválida
     */
    public Option(int id, String description) {
        if (id < 0) {
            throw new IllegalArgumentException("id cannot be negative");
        }
        if (description == null || description.trim().isEmpty()) {
            throw new IllegalArgumentException("description cannot be null or empty");
        }
        this.id = id;
        this.description = description;
    }

    /**
     * Retorna o identificador da opção.
     */
    public int getId() {
        return id;
    }

    /**
     * Retorna a descrição textual da opção.
     */
    public String getDescription() {
        return description;
    }

    /**
     * Duas opções são iguais se tiverem o mesmo id e a mesma descrição.
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Option)) return false;
        Option other = (Option) o;
        return this.id == other.id && this.description.equals(other.description);
    }

    @Override
    public int hashCode() {
        int result = Integer.hashCode(id);
        result = 31 * result + description.hashCode();
        return result;
    }

    /**
     * Formato textual: "+ <id>. <description>"
     * Ex.: "+ 7. Seguir para a floresta das Sombras Eternas."
     */
    @Override
    public String toString() {
        return "+ " + id + ". " + description;
    }

    /**
     * Lê uma Option a partir de uma string no formato do toString().
     * @param s string da opção (ex.: "+ 7. Alguma descrição")
     * @return objeto Option
     * @throws NumberFormatException se não for possível extrair um inteiro para o id
     * @throws IllegalArgumentException se a descrição extraída for inválida (delegada no construtor)
     */
    public static Option parseOption(String s) {
        if (s == null) {
            throw new NumberFormatException("null string");
        }
        String str = s.trim();

        // Aceita espaços variáveis: "+  <id>  .  <desc>"
        // Regex captura: id (grupo 1) e descrição (grupo 2)
        java.util.regex.Pattern p = java.util.regex.Pattern.compile("^\\+\\s*(\\d+)\\s*\\.\\s*(.+)\\s*$");
        java.util.regex.Matcher m = p.matcher(str);

        if (!m.matches()) {
            // Incapaz de extrair id conforme especificação → NumberFormatException
            throw new NumberFormatException("Invalid option format or id");
        }

        int parsedId;
        try {
            parsedId = Integer.parseInt(m.group(1));
        } catch (NumberFormatException e) {
            // Encaminha exatamente como NumberFormatException quando o id não é um número válido
            throw e;
        }

        String desc = m.group(2);
        return new Option(parsedId, desc);
    }
}
