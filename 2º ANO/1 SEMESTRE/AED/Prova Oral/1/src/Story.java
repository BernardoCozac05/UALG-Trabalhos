import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;

public class Story {
    public static final String FILE_ERROR = "Error: invalid file or error opening file!";
    public static final String NUMBER_FORMAT_ERROR = "Error: invalid number format when parsing!";
    public static final String SAVE_SUCCESS = "Story state saved successfully.";
    public static final String LOAD_SUCCESS = "Story state loaded successfully.";
    public static final String INVALID_LOAD_STORY_NAME = "The loaded story name does not match. Loading aborted.";
    public static final String INVALID_LOAD_SCENE = "The scene in the loaded file is not valid. Loading aborted.";
    public static final String INVALID_NUMBER = "Invalid option. Please enter a number of an option, or one of the commands (PRINT,RESET,LOAD,SAVE).";
    public static final String INVALID_OPTION = "Invalid option. Such option does not exist.";

    private final String title;
    private final Node<Scene>[] nodes;
    private final Map<Integer, Node<Scene>> idToNode;
    private Node<Scene> current;

    private static final BufferedReader inputReader = new BufferedReader(new InputStreamReader(System.in));

    public Story(String title, Node<Scene>[] nodes) {
        if (title == null || title.isEmpty() || nodes == null || nodes.length == 0) {
            throw new IllegalArgumentException("Invalid Story arguments");
        }
        this.title = title;
        this.nodes = nodes;
        this.current = nodes[0];
        this.idToNode = new HashMap<>();
        for (Node<Scene> n : nodes) {
            Scene s = nodeScene(n);
            if (s != null) {
                idToNode.put(s.getId(), n);
            }
        }
    }

    public String getTitle() { return title; }
    public Node<Scene> getCurrent() { return current; }
    public void reset() { current = nodes[0]; }

