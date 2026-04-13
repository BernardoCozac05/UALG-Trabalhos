def pow10(k):
    p = 1
    for _ in range(k):
        p *= 10
    return p

def round_sig_fraction(num, den, n, mode):
    if den == 0 or n < 1:
        return "Erro"
    if num == 0:
        return "0"

    neg = num < 0
    if neg:
        num = -num

    ip = num // den
    r = num % den

    if ip != 0:
        k = len(str(ip))
        dec = n - k
        if dec < 0:
            dec = 0
    else:
        if r == 0:
            dec = 0
        else:
            z = 0
            t = r
            while (t * 10) < den:
                t *= 10
                z += 1
            dec = z + n

    scale = pow10(dec)
    q, rem = divmod(num * scale, den)

    if mode == 1:
        if rem != 0:
            q += 1
    elif mode == 2:
        pass
    else:
        if rem * 2 >= den:
            q += 1

    s = str(q)
    if dec == 0:
        out = s
    else:
        if len(s) <= dec:
            s = "0" * (dec - len(s) + 1) + s
        out = s[:-dec] + "." + s[-dec:]

    if neg:
        out = "-" + out
    return out

def parse_base_number(x, b):
    s = x.strip()
    neg = False
    if s and s[0] in "+-":
        neg = (s[0] == "-")
        s = s[1:].strip()

    if "}" in s:
        a, f = s.split("}", 1)
    else:
        a, f = s, ""

    ai = [p for p in a.split(",") if p.strip() != ""]
    af = [p for p in f.split(",") if p.strip() != ""]

    ip = 0
    for p in ai:
        d = int(p.strip())
        if d < 0 or d >= b:
            return None
        ip = ip * b + d

    num = ip
    den = 1
    for p in af:
        d = int(p.strip())
        if d < 0 or d >= b:
            return None
        num = num * b + d
        den *= b

    if neg:
        num = -num
    return num, den

b = int(input("Base b: "))
print("(',' entre algarismos e '}' entre int e frac)")
x = input("x: ")
t = int(input("(1=exces, 2=def, 3=sim): "))
n = int(input("Alg. rel.: "))

try:
    parsed = parse_base_number(x, b)
    if parsed is None or b < 2 or t not in (1, 2, 3) or n < 1:
        print("Erro")
    else:
        num, den = parsed
        print(round_sig_fraction(num, den, n, t))
except:
    print("Erro")
