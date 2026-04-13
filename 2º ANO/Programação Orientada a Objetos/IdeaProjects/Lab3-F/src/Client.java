import java.util.Scanner;

/**
 * Responsabilidade: ler os dados de entrada, executar a operação pedida e escrever o resultado.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 * @see POO25-26_Lab3-JUnit.pdf
 */
public class Client {
    static final String TOTIME = "asTime";
    static final String TOSECS = "asSeconds";
    static final String ADDTIME = "add";

    /**
     * Responsabilidade: executar o programa principal.
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public static void main() {
        Scanner sc = new Scanner(System.in);
        String aString = sc.nextLine();
        String operator = sc.nextLine();
        String output="";
        T2time receiver;

        switch (operator) {
            case TOTIME:
                receiver = new T2time(Integer.parseInt(aString));
                output = receiver.toString();
                break;

            case TOSECS:
                receiver = new T2time(aString);
                output = "" + receiver.asSeconds();
                break;

            case ADDTIME:
                T2time argument;
                receiver = new T2time(aString);
                String bString = sc.nextLine();
                if (T2time.isTime(bString))
                    argument = new T2time(bString);
                else
                    argument = new T2time(Integer.parseInt(bString));
                output = receiver.add(argument).toString();
                break;

            default:
                //Seems right to have a default statement .
        }

        IO.println(output);
    }
}