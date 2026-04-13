DIG = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

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

def is_zero(int_d, frac_d):
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

def digits_to_str(int_d, frac_d):
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

def normalize(int_d, frac_d):
    if is_zero(int_d, frac_d):
        return [0], 0
    all_d = int_d + frac_d
    int_len = len(int_d)

    first = 0
    while first < len(all_d) and all_d[first] == 0:
        first += 1

    exp = int_len - 1 - first
    return all_d[first:], exp

def sci_to_fixed(mant, exp, base):
    if mant == [0]:
        return [0], []
    k = exp + 1
    if k <= 0:
        return [0], [0] * (-k) + mant

    if len(mant) < k:
        return mant + [0] * (k - len(mant)), []
    return mant[:k], mant[k:]

def inc_digits(digs, base):
    i = len(digs) - 1
    while i >= 0:
        digs[i] += 1
        if digs[i] < base:
            return digs, 0
        digs[i] = 0
        i -= 1
    return [1] + digs, 1

def compare_tail_half(tail, base):
    if len(tail) == 0:
        return -1
    half = base // 2
    d0 = tail[0]
    if d0 > half:
        return 1
    if d0 < half:
        return -1
    i = 1
    while i < len(tail):
        if tail[i] != 0:
            return 1
        i += 1
    return 0

def round_sig_mag(mant, exp, base, t, mode):
    if mant == [0]:
        return [0], 0

    keep = mant[:t]
    tail = mant[t:]

    inc = False
    if mode == "corte":
        inc = False
    elif mode == "excesso":
        i = 0
        while i < len(tail):
            if tail[i] != 0:
                inc = True
                break
            i += 1
    elif mode == "nearest_up":
        inc = (compare_tail_half(tail, base) >= 0)
    elif mode == "nearest_even":
        cmp = compare_tail_half(tail, base)
        if cmp > 0:
            inc = True
        elif cmp < 0:
            inc = False
        else:
            inc = (keep[-1] % 2 == 1)
    else:
        raise ValueError("Modo invalido")

    if inc:
        keep2, ov = inc_digits(keep[:], base)
        if ov == 1:
            exp += 1
            keep2 = keep2[:t]
        keep = keep2
    return keep, exp

def round_sig(s, base, t, modo_global):
    sign, int_d, frac_d = parse_num(s, base)
    if is_zero(int_d, frac_d):
        return "0"

    mant, exp = normalize(int_d, frac_d)

    def build(mant2, exp2, sign_out):
        I, F = sci_to_fixed(mant2, exp2, base)
        out = digits_to_str(I, F)
        if sign_out < 0 and out != "0":
            return "-" + out
        return out

    if modo_global == "corte":
        m2, e2 = round_sig_mag(mant, exp, base, t, "corte")
        return build(m2, e2, sign)

    if modo_global == "nearest_up":
        m2, e2 = round_sig_mag(mant, exp, base, t, "nearest_up")
        return build(m2, e2, sign)

    if modo_global == "nearest_even":
        m2, e2 = round_sig_mag(mant, exp, base, t, "nearest_even")
        return build(m2, e2, sign)

    if modo_global == "excesso_modulo":
        m2, e2 = round_sig_mag(mant, exp, base, t, "excesso")
        return build(m2, e2, sign)

    if modo_global == "defeito_modulo":
        if sign >= 0:
            m2, e2 = round_sig_mag(mant, exp, base, t, "corte")
            return build(m2, e2, 1)
        else:
            m2, e2 = round_sig_mag(mant, exp, base, t, "excesso")
            return build(m2, e2, -1)

    raise ValueError("Modo invalido")

def main():
    print("Q2 — Arredondamento em base b (t alg. relevantes)")
    print("Bases 2..36; digitos 0-9 A-Z.")
    s = input("Numero (ex: -5.3124) = ").strip()
    b = int(input("Base b = ").strip())
    t = int(input("t (alg. relevantes) = ").strip())

    print("Modo:")
    print("1 - corte (truncar)")
    print("2 - nearest_up (meio para cima)")
    print("3 - nearest_even (meio para par)")
    print("4 - excesso_modulo (excesso(|x|))")
    print("5 - defeito_modulo (defeito(x<0)=-excesso(|x|))")
    op = input("Opcao = ").strip()

    modos = {"1":"corte","2":"nearest_up","3":"nearest_even","4":"excesso_modulo","5":"defeito_modulo"}
    if op not in modos:
        print("Opcao invalida.")
        return

    out = round_sig(s, b, t, modos[op])
    print("Resultado = {}".format(out))

main()
