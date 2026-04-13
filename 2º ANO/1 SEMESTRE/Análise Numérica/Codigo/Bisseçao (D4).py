from math import sin, cos, tan, exp, log, sqrt, pi, e

def to_float(s):
    return float(s.strip().replace(",", "."))

def norm_expr(s):
    s = s.strip().replace("^", "**")
    s = s.replace("sen(", "sin(")
    s = s.replace("tg(", "tan(")
    return s

def f_eval(x, expr):
    return eval(expr, {"x": x, "sin": sin, "cos": cos, "tan": tan, "exp": exp, "log": log, "sqrt": sqrt, "pi": pi, "e": e})

expr = norm_expr(input("f(x)= "))
a = to_float(input("a= "))
b = to_float(input("b= "))
eps = to_float(input("erro (ex: 1e-100)= "))

try:
    if eps <= 0:
        print("Erro")
    else:
        fa = f_eval(a, expr)
        fb = f_eval(b, expr)

        if fa == 0.0 or fb == 0.0:
            print(0)
        elif fa * fb > 0.0:
            print("Erro")
        else:
            L0 = b - a
            if L0 < 0:
                L0 = -L0
            if L0 == 0:
                print("Erro")
            else:
                q = L0 / eps
                if q <= 0:
                    print("Erro")
                else:
                    m = int(log(q) / log(2.0))
                    if (2.0 ** m) < q:
                        m += 1
                    n = m - 1
                    if n < 0:
                        n = 0
                    print(n)
except:
    print("Erro")
