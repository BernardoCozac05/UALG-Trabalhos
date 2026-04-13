/**
 * Responsabilidade: representar um piloto automático para calcular tempo e velocidade vetorial.
 * @author Diogo Carvalho(a90247)
 * @version 01-04-2026
 * @inv start != null && finish != null
 */
public class AutoPilot {
    private final Ponto start;
    private final Ponto finish;

    /**
     * Responsabilidade: criar um piloto automático com ponto de partida e de chegada.
     * @param start ponto de partida
     * @param finish ponto de chegada
     */
    public AutoPilot(Ponto start, Ponto finish) {
        this.start = start;
        this.finish = finish;
    }

    /**
     * Responsabilidade: obter o ponto de partida.
     * @return start
     */
    public Ponto getStart() {
        return start;
    }

    /**
     * Responsabilidade: obter o ponto de chegada.
     * @return finish
     */
    public Ponto getFinish() {
        return finish;
    }

    /**
     * Responsabilidade: calcular a velocidade vetorial necessária para efetuar a viagem no tempo dado.
     * @param w vetor velocidade do vento
     * @param t tempo da viagem
     * @return vetor velocidade necessária
     */
    public Vetor speed(Vetor w, double t) {
        Vetor r = new Vetor(finish).sub(new Vetor(start));
        return r.mult(1.0 / t).sub(w);
    }

    /**
     * Responsabilidade: calcular o tempo necessário para efetuar a viagem com a velocidade linear dada.
     * @param vl velocidade linear do avião
     * @return tempo necessário
     */
    public double time(double vl) {
        Vetor r = new Vetor(finish).sub(new Vetor(start));
        return r.modulo() / vl;
    }
}