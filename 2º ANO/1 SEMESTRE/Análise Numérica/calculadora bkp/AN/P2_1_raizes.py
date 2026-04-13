import math

# P2.1 — Raizes f(x)=0 (Bisseccao / Secante / Newton)
# Tudo por INPUT (sem editar codigo).
#
# Como escrever a funcao:
#  - use x como variavel
#  - pode usar: sin, cos, tan, exp, log, sqrt, abs, pi, e
#  - use ** para potencia (se escrever ^, o programa troca por **)
# Exemplo: exp(x) - 3*cos(x) + x**2 + 1

def _mk_env(x):
    # ambiente permitido para eval
    env = {}
    env["x"] = x
    env["sin"] = math.sin
    env["cos"] = math.cos
    env["tan"] = math.tan
    env["exp"] = math.exp
    env["log"] = math.log
    env["sqrt"] = math.sqrt
    env["abs"] = abs
    env["pi"] = math.pi
    env["e"] = math.e
    return env

def f_eval(expr, x):
    expr = expr.replace("^", "**")
    # evita builtins (best-effort)
    try:
        return eval(expr, {"__builtins__": None}, _mk_env(x))
    except:
        # fallback (alguns firmwares nao gostam de __builtins__)
        return eval(expr, _mk_env(x), _mk_env(x))

def d_num(expr, x, h):
    return (f_eval(expr, x + h) - f_eval(expr, x - h)) / (2.0*h)

def bisseccao(expr, a, b, tol, maxit):
    fa = f_eval(expr, a)
    fb = f_eval(expr, b)
    if fa == 0:
        print("a e raiz.")
        return a
    if fb == 0:
        print("b e raiz.")
        return b
    if fa*fb > 0:
        print("Erro: f(a)*f(b) > 0 (nao garante raiz).")
        return None

    k = 0
    x = (a + b)/2.0
    fx = f_eval(expr, x)
    print("k   a            b            x            f(x)         |b-a|")
    while k < maxit:
        x = (a + b)/2.0
        fx = f_eval(expr, x)
        width = b - a
        if width < 0:
            width = -width
        print("{}  {}  {}  {}  {}  {}".format(k, a, b, x, fx, width))
        if width/2.0 < tol:
            print("STOP: (b-a)/2 < tol")
            return x
        if fx == 0:
            print("STOP: f(x)=0")
            return x
        if fa*fx < 0:
            b = x
            fb = fx
        else:
            a = x
            fa = fx
        k += 1
    print("maxit atingido")
    return x

def secante(expr, x0, x1, tol, maxit):
    f0 = f_eval(expr, x0)
    f1 = f_eval(expr, x1)
    print("k   x            f(x)         |dx|")
    print("0  {}  {}  -".format(x0, f0))
    print("1  {}  {}  -".format(x1, f1))

    k = 2
    while k <= maxit:
        den = (f1 - f0)
        if den == 0:
            print("Parou: f(x1)-f(x0)=0")
            return x1
        x2 = x1 - f1*(x1 - x0)/den
        dx = x2 - x1
        if dx < 0:
            dx = -dx
        f2 = f_eval(expr, x2)
        print("{}  {}  {}  {}".format(k, x2, f2, dx))
        if dx < tol:
            print("STOP: |dx| < tol")
            return x2
        x0, f0 = x1, f1
        x1, f1 = x2, f2
        k += 1
    print("maxit atingido")
    return x1

def newton(expr_f, expr_df, x0, tol, maxit, modo, h):
    print("k   x            f(x)         f'(x)        |dx|")
    k = 0
    x = x0
    while k < maxit:
        fx = f_eval(expr_f, x)
        if modo == 2:
            dfx = f_eval(expr_df, x)
        else:
            dfx = d_num(expr_f, x, h)

        if dfx == 0:
            print("Parou: f'(x)=0")
            return x

        x1 = x - fx/dfx
        dx = x1 - x
        if dx < 0:
            dx = -dx
        print("{}  {}  {}  {}  {}".format(k, x, fx, dfx, dx))
        if dx < tol:
            print("STOP: |dx| < tol")
            return x1
        x = x1
        k += 1
    print("maxit atingido")
    return x

def main():
    print("P2.1 — Raizes f(x)=0")
    print("Escreva f(x) com x e funcoes: sin cos tan exp log sqrt abs pi e")
    expr = input("f(x) = ").strip()

    print("")
    print("Metodo:")
    print("1 - Bisseccao")
    print("2 - Secante")
    print("3 - Newton")
    op = input("Opcao = ").strip()

    if op == "1":
        a = float(input("a = "))
        b = float(input("b = "))
        tol = float(input("tol = "))
        maxit = int(input("maxit = "))
        r = bisseccao(expr, a, b, tol, maxit)
        print("raiz ~", r)
        return

    if op == "2":
        x0 = float(input("x0 = "))
        x1 = float(input("x1 = "))
        tol = float(input("tol = "))
        maxit = int(input("maxit = "))
        r = secante(expr, x0, x1, tol, maxit)
        print("raiz ~", r)
        return

    if op == "3":
        x0 = float(input("x0 = "))
        tol = float(input("tol = "))
        maxit = int(input("maxit = "))

        print("")
        print("Derivada:")
        print("1 - Numerica (dif. central)")
        print("2 - Fornecer f'(x) por expressao")
        modo = int(input("Opcao = "))
        expr_df = "0"
        h = 1e-6
        if modo == 2:
            expr_df = input("f'(x) = ").strip()
        else:
            h = float(input("h (ex: 1e-6) = "))

        r = newton(expr, expr_df, x0, tol, maxit, modo, h)
        print("raiz ~", r)
        return

    print("Opcao invalida.")

main()
