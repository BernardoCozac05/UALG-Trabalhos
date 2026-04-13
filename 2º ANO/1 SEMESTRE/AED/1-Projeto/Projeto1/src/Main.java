public class Main {
    public static void main(String[] args) {
        if (args.length == 0) {
            System.out.println("Uso: java StoryMain <ficheiro_da_historia.txt> [--non-interactive]");
            System.exit(1);
        }

        String file = args[0];
        boolean nonInteractive = args.length > 1 && "--non-interactive".equalsIgnoreCase(args[1]);

        try {
            // 1) Carrega a história do ficheiro
            Story story = Story.parseStory(file);

            // 2) Corre a história
            if (nonInteractive) {
                // modo silencioso: lê escolhas do STDIN (se o teu Story suportar)
                story.nonInteractivePlay();
            } else {
                // modo normal com prompts/comandos (PRINT, RESET, SAVE/LOAD se existirem)
                story.interactivePlay();
            }

        } catch (Exception e) {
            System.err.println("Erro a correr a história: " + e.getClass().getSimpleName() +
                               (e.getMessage() == null ? "" : " - " + e.getMessage()));
            e.printStackTrace();
            System.exit(1);
        }
    }
}
