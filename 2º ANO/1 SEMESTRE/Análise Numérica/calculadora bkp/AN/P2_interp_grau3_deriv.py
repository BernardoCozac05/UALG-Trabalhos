# P2 — Interpolacao grau 3 (4 pontos) + derivada em x*
# Saidas: coeficientes de Newton (divided differences) e P(x*), P'(x*)

def _to_float(s):
    return float(s.strip().replace(",", "."))

def divided_differences(x, y):
    n = len(x)
    a = y[:]
    coef = [0.0]*n
    coef[0] = a[0]
    j = 1
    while j < n:
        i = n-1
        while i >= j:
            a[i] = (a[i] - a[i-1])/(x[i] - x[i-j])
            i -= 1
        coef[j] = a[j]
        j += 1
    return coef

def eval_and_derivative(x, coef, xp):
    n = len(coef)
    b = coef[n-1]
    db = 0.0
    i = n-2
    while i >= 0:
        db = b + (xp - x[i]) * db
        b = coef[i] + (xp - x[i]) * b
        i -= 1
    return b, db

def main():
    print("P2 — Interpolacao grau 3 + derivada")
    x = []
    y = []
    i = 0
    while i < 4:
        x.append(_to_float(input("x[{}] = ".format(i))))
        y.append(_to_float(input("y[{}] = ".format(i))))
        i += 1

    coef = divided_differences(x, y)

    print("")
    print("Coeficientes de Newton (a0..a3):")
    i = 0
    while i < 4:
        print("a{} = {}".format(i, coef[i]))
        i += 1

    op = input("Calcular P(x*) e P'(x*)? (s/n) ").strip().lower()
    if op == "s":
        xp = _to_float(input("x* = "))
        P, dP = eval_and_derivative(x, coef, xp)
        dec = int(input("Casas decimais na saida = ").strip())
        fmt = "{:." + str(dec) + "f}"
        print("P(x*)  = {}".format(fmt.format(P)))
        print("P'(x*) = {}".format(fmt.format(dP)))

main()
