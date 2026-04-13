def c2d(ch):
    o = ord(ch)
    if 48 <= o <= 57:
        return o - 48
    if 65 <= o <= 90:
        return o - 65 + 10
    if 97 <= o <= 122:
        return o - 97 + 10
    return -1

def d2c(d):
    return chr(48 + d) if d < 10 else chr(65 + d - 10)

def norm(s):
    s = s.strip()
    s = s.replace("−", "-").replace("–", "-").replace("—", "-").replace("-", "-")
    s = s.replace("_", ".").replace(",", ".").replace("·", ".").replace("٫", ".")
    p = s.find("(")
    if p != -1:
        s = s[:p].strip()
    return s

def parse_num(s, b):
    s = norm(s)
    neg = False
    if s and s[0] in "+-":
        neg = (s[0] == "-")
        s = s[1:]
    if "." in s:
        a, f = s.split(".", 1)
    else:
        a, f = s, ""
    if a == "":
        a = "0"
    int_d = []
    for ch in a:
        d = c2d(ch)
        if d < 0 or d >= b:
            raise ValueError
        int_d.append(d)
    frac_d = []
    for ch in f:
        d = c2d(ch)
        if d < 0 or d >= b:
            raise ValueError
        frac_d.append(d)
    return neg, int_d, frac_d

def first_nz(int_d, frac_d):
    for i in range(len(int_d)):
        if int_d[i] != 0:
            return i, 1
    for j in range(len(frac_d)):
        if frac_d[j] != 0:
            return j, 0
    return 0, 1

def any_nz(a, start):
    for i in range(start, len(a)):
        if a[i] != 0:
            return True
    return False

def add1(digs, idx, b):
    i = idx
    while i >= 0:
        v = digs[i] + 1
        if v == b:
            digs[i] = 0
            i -= 1
        else:
            digs[i] = v
            return 0
    digs.insert(0, 1)
    return 1

def fmt(neg, digs, point):
    i = 0
    while i < point - 1 and digs[i] == 0:
        i += 1
    s = "-" if neg else ""
    for k in range(i, point):
        s += d2c(digs[k])
    j = len(digs) - 1
    while j >= point and digs[j] == 0:
        j -= 1
    if j >= point:
        s += "."
        for k in range(point, j + 1):
            s += d2c(digs[k])
    return s if s not in ("", "-") else "0"

def round_rel(x_str, b, N, t):
    neg, int_d, frac_d = parse_num(x_str, b)
    total = int_d + frac_d
    int_len = len(int_d)

    nz, in_int = first_nz(int_d, frac_d)
    start = nz if in_int else int_len + nz
    if start >= len(total):
        return "0"

    keep = start + N - 1
    trunc = keep < len(total) - 1
    rd = total[keep + 1] if trunc else 0
    tail = any_nz(total, keep + 2) if trunc else False
    disc_nz = trunc and (rd != 0 or tail)

    if keep < int_len:
        digs = int_d[:]
        for i in range(keep + 1, int_len):
            digs[i] = 0
        point = int_len
        keep_idx = keep
    else:
        digs = int_d[:] + frac_d[:keep - int_len + 1]
        point = int_len
        keep_idx = keep

    inc = False
    if t == 1:
        inc = disc_nz and neg
    elif t == 2:
        inc = disc_nz and (not neg)
    else:
        if trunc:
            if rd * 2 > b:
                inc = True
            elif rd * 2 < b:
                inc = False
            else:
                if tail:
                    inc = True
                else:
                    inc = (b % 2 == 0 and rd == b // 2 and (digs[keep_idx] % 2 == 1))

    if inc:
        point += add1(digs, keep_idx, b)

    return fmt(neg, digs, point)

b = int(input("Base: "))
N = int(input("Alg. rel.: "))
t = int(input("(1=def,2=exc,3=sim): "))
x = input("x: ")

try:
    if b < 2 or t < 1 or t > 3 or N < 1:
        print("Erro")
    else:
        print(round_rel(x, b, N, t))
except:
    print("Erro")
