import math

# P2.3 — Integracao numerica (trapézio / Simpson / ponto-medio)
# Tudo por INPUT (sem editar codigo).
#
# Funcao f(x): use x e funcoes: sin cos tan exp log sqrt abs pi e
# Exemplo: exp(x)/x

def _mk_env(x):
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
    try:
        return eval(expr, {"__builtins__": None}, _mk_env(x))
    except:
        return eval(expr, _mk_env(x), _mk_env(x))

def trap(expr, a, b, n):
    h = (b-a)/n
    s = 0.5*(f_eval(expr, a) + f_eval(expr, b))
    i = 1
    while i < n:
        s += f_eval(expr, a + i*h)
        i += 1
    return h*s

def midpoint(expr, a, b, n):
    h = (b-a)/n
    s = 0.0
    i = 0
    while i < n:
        xm = a + (i+0.5)*h
        s += f_eval(expr, xm)
        i += 1
    return h*s

def simpson(expr, a, b, n):
    # n deve ser par; se impar, ajusta
    if n % 2 == 1:
        n += 1
    h = (b-a)/n
    s = f_eval(expr, a) + f_eval(expr, b)
    i = 1
    while i < n:
        coef = 4.0 if (i % 2 == 1) else 2.0
        s += coef * f_eval(expr, a + i*h)
        i += 1
    return (h/3.0)*s

def main():
    print("P2.3 — Integracao numerica")
    expr = input("f(x) = ").strip()
    a = float(input("a = "))
    b = float(input("b = "))
    n = int(input("n subintervalos = "))

    print("")
    print("Metodo:")
    print("1 - Trapezio")
    print("2 - Simpson")
    print("3 - Ponto medio")
    op = input("Opcao = ").strip()

    if op == "1":
        val = trap(expr, a, b, n)
    elif op == "2":
        val = simpson(expr, a, b, n)
    elif op == "3":
        val = midpoint(expr, a, b, n)
    else:
        print("Opcao invalida.")
        return

    print("Integral ~ {}".format(val))

    # modo improprio opcional: repetir com delta
    extra = input("Teste improprio (integral perto de singularidade)? (s/n) ").strip().lower()
    if extra == "s":
        print("Assume integral em [delta, b] (ou [a, b-delta] se singular no fim).")
        lado = input("Singular em a (1) ou em b (2)? ").strip()
        k = int(input("Quantos deltas testar? (ex: 4) = ").strip())
        i = 0
        while i < k:
            d = float(input("delta_{} = ".format(i+1)))
            if lado == "1":
                val2 = trap(expr, a + d, b, n)
                print("I(delta) ~ {}".format(val2))
            else:
                val2 = trap(expr, a, b - d, n)
                print("I(delta) ~ {}".format(val2))
            i += 1

main()
