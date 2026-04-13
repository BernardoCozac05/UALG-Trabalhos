import math

# Q1 — Base 60 -> Decimal (a0}a1}a2}...)
# Melhorado: arredondamento em base 10 com varios modos:
#  - corte (truncar)
#  - excesso (ceil)
#  - defeito (floor)
#  - nearest_up (meio para cima)
#  - nearest_even (meio para par)
# Escolha: (A) algarismos relevantes  OU  (B) casas decimais

def base60_to_decimal(s):
    s = s.strip()
    sign = 1
    if s.startswith("-"):
        sign = -1
        s = s[1:].strip()
    elif s.startswith("+"):
        s = s[1:].strip()

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

def _trunc(y):
    # truncar em direcao a 0
    if y >= 0:
        return int(y)
    return -int(-y)

def round_decimals(x, d, mode):
    scale = 10 ** d
    y = x * scale

    if mode == "corte":
        return _trunc(y) / scale
    if mode == "defeito":
        return math.floor(y) / scale
    if mode == "excesso":
        return math.ceil(y) / scale

    # nearest_* : tratar por simetria para negativos
    if y < 0:
        return -round_decimals(-x, d, mode)

    base = math.floor(y)
    frac = y - base

    if mode == "nearest_up":
        if frac > 0.5:
            return (base + 1.0) / scale
        if frac < 0.5:
            return base / scale
        return (base + 1.0) / scale

    if mode == "nearest_even":
        if frac > 0.5:
            return (base + 1.0) / scale
        if frac < 0.5:
            return base / scale
        if int(base) % 2 == 0:
            return base / scale
        return (base + 1.0) / scale

    return x

def round_sig(x, t, mode):
    if x == 0:
        return 0.0
    if x < 0:
        return -round_sig(-x, t, mode)

    e = int(math.floor(math.log10(x)))
    scale = 10 ** (t - 1 - e)
    y = x * scale

    if mode == "corte":
        return _trunc(y) / scale
    if mode == "defeito":
        return math.floor(y) / scale
    if mode == "excesso":
        return math.ceil(y) / scale

    base = math.floor(y)
    frac = y - base

    if mode == "nearest_up":
        if frac > 0.5:
            return (base + 1.0) / scale
        if frac < 0.5:
            return base / scale
        return (base + 1.0) / scale

    if mode == "nearest_even":
        if frac > 0.5:
            return (base + 1.0) / scale
        if frac < 0.5:
            return base / scale
        if int(base) % 2 == 0:
            return base / scale
        return (base + 1.0) / scale

    return x

def choose_mode():
    print("")
    print("Modo de arredondamento:")
    print("1 - corte (truncar)")
    print("2 - defeito (floor)")
    print("3 - excesso (ceil)")
    print("4 - nearest_up (meio para cima)")
    print("5 - nearest_even (meio para par)")
    op = input("Opcao = ").strip()
    m = {"1":"corte","2":"defeito","3":"excesso","4":"nearest_up","5":"nearest_even"}
    if op not in m:
        return None
    return m[op]

def main():
    print("Q1 — Base 60 -> Decimal (a0}a1}a2}...)")
    print("Exemplos: 2}15}25   |   12}30   |  -1}10}00")
    s = input("Entrada = ").strip()
    x = base60_to_decimal(s)
    print("Decimal = {}".format(x))

    op = input("Aplicar arredondamento? (s/n) ").strip().lower()
    if op != "s":
        return

    mode = choose_mode()
    if mode is None:
        print("Opcao invalida.")
        return

    print("")
    print("Tipo:")
    print("1 - t algarismos significativos")
    print("2 - d casas decimais")
    tp = input("Opcao = ").strip()

    if tp == "1":
        t = int(input("t = ").strip())
        y = round_sig(x, t, mode)
        print("Resultado = {}".format(y))
        return

    if tp == "2":
        d = int(input("d = ").strip())
        y = round_decimals(x, d, mode)
        print("Resultado = {}".format(y))
        return

    print("Opcao invalida.")

main()
