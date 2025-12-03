/*Trabalho Realizado Por:
 * Diogo Carvalho - a90247
 * Diogo Freitas - a90147
 */

package aed.collections;

import java.util.Arrays;
import java.util.Iterator;
import java.util.function.BinaryOperator;
import java.util.function.Consumer;
import java.util.function.UnaryOperator;

public class FintList implements Iterable<Integer> {

    //a ordem dos metodos usada aqui é a ordem recomendada para a sua implementação
    //por exemplo, devem começar por implementar os métodos de add, get e remove mais simples
    public FintList()
    {
       //TODO implementx
    }


    public boolean add(int item)
    {
        //TODO implement
        return true;
    }

    public int get()
    {
        //TODO implement
		return 0;
    }

    public boolean isEmpty()
    {
        //TODO implement
		return false;
    }

    public int size()
    {
        //TODO implement
		return 0;
    }

    public int remove()
    {
		//TODO implement
        return 0;
    }


    public void addAt(int index, int item) 
	{
        //TODO implement
    }

    public int getFirst()
    {
        //TODO implement
		return 0;
    }

    public int get(int index)
    {
        //TODO implement
		return 0;
    }

    public void set(int index, int item)
    {
        //TODO implement
    }

    public int removeAt(int index) {
        //TODO implement
		return 0;
    }

    public int indexOf(int item)
    {
        //TODO implement
		return -1;
    }

    public boolean contains(int item)
    {
        //TODO implement
		return true;
    }

    //este método não precisa de ser eficiente
    public boolean remove(int item)
    {
        //TODO implement
		return false;
    }

    public void reverse()
    {
        //TODO implement
    }

    public FintList deepCopy()
    {
        //TODO implement
    }

   
    public Iterator<Integer> iterator()
    {
        //TODO implement
		return null;
    }

	//a utilizacao de ? super Integer e por causa da implementacao da interface Iterable
    public void forEach(Consumer<? super Integer> c)
    {
        //TODO implement
    }

    public void map(UnaryOperator<Integer> op)
    {
        //TODO implement
    }

    public int reduce(BinaryOperator<Integer> op, int defaultValue)
    {
        //TODO implement
    }
    
    public static void main(String[] args)
    {
    	//TODO implement
    }
}
