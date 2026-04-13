/**
 * Responsabilidade: representar um piloto automático para calcular tempo e velocidade vetorial.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
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
     * @param windSpeed vetor wt do vento
     * @param time tempo da viagem
     * @return vetor velocidade necessária
     */
    public Vetor speed(Vetor windSpeed, double time) {
        Vetor r = new Vetor(finish).sub(new Vetor(start));
        return r.sub(windSpeed).mult(1 / time);
    }

    /**
     * Responsabilidade: calcular o tempo neciessário para efetuar a viagem com velocdade linear dada.
     * @param windSpeed vetor wt do vento
     * @param linearSpeed velocidade linear desejada
     * @return tempo necessário
     */
    public double time(Vetor windSpeed, double linearSpeed) {
        Vetor r = new Vetor(finish).sub(new Vetor(start));
        return r.sub(windSpeed).modulo() / linearSpeed;
    }
}