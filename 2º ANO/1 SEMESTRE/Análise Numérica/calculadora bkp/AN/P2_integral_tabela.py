# P2 — Integral por tabela (NC fechadas) usando TODOS os pontos
# Preferencia: Simpson 1/3 em blocos; se sobrar 3 subintervalos usa 3/8; se sobrar 1 usa trapezio.

def _to_float(s):
    return float(s.strip().replace(",", "."))

def trap(h, f0, f1):
    return h*(f0+f1)/2.0

def simpson13(h, f0, f1, f2):
    return (h/3.0)*(f0 + 4.0*f1 + f2)

def simpson38(h, f0, f1, f2, f3):
    return (3.0*h/8.0)*(f0 + 3.0*f1 + 3.0*f2 + f3)

def main():
    print("P2 — Integral por tabela (usa todos os pontos)")
    m = int(input("Numero de pontos = ").strip())
    x = []
    f = []
    i = 0
    while i < m:
        x.append(_to_float(input("x[{}] = ".format(i))))
        f.append(_to_float(input("f[{}] = ".format(i))))
        i += 1

    if m < 2:
        print("Precisa de pelo menos 2 pontos.")
        return

    h = x[1] - x[0]
    n = m - 1
    I = 0.0

    i = 0
    while i < n:
        rem = n - i
        if rem >= 3 and (rem % 2 == 1):
            I += simpson38(h, f[i], f[i+1], f[i+2], f[i+3])
            i += 3
        elif rem >= 2:
            I += simpson13(h, f[i], f[i+1], f[i+2])
            i += 2
        else:
            I += trap(h, f[i], f[i+1])
            i += 1

    dec = int(input("Casas decimais na saida = ").strip())
    fmt = "{:." + str(dec) + "f}"
    print("Integral ~ {}".format(fmt.format(I)))

main()
