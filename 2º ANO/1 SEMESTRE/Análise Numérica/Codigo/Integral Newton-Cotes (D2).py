from math import sin, cos, tan, exp, log, sqrt, pi, e

def to_float(s):
    return float(s.strip().replace(",", "."))

def norm_expr(s):
    s = s.strip().replace(" ", "")
    s = s.replace("^", "**")
    s = s.replace("sen(", "sin(")
    s = s.replace("tg(", "tan(")
    s = s.replace("ln(", "log(")
    s = s.replace("e(", "exp(")
    return s

def f_eval(x, expr):
    return eval(expr, {"x": x, "sin": sin, "cos": cos, "tan": tan,
                       "exp": exp, "log": log, "sqrt": sqrt, "pi": pi, "e": e})

def gauss(A, b):
    n = len(b)
    for i in range(n):
        piv = i
        mx = abs(A[i][i])
        for r in range(i + 1, n):
            v = abs(A[r][i])
            if v > mx:
                mx = v
                piv = r
        if mx == 0:
            return None
        if piv != i:
            A[i], A[piv] = A[piv], A[i]
            b[i], b[piv] = b[piv], b[i]
        p = A[i][i]
        inv = 1.0 / p
        for j in range(i, n):
            A[i][j] *= inv
        b[i] *= inv
        for r in range(n):
            if r == i:
                continue
            fac = A[r][i]
            if fac != 0.0:
                for j in range(i, n):
                    A[r][j] -= fac * A[i][j]
                b[r] -= fac * b[i]
    return b

def weights_closed(n):
    A = []
    b = []
    nn = float(n)
    for k in range(n + 1):
        row = []
        for i in range(n + 1):
            row.append((float(i)) ** k)
        A.append(row)
        b.append((nn ** (k + 1)) / float(k + 1))
    return gauss(A, b)

def weights_open(n):
    A = []
    b = []
    nn = float(n + 2)
    for k in range(n + 1):
        row = []
        for i in range(n + 1):
            row.append((float(i + 1)) ** k)
        A.append(row)
        b.append((nn ** (k + 1)) / float(k + 1))
    return gauss(A, b)

def nc_closed(expr, a, b, n):
    w = weights_closed(n)
    if w is None:
        return None
    h = (b - a) / float(n)
    s = 0.0
    for i in range(n + 1):
        x = a + float(i) * h
        fx = f_eval(x, expr)
        s += w[i] * fx
    return h * s

def nc_open(expr, a, b, n):
    w = weights_open(n)
    if w is None:
        return None
    h = (b - a) / float(n + 2)
    s = 0.0
    for i in range(n + 1):
        x = a + float(i + 1) * h
        fx = f_eval(x, expr)
        s += w[i] * fx
    return h * s

expr = norm_expr(input("f(x)= "))
a = to_float(input("a= "))
b = to_float(input("b= "))
n = int(input("n= "))
modo = int(input("Modo (1=fechada, 2=aberta, 3=auto): "))

try:
    if modo == 1:
        print(nc_closed(expr, a, b, n))
    elif modo == 2:
        print(nc_open(expr, a, b, n))
    else:
        try:
            print(nc_closed(expr, a, b, n))
        except:
            print(nc_open(expr, a, b, n))
except:
    print("Erro")
