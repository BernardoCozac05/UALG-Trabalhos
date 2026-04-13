def char_to_digit(ch):
    o = ord(ch)
    if 48 <= o <= 57:
        return o - 48
    if 65 <= o <= 90:
        return o - 65 + 10
    if 97 <= o <= 122:
        return o - 97 + 10
    return -1

def parse_base_b_number(num_str, b):
    s = num_str.strip()
    if s and (s[0] == '+' or s[0] == '-'):
        s = s[1:]
    if '.' in s:
        a, f = s.split('.', 1)
    else:
        a, f = s, ""
    if a == "":
        a = "0"
    m = len(f)
    num = 0
    for ch in a:
        d = char_to_digit(ch)
        if d < 0 or d >= b:
            raise ValueError("digito invalido")
        num = num * b + d
    for _ in range(m):
        num *= b
    frac_num = 0
    for ch in f:
        d = char_to_digit(ch)
        if d < 0 or d >= b:
            raise ValueError("digito invalido")
        frac_num = frac_num * b + d
    num += frac_num
    return num, m, a, f

def pow_int(base, exp):
    r = 1
    for _ in range(exp):
        r *= base
    return r

def compute_k(int_part_str, frac_part_str):
    a = int_part_str
    i = 0
    while i < len(a) and a[i] == '0':
        i += 1
    if i < len(a):
        return len(a) - i
    f = frac_part_str
    t = 1
    for ch in f:
        if ch != '0':
            return 1 - t
        t += 1
    return 1

def max_sig_correct(xbar_str, eps_str, b):
    x_num, x_m, a, f = parse_base_b_number(xbar_str, b)
    e_num, e_m, _, _ = parse_base_b_number(eps_str, b)
    if e_num == 0:
        return 99
    k = compute_k(a, f)
    best = 0
    for p in range(1, 31):
        exp = e_m + k - p
        if exp < 0:
            continue
        rhs = pow_int(b, exp)
        if 2 * e_num <= rhs:
            best = p
    return best

b = int(input("Base b: "))
xbar_str = input("X: ").strip()
eps_str = input("Erro: ").strip()

try:
    p = max_sig_correct(xbar_str, eps_str, b)
    print(p)
except Exception as e:
    print("Erro")
