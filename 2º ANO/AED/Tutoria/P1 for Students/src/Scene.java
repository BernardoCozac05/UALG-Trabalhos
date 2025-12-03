import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;

public class Scene {

    //definição de um tipo enumerado em java
    public enum Type {NORMAL,WON,FAILED};

    
    public Scene(int id, String description, Type type)
    {
		//TODO: implement
    }

	
    public boolean addOption(Option o)
    {
		//TODO: implement
		return false;
    }

	
    public int getId()
    {
		//TODO: implement
		return 0;
    }

    public Type getType()
    {
        //TODO: implement
		return Type.NORMAL;
    }

    public String getDescription()
    {
		//TODO: implement
        return null;
    }

    //este método retorna um iterável porque não queremos que o ArrayList seja alterado
    //a partir de fora
    public Iterable<Option> getOptions()
    {
        //TODO: implement
        return null;
    }

    //devolve uma string com a representação textual do conteúdo da cena
    //Não imprime diretamente para o ecrâ
    public String toString()
    {
        //TODO: implement
        return null;
    }

    //NOTA: não precisamos de um método para libertar a cena (como no projeto de LP)
    //porque a JVM vai ficar responsável pela libertação de memória dos objetos do tipo Cena
    //que já não forem usados


    public static Scene parseScene(BufferedReader reader) throws IOException, NumberFormatException
    {
        //TODO: implement
		return null;
    }
}
