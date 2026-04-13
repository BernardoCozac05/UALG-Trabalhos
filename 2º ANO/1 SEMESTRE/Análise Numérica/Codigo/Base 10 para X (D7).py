def to_int(s):
    return int(s.strip())

def to_float(s):
    return float(s.strip().replace(",", "."))

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def split_signed(s):
    s = s.strip()
    neg = False
    if s and s[0] in "+-":
        neg = (s[0] == "-")
        s = s[1:].strip()
    return neg, s

def char_to_val(c):
    o = ord(c)
    if 48 <= o <= 57:
        return o - 48
    c = c.upper()
    o = ord(c)
    if 65 <= o <= 90:
        return 10 + (o - 65)
    return -1

def val_to_char(v):
    if v < 10:
        return chr(48 + v)
    return chr(65 + (v - 10))

def parse_base_le36(s, b):
    neg, s = split_signed(s)
    if "." in s:
        a, f = s.split(".", 1)
    else:
        a, f = s, ""
    if a == "":
        a = "0"

    ip = 0
    for ch in a:
        v = char_to_val(ch)
        if v < 0 or v >= b:
            return None
        ip = ip * b + v

    num = ip
    den = 1
    for ch in f:
        v = char_to_val(ch)
        if v < 0 or v >= b:
            return None
        num = num * b + v
        den *= b

    if neg:
        num = -num
    g = gcd(abs(num), den)
    return num // g, den // g

def parse_base_gt36(s, b):
    neg, s = split_signed(s)
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
    g = gcd(abs(num), den)
    return num // g, den // g

def parse_number(s, b):
    if b <= 36:
        return parse_base_le36(s, b)
    return parse_base_gt36(s, b)

def int_to_base_le36(n, b):
    if n == 0:
        return "0"
    s = ""
    while n > 0:
        s = val_to_char(n % b) + s
        n //= b
    return s

def frac_to_base_le36(num, den, b, k):
    out = ""
    r = num % den
    for _ in range(k):
        r *= b
        d = r // den
        r = r % den
        out += val_to_char(int(d))
    return out

def int_to_base_gt36(n, b):
    if n == 0:
        return "0"
    digs = []
    while n > 0:
        digs.append(n % b)
        n //= b
    digs.reverse()
    return ",".join(str(d) for d in digs)

def frac_to_base_gt36(num, den, b, k):
    r = num % den
    digs = []
    for _ in range(k):
        r *= b
        d = r // den
        r = r % den
        digs.append(int(d))
    return ",".join(str(d) for d in digs)

def format_out(num, den, b, k):
    neg = num < 0
    if neg:
        num = -num

    ip = num // den

    if b <= 36:
        a = int_to_base_le36(ip, b)
        if k == 0:
            s = a
        else:
            f = frac_to_base_le36(num, den, b, k)
            s = a + "." + f
    else:
        a = int_to_base_gt36(ip, b)
        if k == 0:
            s = a
        else:
            f = frac_to_base_gt36(num, den, b, k)
            s = a + "}" + f

    if neg:
        s = "-" + s
    return s

b1 = int(input("Base origem: "))
b2 = int(input("Base destino: "))
k = int(input("Digitos fracionarios na base destino: "))
x = input("x: ").strip()

try:
    if b1 < 2 or b2 < 2 or k < 0:
        print("Erro")
    else:
        parsed = parse_number(x, b1)
        if parsed is None:
            print("Erro")
        else:
            num, den = parsed
            print(format_out(num, den, b2, k))
except:
    print("Erro")
