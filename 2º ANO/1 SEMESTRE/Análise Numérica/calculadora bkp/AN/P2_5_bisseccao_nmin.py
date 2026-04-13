import math

# P2.5 — Bisseccao: numero minimo de iteracoes para garantir erro < tol
# |e_n| <= (b-a)/2^{n+1} < tol
# n = menor inteiro que satisfaz.

def min_iters(a, b, tol):
    L0 = b - a
    if L0 < 0:
        L0 = -L0
    if tol <= 0 or L0 <= 0:
        return None
    # usar log base 2
    n1 = math.log(L0 / tol, 2)
    n = int(math.floor(n1))
    while L0 / (2 ** (n + 1)) >= tol:
        n += 1
    return n

def main():
    print("P2.5 — n minimo (bisseccao)")
    a = float(input("a = "))
    b = float(input("b = "))
    tol = float(input("tol = "))
    n = min_iters(a, b, tol)
    if n is None:
        print("Dados invalidos.")
        return
    print("n minimo = {}".format(n))

main()
