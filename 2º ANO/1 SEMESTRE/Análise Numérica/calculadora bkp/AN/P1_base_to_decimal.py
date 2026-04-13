import math

# P1 — Base b (digitos inteiros separados por virgula) -> base 10
# Formato de entrada tipico: 121,89,3}102,23,87   (base b ao lado no enunciado)
# '}' separa parte inteira da fracao, ',' separa digitos/posicoes (cada digito e um inteiro 0..b-1).

def _to_int(s):
    s = s.strip()
    if s == "":
        return None
    return int(s)

def parse_digits(s):
    s = s.strip()
    if s == "":
        return []
    parts = s.split(",")
    out = []
    i = 0
    while i < len(parts):
        v = _to_int(parts[i])
        if v is None:
            raise ValueError("digito vazio")
        out.append(v)
        i += 1
    return out

def base_to_decimal(num_str, b):
    s = num_str.strip()
    sign = 1
    if s.startswith("-"):
        sign = -1
        s = s[1:].strip()
    elif s.startswith("+"):
        s = s[1:].strip()

    if "}" in s:
        a, f = s.split("}")
    else:
        a, f = s, ""

    int_d = parse_digits(a)
    frac_d = parse_digits(f)

    i = 0
    while i < len(int_d):
        if int_d[i] < 0 or int_d[i] >= b:
            raise ValueError("digito fora da base: {}".format(int_d[i]))
        i += 1
    i = 0
    while i < len(frac_d):
        if frac_d[i] < 0 or frac_d[i] >= b:
            raise ValueError("digito fora da base: {}".format(frac_d[i]))
        i += 1

    val = 0.0
    i = 0
    while i < len(int_d):
        val = val*b + int_d[i]
        i += 1

    w = 1.0/b
    i = 0
    while i < len(frac_d):
        val += frac_d[i]*w
        w = w/b
        i += 1

    return sign*val

def _trunc(y):
    if y >= 0:
        return int(y)
    return -int(-y)

def round_sig(x, t, modo):
    if x == 0:
        return 0.0
    neg = False
    if x < 0:
        neg = True
        x = -x

    e = int(math.floor(math.log10(x)))
    scale = 10 ** (t - 1 - e)
    y = x * scale

    if modo == "corte":
        y2 = _trunc(y)
    elif modo == "defeito":
        y2 = math.floor(y)
    else:
        y2 = math.ceil(y)

    out = y2 / scale
    if neg:
        out = -out
    return out

def main():
    print("P1 — Base b -> Base 10")
    b = int(input("Base b = ").strip())
    s = input("Numero (ex: 121,89,3}102,23,87) = ").strip()

    try:
        x = base_to_decimal(s, b)
    except Exception as e:
        print("Erro:", e)
        return

    print("Base 10 = {}".format(x))

    op = input("Arredondar (base10) a t alg. relevantes? (s/n) ").strip().lower()
    if op == "s":
        t = int(input("t = ").strip())
        print("Modo: 1-corte  2-excesso  3-defeito")
        m = input("Opcao = ").strip()
        modo = {"1":"corte","2":"excesso","3":"defeito"}.get(m, None)
        if modo is None:
            print("Opcao invalida.")
            return
        print("Resultado = {}".format(round_sig(x, t, modo)))

main()