    public static Story parseStory(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String title = br.readLine();
            if (title == null) {
                System.out.println(FILE_ERROR);
                return null;
            }
            String nLine = br.readLine();
            if (nLine == null) {
                System.out.println(NUMBER_FORMAT_ERROR);
                return null;
            }
            int n = Integer.parseInt(nLine.trim());
            if (n <= 0) {
                System.out.println(NUMBER_FORMAT_ERROR);
                return null;
            }
            @SuppressWarnings("unchecked")
            Node<Scene>[] nodes = (Node<Scene>[]) new Node<?>[n];
            for (int i = 0; i < n; i++) {
                Scene s = Scene.parseScene(br);
                nodes[i] = new Node<Scene>(s);
            }
            Map<Integer, Node<Scene>> idMap = new HashMap<>();
            for (Node<Scene> node : nodes) {
                Scene sc = nodeScene(node);
                if (sc != null) {
                    idMap.put(sc.getId(), node);
                }
            }
            for (Node<Scene> node : nodes) {
                Scene sc = nodeScene(node);
                if (sc != null && sc.getType() == Scene.Type.NORMAL) {
                    for (Option o : sc.getOptions()) {
                        Node<Scene> dest = idMap.get(o.getId());
                        if (dest != null) {
                            tryAddNeighbour(node, dest);
                        }
                    }
                }
            }
            return new Story(title, nodes);
        } catch (NumberFormatException e) {
            System.out.println(NUMBER_FORMAT_ERROR);
            return null;
        } catch (IOException e) {
            System.out.println(FILE_ERROR);
            return null;
        }
    }

    public void nonInteractivePlay() {
        try {
            reset();
            String line;
            while (nodeScene(current).getType() == Scene.Type.NORMAL && (line = inputReader.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty()) {
                    continue;
                }
                int choice;
                try {
                    choice = Integer.parseInt(line);
                } catch (NumberFormatException e) {
                    System.out.println("WAITING");
                    return;
                }
                Node<Scene> next = findNextByOption(nodeScene(current), choice);
                if (next == null) {
                    System.out.println("WAITING");
                    return;
                }
                current = next;
            }
            Scene.Type t = nodeScene(current).getType();
            if (t == Scene.Type.WON) {
                System.out.println("WON");
            } else if (t == Scene.Type.FAILED) {
                System.out.println("FAILED");
            } else {
                System.out.println("WAITING");
            }
        } catch (IOException e) {
            System.out.println("WAITING");
        }
    }

    public boolean saveStoryState(String fileName) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(fileName))) {
            bw.write(title);
            bw.newLine();
            bw.write(Integer.toString(nodeScene(current).getId()));
            bw.newLine();
            System.out.println(SAVE_SUCCESS);
            return true;
        } catch (IOException e) {
            System.out.println(FILE_ERROR);
            return false;
        }
    }

    public boolean loadStoryState(String fileName) {
        try (BufferedReader br = new BufferedReader(new FileReader(fileName))) {
            String fileTitle = br.readLine();
            if (fileTitle == null) {
                System.out.println(FILE_ERROR);
                return false;
            }
            String idLine = br.readLine();
            if (idLine == null) {
                System.out.println(NUMBER_FORMAT_ERROR);
                return false;
            }
            int targetId;
            try {
                targetId = Integer.parseInt(idLine.trim());
            } catch (NumberFormatException e) {
                System.out.println(NUMBER_FORMAT_ERROR);
                return false;
            }
            if (!this.title.equals(fileTitle)) {
                System.out.println(INVALID_LOAD_STORY_NAME);
                return false;
            }
            Node<Scene> node = idToNode.get(targetId);
            if (node == null) {
                System.out.println(INVALID_LOAD_SCENE);
                return false;
            }
            this.current = node;
            System.out.println(LOAD_SUCCESS);
            return true;
        } catch (IOException e) {
            System.out.println(FILE_ERROR);
            return false;
        }
    }

    public void interactivePlay() {
        System.out.println(title);

        // Imprime a cena inicial
        Scene currentScene = nodeScene(current);
        System.out.println(currentScene.toString());
        if (currentScene.getType() == Scene.Type.WON || currentScene.getType() == Scene.Type.FAILED) {
            return;
        }

        while (true) {
            System.out.print("Select your option: ");

            String line;
            try {
                line = inputReader.readLine();
            } catch (IOException e) {
                // O teste espera esta mensagem na mesma linha do prompt
                System.out.println("Error: cannot access or read from standard input");
                return;
            }
            if (line == null) {
                // EOF também deve imprimir a mesma mensagem
                System.out.println("Error: cannot access or read from standard input");
                return;
            }

            line = line.trim();
            if (line.isEmpty()) {
                System.out.println(INVALID_NUMBER);
                continue;
            }

            // Comandos
            if (line.equalsIgnoreCase("RESET")) {
                reset();
                System.out.println(nodeScene(current).toString());
                if (nodeScene(current).getType() == Scene.Type.WON || nodeScene(current).getType() == Scene.Type.FAILED) {
                    return;
                }
                continue;
            }
            if (line.equalsIgnoreCase("PRINT")) {
                System.out.println(nodeScene(current).toString());
                continue;
            }
            if (line.toUpperCase().startsWith("SAVE")) {
                String[] parts = line.split("\\s+", 2);
                if (parts.length < 2 || parts[1].trim().isEmpty()) {
                    System.out.println(FILE_ERROR);
                } else {
                    saveStoryState(parts[1].trim());
                }
                continue;
            }
            if (line.toUpperCase().startsWith("LOAD")) {
                String[] parts = line.split("\\s+", 2);
                if (parts.length < 2 || parts[1].trim().isEmpty()) {
                    System.out.println(FILE_ERROR);
                } else {
                    boolean loaded = loadStoryState(parts[1].trim());
                    if (loaded) {
                        System.out.println(nodeScene(current).toString());
                        Scene.Type loadedType = nodeScene(current).getType();
                        if (loadedType == Scene.Type.WON || loadedType == Scene.Type.FAILED) {
                            return;
                        }
                    }
                }
                continue;
            }

            // Escolha numérica
            int choice;
            try {
                choice = Integer.parseInt(line);
            } catch (NumberFormatException e) {
                System.out.println(INVALID_NUMBER);
                continue;
            }

            Node<Scene> next = findNextByOption(nodeScene(current), choice);
            if (next == null) {
                System.out.println(INVALID_OPTION);
                continue;
            }

            current = next;
            System.out.println(nodeScene(current).toString());
            Scene.Type typeNow = nodeScene(current).getType();
            if (typeNow == Scene.Type.WON || typeNow == Scene.Type.FAILED) {
                return;
            }
        }
    }


    /* ============================== Helpers ============================== */

    private static Scene nodeScene(Node<Scene> n) {
        if (n == null) return null;
        try {
            return (Scene) n.getClass().getMethod("getValue").invoke(n);
        } catch (Exception e) {
            try {
                return tryGetSceneField(n);
            } catch (Exception ex) {
                for (java.lang.reflect.Field f : n.getClass().getDeclaredFields()) {
                    f.setAccessible(true);
                    try {
                        Object v = f.get(n);
                        if (v instanceof Scene) {
                            return (Scene) v;
                        }
                    } catch (IllegalAccessException ignore) {}
                }
            }
        }
        throw new IllegalStateException("Cannot access Scene value from Node");
    }

    private static Scene tryGetSceneField(Node<Scene> n) throws IllegalAccessException, NoSuchFieldException {
        String[] fieldNames = {"value", "element", "scene", "data", "obj"};
        for (String fn : fieldNames) {
            try {
                java.lang.reflect.Field f = n.getClass().getDeclaredField(fn);
                f.setAccessible(true);
                Object v = f.get(n);
                if (v instanceof Scene) {
                    return (Scene) v;
                }
            } catch (NoSuchFieldException e) {
                continue;
            }
        }
        throw new NoSuchFieldException();
    }

    private static void tryAddNeighbour(Node<Scene> a, Node<Scene> b) {
        String[] names = {"addNeighbour", "addNeighbor"};
        for (String nm : names) {
            try {
                a.getClass().getMethod(nm, a.getClass()).invoke(a, b);
                return;
            } catch (Exception ignore) { }
        }
    }

    private Node<Scene> findNextByOption(Scene scene, int optionId) {
        for (Option o : scene.getOptions()) {
            if (o.getId() == optionId) {
                return idToNode.get(optionId);
            }
        }
        return null;
    }
}