/**
 * Responsabilidade: representar um obstáculo geométrico.
 * @author Diogo Carvalho(a90247)
 * @version 23-03-2026
 * @inv A invariante é definida em cada subclasse concreta.
 */
public abstract class Obstaculo {
    /**
     * Responsabilidade: determinar os pontos de interseção entre este obstáculo e um segmento de reta.
     * @param s segmento de reta
     * @return pontos de interseção ou null
     * @pre s != null
     */
    public abstract Ponto[] intersect(SegmentoReta s);
}