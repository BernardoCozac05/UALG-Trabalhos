DIG = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

def base_to_float(s, base):
    s = s.strip().upper().replace(',', '.')
    sign = 1
    if s.startswith("-"):
        sign = -1
        s = s[1:]
    elif s.startswith("+"):
        s = s[1:]

    if "." in s:
        a, b = s.split(".")
    else:
        a, b = s, ""
    if a == "":
        a = "0"

    iv = 0
    i = 0
    while i < len(a):
        d = DIG.find(a[i])
        if d < 0 or d >= base:
            raise ValueError("Digito invalido")
        iv = iv * base + d
        i += 1

    fv = 0.0
    p = 1.0 / base
    i = 0
    while i < len(b):
        d = DIG.find(b[i])
        if d < 0 or d >= base:
            raise ValueError("Digito invalido")
        fv += d * p
        p /= base
        i += 1

    return sign * (iv + fv)

def first_sig_k(xbar, base):
    s = xbar.strip().upper()
    if s.startswith("-") or s.startswith("+"):
        s = s[1:]

    if "." not in s:
        return 0

    a, b = s.split(".")
    if a != "" and any(ch != "0" for ch in a):
        return 0

    k = 1
    i = 0
    while i < len(b):
        d = DIG.find(b[i])
        if d < 0 or d >= base:
            raise ValueError("Digito invalido")
        if d != 0:
            break
        k += 1
        i += 1
    return k

def main():
    print("Q3 — Algarismos significativos a partir do erro (base b)")
    print("Regra tipica (x<1): erro < 0.5 * b^{-(k+t-1)}")
    xbar = input("x_barra (ex: 0.123) = ").strip().replace(",", ".")
    err  = input("erro   (ex: 0.0024) = ").strip().replace(",", ".")
    b    = int(input("Base b = ").strip())

    k = first_sig_k(xbar, b)
    e = abs(base_to_float(err, b))

    best = 0
    t = 1
    while t <= 30:
        thr = 0.5 * (b ** (-(k + t - 1)))
        if e < thr:
            best = t
            t += 1
        else:
            break

    print("k = {}".format(k))
    print("erro (decimal) = {}".format(e))
    print("t = {}".format(best))

main()
