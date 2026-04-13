# P2 — Derivadas a partir de tabela (x igualmente espacados)
# Extremidades: 3-pontos O(h^2); interior: diferenca central.

def _to_float(s):
    return float(s.strip().replace(",", "."))

def main():
    print("P2 — Derivadas de tabela (f'(x))")
    m = int(input("Numero de pontos = ").strip())
    x = []
    f = []
    i = 0
    while i < m:
        x.append(_to_float(input("x[{}] = ".format(i))))
        f.append(_to_float(input("f[{}] = ".format(i))))
        i += 1

    if m < 3:
        print("Precisa de pelo menos 3 pontos.")
        return

    h = x[1] - x[0]
    dec = int(input("Casas decimais na saida = ").strip())
    fmt = "{:." + str(dec) + "f}"

    fp = [0.0]*m
    fp[0] = (-3.0*f[0] + 4.0*f[1] - f[2])/(2.0*h)
    fp[m-1] = (3.0*f[m-1] - 4.0*f[m-2] + f[m-3])/(2.0*h)

    i = 1
    while i < m-1:
        fp[i] = (f[i+1] - f[i-1])/(2.0*h)
        i += 1

    print("")
    i = 0
    while i < m:
        print("f'({}) = {}".format(x[i], fmt.format(fp[i])))
        i += 1

main()
