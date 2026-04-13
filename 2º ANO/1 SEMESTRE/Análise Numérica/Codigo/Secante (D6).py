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

def secant(expr, a, b, eps, itmax):
    x0 = a
    x1 = b
    f0 = f_eval(x0, expr)
    f1 = f_eval(x1, expr)
    for _ in range(itmax):
        den = f1 - f0
        if den == 0:
            return None
        x2 = x1 - f1 * (x1 - x0) / den
        if abs(x2 - x1) < eps:
            return x2
        x0, x1 = x1, x2
        f0, f1 = f1, f_eval(x1, expr)
    return x1

expr = norm_expr(input("f(x)= "))
a = to_float(input("a= "))
b = to_float(input("b= "))
eps = to_float(input("eps= "))
itmax = int(input("itmax= "))

try:
    r = secant(expr, a, b, eps, itmax)
    if r is None:
        print("Erro")
    else:
        print(r)
except:
    print("Erro")
