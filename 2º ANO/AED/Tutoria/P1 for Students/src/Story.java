import java.io.*;

public class Story {

    //definicao de constantes em java são feitas assim
    //Como convenção costuma usar-se letras maiúsculas como nome de variáveis que são constantes
    //para que seja fácil perceber no código quando se usa uma constante
    //a keyword final indica que depois de definido, o valor não pode ser mais alterado
    private static final String WAITING = "WAITING";
    private static final String PROMPT = "Select your option: ";
    private static final String RESET_CMD = "RESET";
    private static final String SAVE_CMD = "SAVE";
    private static final String LOAD_CMD = "LOAD";
    private static final String PRINT_CMD = "PRINT";

    private static final String FILE_ERROR = "Error: invalid file or error opening file!";
    private static final String NUMBER_FORMAT_ERROR = "Error: invalid number format when parsing!";
    private static final String STDIN_ERROR = "Error: cannot access or read from standard input";

    private static final String INVALID_OPTION = "Invalid option. Such option does not exist.";
    private static final String INVALID_NUMBER = "Invalid option. Please enter a number of an option, or one of the commands (PRINT,RESET,LOAD,SAVE).";
    private static final String INVALID_LOAD_STORY_NAME = "The loaded story name does not match. Loading aborted.";
    private static final String INVALID_LOAD_SCENE = "The scene in the loaded file is not valid. Loading aborted.";

    private static final String LOAD_SUCCESS = "Story state loaded successfully.";
    private static final String SAVE_SUCCESS = "Story state saved successfully.";


    public Story(String title, Node<Scene>[] nodes)
    {
        //TODO: implement
    }

    public String getTitle()
    {
        //TODO: implement
		return null;
    }

    public Node<Scene> getCurrent()
    {
        //TODO: implement
		return null;
    }

    public void reset()
    {
        //TODO: implement
    }

    public boolean saveStoryState(String fileName)
    {
        //TODO: implement
		return false;
    }

    public boolean loadStoryState(String fileName)
    {
        //TODO: implement
		return false;
    }

    public void nonInteractivePlay()
    {
        //TODO: implement
    }

    public void interactivePlay()
    {
        //TODO: implement
    }

    public static Story parseStory(String filename)
    {
        //TODO: implement
		return null;
    }
}
