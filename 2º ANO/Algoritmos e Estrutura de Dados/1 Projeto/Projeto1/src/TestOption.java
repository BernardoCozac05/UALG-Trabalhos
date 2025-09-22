public class TestOption {
    public static void main(String[] args) {
        Option o1 = new Option(7, "Seguir para a floresta das Sombras Eternas.");
        System.out.println("Created: " + o1);

        Option o2 = Option.parseOption("+ 7. Seguir para a floresta das Sombras Eternas.");
        System.out.println("Parsed:  " + o2);

        System.out.println("o1 equals o2? " + o1.equals(o2));

        Option o3 = new Option(8, "Voltar para casa.");
        System.out.println("Different: " + o3);

        try { new Option(-1, "Invalid id"); }
        catch (Exception e) { System.out.println("Caught expected exception: " + e); }

        try { Option.parseOption("+ x. Invalid id here"); }
        catch (Exception e) { System.out.println("Caught expected parse exception: " + e); }
    }
}
