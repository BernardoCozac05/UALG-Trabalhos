DIG = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

# Q2 — Arredondamento em base b (MicroPython 1.9.4)
# Apenas 3 modos, consistentes com a regra do modulo em numeros negativos:
#  1) corte   : truncar (direcao a 0)
#  2) excesso : para x<0, excesso(x) = -defeito(|x|)  (equivale a ceil)
#  3) defeito : para x<0, defeito(x) = -excesso(|x|)  (equivale a floor)
#
# Tipo:
#  1) t algarismos significativos (normalizado)
#  2) p casas fracionarias (apos o ponto)

def _strip_sign(s):
    s = s.strip().upper()
    sign = 1
    if s.startswith("-"):
        sign = -1
        s = s[1:]
    elif s.startswith("+"):
        s = s[1:]
    return sign, s

def _d(ch, base):
    v = DIG.find(ch)
    if v < 0 or v >= base:
        raise ValueError("Digito invalido '{}' na base {}".format(ch, base))
    return v

def parse_num(s, base):
    sign, mag = _strip_sign(s)
    if "." in mag:
        a, b = mag.split(".")
    else:
        a, b = mag, ""
    if a == "":
        a = "0"

    int_d = []
    i = 0
    while i < len(a):
        int_d.append(_d(a[i], base))
        i += 1

    frac_d = []
    i = 0
    while i < len(b):
        frac_d.append(_d(b[i], base))
        i += 1

    return sign, int_d, frac_d

def _is_zero(int_d, frac_d):
    i = 0
    while i < len(int_d):
        if int_d[i] != 0:
            return False
        i += 1
    i = 0
    while i < len(frac_d):
        if frac_d[i] != 0:
            return False
        i += 1
    return True

def _digits_to_str(int_d, frac_d):
    i = 0
    while i < len(int_d) - 1 and int_d[i] == 0:
        i += 1
    int_d = int_d[i:]

    s = ""
    i = 0
    while i < len(int_d):
        s += DIG[int_d[i]]
        i += 1

    if len(frac_d) > 0:
        s += "."
        i = 0
        while i < len(frac_d):
            s += DIG[frac_d[i]]
            i += 1
    return s

def _normalize(int_d, frac_d):
    if _is_zero(int_d, frac_d):
        return [0], 0
    all_d = int_d + frac_d
    int_len = len(int_d)

    first = 0
    while first < len(all_d) and all_d[first] == 0:
        first += 1

    exp = int_len - 1 - first
    return all_d[first:], exp

def _sci_to_fixed(mant, exp, base):
    if mant == [0]:
        return [0], []
    k = exp + 1
    if k <= 0:
        return [0], [0] * (-k) + mant
    if len(mant) < k:
        return mant + [0]*(k - len(mant)), []
    return mant[:k], mant[k:]

def _inc_digits(digs, base):
    i = len(digs) - 1
    while i >= 0:
        digs[i] += 1
        if digs[i] < base:
            return digs, 0
        digs[i] = 0
        i -= 1
    return [1] + digs, 1

def _has_nonzero(digs):
    i = 0
    while i < len(digs):
        if digs[i] != 0:
            return True
        i += 1
    return False

def _round_mantissa_abs(mant, exp, base, t, mode_abs):
    # mode_abs: "corte" | "excesso" | "defeito" sobre |x|
    if mant == [0]:
        return [0], 0
    if t <= 0:
        return [0], 0

    keep = mant[:t]
    tail = mant[t:]

    if mode_abs == "corte" or mode_abs == "defeito":
        return keep, exp

    if mode_abs == "excesso":
        if _has_nonzero(tail):
            keep2, ov = _inc_digits(keep[:], base)
            if ov == 1:
                exp += 1
                keep2 = keep2[:t]
            keep = keep2
        return keep, exp

    raise ValueError("Modo invalido")

def _build_from_mant(mant, exp, base, neg):
    I, F = _sci_to_fixed(mant, exp, base)
    out = _digits_to_str(I, F)
    if neg and out != "0":
        return "-" + out
    return out

def round_sig(s, base, t, modo):
    sign, int_d, frac_d = parse_num(s, base)
    if _is_zero(int_d, frac_d):
        return "0"

    mant, exp = _normalize(int_d, frac_d)

    if sign >= 0:
        mode_abs = modo
        neg = False
    else:
        # regra do modulo:
        # excesso(x<0) = -defeito(|x|); defeito(|x|)=corte(|x|)
        # defeito(x<0) = -excesso(|x|)
        if modo == "corte":
            mode_abs = "corte"
            neg = True
        elif modo == "excesso":
            mode_abs = "defeito"
            neg = True
        else:  # defeito
            mode_abs = "excesso"
            neg = True

    m2, e2 = _round_mantissa_abs(mant, exp, base, t, mode_abs)
    return _build_from_mant(m2, e2, base, neg)

def _round_frac_abs(int_d, frac_d, base, p, mode_abs):
    if p < 0:
        p = 0
    frac = frac_d[:]
    while len(frac) < p + 1:
        frac.append(0)

    kept = frac[:p]
    tail = frac[p:]

    I = int_d[:]
    F = kept[:]

    if mode_abs == "corte" or mode_abs == "defeito":
        pass
    elif mode_abs == "excesso":
        if _has_nonzero(tail):
            if p == 0:
                I2, ov = _inc_digits(I, base)
                I = I2
                F = []
            else:
                j = p - 1
                carry = 1
                while j >= 0 and carry == 1:
                    F[j] += 1
                    if F[j] >= base:
                        F[j] = 0
                        carry = 1
                    else:
                        carry = 0
                    j -= 1
                if carry == 1:
                    I2, ov = _inc_digits(I, base)
                    I = I2
    else:
        raise ValueError("Modo invalido")

    if p == 0:
        F = []
    else:
        if len(F) < p:
            F = F + [0]*(p-len(F))
    return I, F

def round_frac(s, base, p, modo):
    sign, int_d, frac_d = parse_num(s, base)
    if _is_zero(int_d, frac_d):
        return "0"

    if sign >= 0:
        mode_abs = modo
        neg = False
    else:
        if modo == "corte":
            mode_abs = "corte"
            neg = True
        elif modo == "excesso":
            mode_abs = "defeito"
            neg = True
        else:
            mode_abs = "excesso"
            neg = True

    I, F = _round_frac_abs(int_d, frac_d, base, p, mode_abs)
    out = _digits_to_str(I, F)
    if neg and out != "0":
        return "-" + out
    return out

def main():
    print("Q2 — Arredondamento em base b")
    print("Bases 2..36; digitos 0-9 A-Z; use '.' para fracao.")
    s = input("Numero = ").strip()
    b = int(input("Base b = ").strip())

    print("")
    print("Modo:")
    print("1 - corte")
    print("2 - excesso")
    print("3 - defeito")
    op = input("Opcao = ").strip()
    modos = {"1":"corte","2":"excesso","3":"defeito"}
    if op not in modos:
        print("Opcao invalida.")
        return
    modo = modos[op]

    print("")
    print("Tipo:")
    print("1 - t algarismos significativos")
    print("2 - p casas fracionarias")
    tp = input("Opcao = ").strip()

    if tp == "1":
        t = int(input("t = ").strip())
        print("Resultado = {}".format(round_sig(s, b, t, modo)))
        return

    if tp == "2":
        p = int(input("p = ").strip())
        print("Resultado = {}".format(round_frac(s, b, p, modo)))
        return

    print("Opcao invalida.")

main()
