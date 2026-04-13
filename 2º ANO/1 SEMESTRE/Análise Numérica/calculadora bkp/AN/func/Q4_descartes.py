def sign_changes(coeffs):
    s = []
    i = 0
    while i < len(coeffs):
        c = coeffs[i]
        if c != 0:
            s.append(1 if c > 0 else -1)
        i += 1

    v = 0
    i = 1
    while i < len(s):
        if s[i] != s[i-1]:
            v += 1
        i += 1
    return v

def possibilities(v):
    out = []
    k = v
    while k >= 0:
        out.append(k)
        k -= 2
    return out

def main():
    print("Q4 — Regra de Descartes")
    n = int(input("Grau n = ").strip())
    print("Introduza coeficientes a_n ate a_0")
    coeffs = []
    deg = n
    while deg >= 0:
        coeffs.append(float(input("a_{} = ".format(deg))))
        deg -= 1

    vpos = sign_changes(coeffs)

    coeffs_m = []
    i = 0
    while i < len(coeffs):
        deg = n - i
        c = coeffs[i]
        coeffs_m.append(-c if (deg % 2 == 1) else c)
        i += 1

    vneg = sign_changes(coeffs_m)

    print("V(P(x))  = {}  -> positivas possiveis: {}".format(vpos, possibilities(vpos)))
    print("V(P(-x)) = {}  -> negativas possiveis: {}".format(vneg, possibilities(vneg)))

main()
