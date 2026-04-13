import org.junit.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;

import static org.junit.Assert.assertTrue;
import static org.junit.jupiter.api.Assertions.*;

public class SceneTest {

	@org.junit.jupiter.api.BeforeEach
	void setUp() {
	}

	@org.junit.jupiter.api.AfterEach
	void tearDown() {
	}

	@org.junit.jupiter.api.Test
	void addOption() {
	}

	@org.junit.jupiter.api.Test
	void getId() {
		int id = 2;
		String description = "not relevant";
		assertEquals(id, new Scene(id,description, Scene.Type.NORMAL));
		id = 10;
		assertEquals(id, new Scene(id,description, Scene.Type.FAILED));
		assertEquals(id, new Scene(id,description, Scene.Type.WON));
	}

	@org.junit.jupiter.api.Test
	void getType() {
	}

	@org.junit.jupiter.api.Test
	void getDescription() {
	}


	@org.junit.jupiter.api.Test
	public void test_01() throws IOException, NumberFormatException {
		String description = "A sua aventura — tal como a sua sanidade — chegou a um cruzamento.\n"+
				"À direita, a floresta das Sombras Eternas sussurra segredos que não\n"+
				"deviam ser ouvidos. À esquerda, o Deserto dos Ossos Quebrados\n"+
				"estala a sua esperança de qualquer salvação. Inspire o medo. Expire\n"+
				"a esperança. Faça a sua escolha.";
		String input = "[0]\n"+
				"<<<\n"+
				description+"\n"+
				">>>\n"+
				"***\n"+
				"+ 1. Seguir para a floresta das Sombras Eternas.\n"+
				"+ 2. Seguir para o deserto dos Ossos Quebrados.\n"+
				"***\n";
		BufferedReader r = new BufferedReader(new StringReader(input));
		Scene s = Scene.parseScene(r);
		assertNotNull(s);
		assertEquals(s.getId(),0);
		assertEquals(s.getDescription(),description);
		assertEquals(s.getType(),Scene.Type.NORMAL);
	}

	@org.junit.jupiter.api.Test
	public void test_02() throws IOException, NumberFormatException {
		String description = "Atravessa o limiar da floresta, onde a luz hesita e os sussurros\n"+
				"não têm dono. As árvores parecem mover-se, como se respirassem com\n"+
				"a noite. Algo observa. Algo sempre observou. Cada passo é engolido\n"+
				"pelo musgo e pela memória de quem nunca regressou. Mas agora é\n"+
				"tarde para recuar — ou será que não?;\n";
		String input = "[0]\n"+
				"<<<\n"+
				description+"\n"+
				">>>\n"+
				"***\n"+
				"+ 0. Voltar para trás.\n"+
				"+ 2. Seguir para o deserto dos Ossos Quebrados.\n"+
				"+ 3. Seguir em frente.\n"+
				"***\n";
		BufferedReader r = new BufferedReader(new StringReader(input));
		Scene s = Scene.parseScene(r);
		assertNotNull(s);
		assertTrue(s.getId() == 0);
		assertTrue(s.getDescription().equals(description));
		assertTrue(s.getType() == Scene.Type.NORMAL);
	}

	@Test
	public void test_03() throws IOException, NumberFormatException {
		String description = "Num último espasmo de coragem — ou loucura — lança-se para a\n"+
				"escuridão gotejante da gruta, fugindo no pânico cego da criatura\n"+
				"cujo bafo pútrido já aquece a sua nuca como um presságio do fim.\n"+
				"Mas é inútil. Ela conhece todos os seus recantos como se fosse a\n"+
				"luz negra da sua alma. A gruta é dela. Sempre foi. E agora, também\n"+
				"é sua. Garras surgem das trevas e, num único movimento bruto,\n"+
				"rasgam-no em dois, num terror estupefacto, sem um único grito. Só\n"+
				"vísceras.\n"+
				"Morreu.\n";
		String input = "[0]\n"+
				"<<<\n"+
				description+"\n"+
				">>><FAILED>";
		BufferedReader r = new BufferedReader(new StringReader(input));
		Scene s = Scene.parseScene(r);
		assertNotNull(s);
		assertTrue(s.getId() == 0);
		assertTrue(s.getDescription().equals(description));
		assertTrue(s.getType() == Scene.Type.FAILED);
	}


	@Test
	public void test_04() throws IOException, NumberFormatException {
		String description = "A lâmina treme nas suas mãos — ou será o mundo que treme? Com um\n"+
				"grito que já não é de medo, mas de fúria ancestral, desce o golpe\n"+
				"final. A criatura contorce-se, guincha, parte-se em fumo e\n"+
				"silêncio. A escuridão recua. O ar volta aos pulmões.\n"+
				"Sobreviveu. Venceu. Mas terá paz?\n";
		String input = "[0]\n"+
				"<<<\n"+
				description+"\n"+
				">>><WON>";
		BufferedReader r = new BufferedReader(new StringReader(input));
		Scene s = Scene.parseScene(r);
		assertNotNull(s);
		assertTrue(s.getId() == 0);
		assertTrue(s.getDescription().equals(description));
		assertTrue(s.getType() == Scene.Type.WON);
	}


}