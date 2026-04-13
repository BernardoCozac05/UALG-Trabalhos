from math import sin, cos, tan, exp, log, sqrt, pi, e

def to_float(s):
    return float(s.strip().replace(",", "."))

def norm_expr(s):
    s = s.strip().replace("^", "**")
    s = s.replace("sen(", "sin(")
    s = s.replace("tg(", "tan(")
    return s

def f(expr, x, y):
    return eval(expr, {"x": x, "y": y, "sin": sin, "cos": cos, "tan": tan, "exp": exp, "log": log, "sqrt": sqrt, "pi": pi, "e": e})

def jac_num(f1, f2, x, y):
    h = 1e-6
    f1x1 = f(f1, x + h, y)
    f1x0 = f(f1, x - h, y)
    f1y1 = f(f1, x, y + h)
    f1y0 = f(f1, x, y - h)
    f2x1 = f(f2, x + h, y)
    f2x0 = f(f2, x - h, y)
    f2y1 = f(f2, x, y + h)
    f2y0 = f(f2, x, y - h)
    a = (f1x1 - f1x0) / (2*h)
    b = (f1y1 - f1y0) / (2*h)
    c = (f2x1 - f2x0) / (2*h)
    d = (f2y1 - f2y0) / (2*h)
    return a, b, c, d

def newton1(f1, f2, x0, y0):
    F1 = f(f1, x0, y0)
    F2 = f(f2, x0, y0)
    a, b, c, d = jac_num(f1, f2, x0, y0)
    det = a*d - b*c
    if det == 0:
        return None
    dx = (-d*F1 + b*F2) / det
    dy = (c*F1 - a*F2) / det
    return x0 + dx, y0 + dy

f1 = norm_expr(input("F1(x,y)= "))
f2 = norm_expr(input("F2(x,y)= "))
x0 = to_float(input("x0= "))
y0 = to_float(input("y0= "))

try:
    r = newton1(f1, f2, x0, y0)
    if r is None:
        print("Erro")
    else:
        print(r[0], r[1])
except:
    print("Erro")
