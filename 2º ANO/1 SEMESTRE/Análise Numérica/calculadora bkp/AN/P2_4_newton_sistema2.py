import math

# P2.4 — Newton para sistema 2x2: F(x1,x2)=0
# 1) Apenas a primeira iteracao (k=0 -> k=1)
# 2) Iterar ate tol/maxit
# Jacobiano numerico por diferencas finitas centrais.

def _mk_env(x1, x2):
    env = {"x1": x1, "x2": x2}
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

def F(expr, x1, x2):
    expr = expr.replace("^", "**")
    try:
        return eval(expr, {"__builtins__": None}, _mk_env(x1, x2))
    except:
        env = _mk_env(x1, x2)
        return eval(expr, env, env)

def jac_num(expr1, expr2, x1, x2, h):
    f1_x1p = F(expr1, x1 + h, x2)
    f1_x1m = F(expr1, x1 - h, x2)
    f1_x2p = F(expr1, x1, x2 + h)
    f1_x2m = F(expr1, x1, x2 - h)

    f2_x1p = F(expr2, x1 + h, x2)
    f2_x1m = F(expr2, x1 - h, x2)
    f2_x2p = F(expr2, x1, x2 + h)
    f2_x2m = F(expr2, x1, x2 - h)

    J11 = (f1_x1p - f1_x1m)/(2.0*h)
    J12 = (f1_x2p - f1_x2m)/(2.0*h)
    J21 = (f2_x1p - f2_x1m)/(2.0*h)
    J22 = (f2_x2p - f2_x2m)/(2.0*h)
    return J11, J12, J21, J22

def solve_2x2(a, b, c, d, e, f):
    det = a*d - b*c
    if det == 0:
        return None
    s1 = (e*d - b*f) / det
    s2 = (a*f - e*c) / det
    return s1, s2

def step(expr1, expr2, x1, x2, h):
    f1 = F(expr1, x1, x2)
    f2 = F(expr2, x1, x2)
    J11, J12, J21, J22 = jac_num(expr1, expr2, x1, x2, h)
    sol = solve_2x2(J11, J12, J21, J22, -f1, -f2)
    return sol, (f1, f2), (J11, J12, J21, J22)

def inf_norm2(a, b):
    aa = -a if a < 0 else a
    bb = -b if b < 0 else b
    return bb if bb > aa else aa

def iterate(expr1, expr2, x1, x2, tol, maxit, h):
    print("k   x1            x2            ||s||inf      F1           F2")
    k = 0
    while k < maxit:
        sol, Fv, _ = step(expr1, expr2, x1, x2, h)
        if sol is None:
            print("Parou: Jacobiano singular.")
            return x1, x2
        s1, s2 = sol
        ns = inf_norm2(s1, s2)
        f1, f2 = Fv
        print("{}  {}  {}  {}  {}  {}".format(k, x1, x2, ns, f1, f2))
        x1 += s1
        x2 += s2
        if ns < tol:
            print("STOP: ||s||inf < tol")
            return x1, x2
        k += 1
    print("maxit atingido")
    return x1, x2

def main():
    print("P2.4 — Newton (sistema 2x2)")
    expr1 = input("F1(x1,x2) = ").strip()
    expr2 = input("F2(x1,x2) = ").strip()
    x1 = float(input("x1(0) = ").strip().replace(",", "."))
    x2 = float(input("x2(0) = ").strip().replace(",", "."))
    h = float(input("h (ex: 1e-6) = ").strip().replace(",", "."))

    print("")
    print("1 - Apenas 1 iteracao (k=0 -> k=1)")
    print("2 - Iterar ate tol/maxit")
    op = input("Opcao = ").strip()

    if op == "1":
        sol, Fv, J = step(expr1, expr2, x1, x2, h)
        if sol is None:
            print("Jacobiano singular.")
            return
        s1, s2 = sol
        f1, f2 = Fv
        print("F(x0) = ({}, {})".format(f1, f2))
        print("s0 = ({}, {})".format(s1, s2))
        print("x1 = ({}, {})".format(x1 + s1, x2 + s2))
        return

    if op == "2":
        tol = float(input("tol = ").strip().replace(",", "."))
        maxit = int(input("maxit = ").strip())
        r1, r2 = iterate(expr1, expr2, x1, x2, tol, maxit, h)
        print("solucao ~ ({}, {})".format(r1, r2))
        return

    print("Opcao invalida.")

main()
