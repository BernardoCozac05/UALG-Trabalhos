import math

# P2 — Base 10 -> Base b (suporta bases grandes como 60/123/132)
# Saida: digitos como inteiros separados por virgula; fracao depois de ';'
# Ex: 1.11 em base 2 com 10 digitos: 1;0,0,0,1,1,1,0,0,0,1

def _split_decimal_places(s):
    s = s.strip().replace(",", ".")
    if "." not in s:
        return 0
    return len(s.split(".")[1])

def _to_float(s):
    return float(s.strip().replace(",", "."))

def _int_to_digits(n, b):
    if n == 0:
        return [0]
    digs = []
    while n > 0:
        digs.append(n % b)
        n //= b
    digs.reverse()
    return digs

def _frac_to_digits(x, b, k):
    digs = []
    i = 0
    while i < k:
        x *= b
        d = int(x)
        digs.append(d)
        x -= d
        i += 1
    return digs

def _digits_str(int_d, frac_d):
    s = ""
    i = 0
    while i < len(int_d):
        if i > 0:
            s += ","
        s += str(int_d[i])
        i += 1
    if len(frac_d) > 0:
        s += ";"
        i = 0
        while i < len(frac_d):
            if i > 0:
                s += ","
            s += str(frac_d[i])
            i += 1
    return s

def main():
    print("P2 — Base 10 -> Base b")
    b = int(input("Base b = ").strip())
    s = input("x (base 10) = ").strip()
    x = _to_float(s)

    auto = input("Usar mesmas casas decimais do input? (s/n) ").strip().lower()
    if auto == "s":
        k = _split_decimal_places(s)
    else:
        k = int(input("k casas fracionarias = ").strip())

    neg = False
    if x < 0:
        neg = True
        x = -x

    n = int(x)
    frac = x - n

    int_d = _int_to_digits(n, b)
    frac_d = _frac_to_digits(frac, b, k)

    out = _digits_str(int_d, frac_d)
    if neg:
        out = "-" + out
    print("Resultado = {}".format(out))

main()
