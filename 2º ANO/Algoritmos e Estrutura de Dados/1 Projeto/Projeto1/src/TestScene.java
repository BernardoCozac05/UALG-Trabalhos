import java.io.*;

public class TestScene {
    public static void main(String[] args) throws Exception {
        // Cena FINAL (FAILED)
        String fim =
            "[2]\n" +
            "<<<\n" +
            "Num ultimo espasmo de coragem, ou loucura, lanca-se para a\n" +
            "escuridao gotejante da gruta, fugindo no panico cego da criatura\n" +
            "cujo bafo putrido ja aquece a sua nuca como um pressagio do fim.\n" +
            "Mas e inutil. Ela conhece todos os seus recantos como se fosse a\n" +
            "luz negra da sua alma. A gruta e dela. Sempre foi. E agora, tambem\n" +
            "e sua. Garras surgem das trevas e, num unico movimento bruto,\n" +
            "rasgam-no em dois, num terror estupefacto, sem um unico grito. So\n" +
            "visceras.\n" +
            "Morreu.\n" +
            ">>>FAILED\n";
        Scene sFailed = Scene.parseScene(new BufferedReader(new StringReader(fim)));
        System.out.println(sFailed.toString());
        System.out.println();

        // Cena NORMAL (com opções)
        String normal =
            "[0]\n" +
            "<<<\n" +
            "A sua aventura chegou a um cruzamento.\n" +
            ">>>\n" +
            "***\n" +
            "+ 1. Ir para a floresta.\n" +
            "+ 2. Ir para o deserto.\n" +
            "***\n";
        Scene sNormal = Scene.parseScene(new BufferedReader(new StringReader(normal)));
        System.out.println(sNormal.toString());
    }
}
