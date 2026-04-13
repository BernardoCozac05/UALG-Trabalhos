import math

def base60_to_decimal(s):
    s = s.strip()
    sign = 1
    if s.startswith("-"):
        sign = -1
        s = s[1:].strip()
    elif s.startswith("+"):
        s = s[1:].strip()

    # Separador em base 60: usa '}' (ex: 2}15}25)
    parts = s.split("}")
    if len(parts) == 0:
        print("Entrada vazia.")
        return 0.0

    vals = []
    i = 0
    while i < len(parts):
        p = parts[i].strip()
        if p == "":
            print("Campo vazio.")
            return 0.0
        v = int(p)
        if v < 0:
            print("Campos devem ser >= 0.")
            return 0.0
        if i > 0 and v >= 60:
            print("Campo fora de 0..59 na posicao {}: {}".format(i, v))
            return 0.0
        vals.append(v)
        i += 1

    total = 0.0
    i = 0
    while i < len(vals):
        total += vals[i] / (60 ** i)
        i += 1
    return sign * total

def round_sig_excess(x, t):
    if x == 0:
        return 0.0
    if x < 0:
        return -round_sig_excess(-x, t)

    e = int(math.floor(math.log10(x)))
    scale = 10 ** (t - 1 - e)
    y = x * scale
    yi = int(y)
    if y == yi:
        return x
    yi += 1
    return yi / scale

def main():
    print("Q1 — Base 60 -> Decimal (a0}a1}a2}...)")
    print("Exemplos: 2}15}25   |   12}30   |  -1}10}00")
    s = input("Entrada = ").strip()
    x = base60_to_decimal(s)
    print("Decimal = {}".format(x))

    op = input("Arredondar por excesso a t alg. relevantes? (s/n) ").strip().lower()
    if op == "s":
        t = int(input("t = ").strip())
        print("Excesso (t) = {}".format(round_sig_excess(x, t)))

main()
