
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;

public class Scene {

    //definição de um tipo enumerado em java
    public enum Type {NORMAL,WON,FAILED};

    private int id;
    private Type type;
    private String description;
    private ArrayList<Option> options;
	public String getDescription;

    public Scene(int id, String description, Type type)
    {
        this.id = id;
        this.description = description;
        this.type = type;
        this.options = new ArrayList<>();
    }

    public boolean addOption(Option o)
    {
        this.options.add(o);
        return true;
    }
	
    public int getId()
    {
		return id;
    }

    public Type getType()
    {
		return type;
    }

    public String getDescription()
    {
        return description;
    }
    
    public static Scene parseScene(BufferedReader reader) throws IOException, NumberFormatException
    {
        //TODO: implement
    	// you should implement this method on several steps using the methods and
    	// functionalities furnished by the class "BufferedReader" of the object "reader"
    	// that's used as parameter of the method "parseScene".

    	// To check if your implementation is correct or not, you should run class
    	// "SceneTest" as a JUnit test: a) right-click class "SceneTest" b) Run as
    	// JUnit Test d) check which tests fail (red colour) or pass (green colour)
    	// and modify your code to make the tests pass.
    	
    	// this software development approach is based on TDD (test driven development), hence,
    	// you first write unit tests and their results to write the code implementation.

    	// method "parseScene" parses a scene, that is, it takes the text of a scene and
    	// returns an object of type "Scene" where "id" is the identifier of the
    	// scene (a number), type is either "NORMAL", "WON" or "FAILED", "description"
    	// is the description of the scene, and field "options" is the list of next scenes.
    	
    	// STEP 1
    	// creates local variables "description", "sceneId", "numberOfOptions", "sceneType",
    	// and "options", and "opt", with proper types, to hold the values of an object 
    	// of type "Scene".
    	
    	// STEP 2
    	// use the method "readLine" of object "reader" to parse the first line of the buffer "reader";
    	// parse that line and assign the value to "sceneId".
    	
    	// STEP 3
    	// use the method "readLine" of object "reader" to parse the first line of the buffer "reader";
    	// parse that line and assign its integer value to "sceneId".

    	// STEP 3
    	// read a new line to discard string "<<<" of the buffer reader (check the syntax of a scene)
            
    
    	// STEP 4
    	// you will need to read all the lines of the description while the next line is different from
    	// ">>>" ; every time you read a new line of the description you need to concatenate the new
    	// line to all the previous lines; do not forget to add a "\n" at the end of each line. 
    	

    	// STEP 5
    	// the last line you read in the previous step might be "FAILED" or "WON", otherwise the last
    	// line implicitly is "NORMAL"; you must set "sceneType" accordingly.


    	// STEP 6
    	// use the "new" clause to create an object s of type Scene - check the constructor of class
    	// Scene for the right type and order of parameters.
    	
    	
    	// STEP 7
    	// if your scene is "NORMAL" you need to do some extra pre-processing!!
    	// You will need to parse the options (check the structure of the input file)

            	
     // STEP 8
     // once you are done with previous steps, replace the following line with "return s";
            return null;
        }
    }
