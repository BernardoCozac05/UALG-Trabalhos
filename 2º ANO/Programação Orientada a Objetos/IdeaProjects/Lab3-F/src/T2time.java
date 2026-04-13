/**
 * Responsabilidade: representar um instante de tempo no planeta T2.
 * @author Diogo Carvalho(a90247)
 * @version 08-03-2026
 * @inv 0 <= seconds < T2DAYSECONDS
 * @see POO25-26_Lab3-JUnit.pdf
 */
public class T2time {
    static final int T2DAYSECONDS = 13 * 3600 + 27 * 60 + 16; //48436 secs/day
    private int seconds = 0; //valid range [0, T2DAYSECONDS-1]

    /**
     * @param secs an integer representing the number of seconds since midnight
     * @inv this.seconds in 0..T2DAYSECONDS-1
     * @post this.seconds==secs
     * @throws IllegalArgumentException if secs not in 0..T2DAYSECONDS-1
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public T2time(int secs) {
        if (secs < 0 || secs >= T2DAYSECONDS)
            throw new IllegalArgumentException("0 <= secs < T2DAYSECONDS");
        this.seconds = secs;
    }

    /**
     * @param h an integer representing the number of hours since midnight
     * @param m an integer representing the number of min since h
     * @param s an integer representing the number of hours since m
     * @inv this.seconds in 0..T2DAYSECONDS-1
     * @post this.seconds = h*3600 + m*60 + s
     * @throws IllegalArgumentException if (h*3600 + m*60 + s) not in [0, T2DAYSECONDS-1]
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public T2time(int h, int m, int s) {
        this(h * 3600 + m * 60 + s);
    }

    /**
     * Responsabilidade: criar um instante T2 a partir de uma string no formato HH:MM:SS.
     * @param time string com um instante no formato HH:MM:SS
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public T2time(String time) {
        String[] parts = time.split(":");
        int h = Integer.parseInt(parts[0]);
        int m = Integer.parseInt(parts[1]);
        int s = Integer.parseInt(parts[2]);
        this.seconds = h * 3600 + m * 60 + s;
        if (seconds < 0 || seconds >= T2DAYSECONDS)
            throw new IllegalArgumentException("0 <= secs < T2DAYSECONDS");
    }

    /**
     * @return this.seconds
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public int asSeconds() {
        return seconds;
    }

    /**
     * @return this.seconds formatted as "HH:MM:SS"
     * @see POO25-26_Lab3-JUnit.pdf
     */
    @Override
    public String toString() {
        int h = seconds / 3600;
        int m = (seconds % 3600) / 60;
        int s = (seconds % 3600) % 60;
        return String.format("%02d:%02d:%02d", h, m, s);
    }

    /**
     * @pre arg != null
     * @return new T2time with the sum of this and arg mod T2DAYSECONDS
     * @post this == old this
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public T2time add(T2time arg) {
        int sum = (this.seconds + arg.asSeconds()) % T2DAYSECONDS;
        return new T2time(sum);
    }

    /**
     * Responsabilidade: verificar se uma string representa um tempo.
     * @param s string a verificar
     * @return true se s representar um tempo; false caso contrário
     * @see POO25-26_Lab3-JUnit.pdf
     */
    public static boolean isTime(String s) {
        return s.contains(":");
    }
}