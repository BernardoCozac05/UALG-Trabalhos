# Q6 — Interpolacao de Lagrange (grau <= 1)
# MicroPython 1.9.4 (fx-CG50) — tudo por INPUT.
#
# Opcoes:
# 1) Modo prova/simbolos: imprime as formulas equivalentes (Lagrange e forma ax+b)
# 2) Modo numerico: le (x0,y0),(x1,y1) e:
#    - calcula P1(x) num ponto x
#    - e/ou devolve a reta na forma y = a x + b

def _to_num(s):
    # aceita "3/4" ou "2.5" ou "-1"
    s = s.strip()
    if "/" in s:
        p = s.split("/")
        if len(p) != 2:
            raise ValueError("Frac invalida")
        return float(p[0].strip()) / float(p[1].strip())
    return float(s)

def P1(x, x0, y0, x1, y1):
    if x0 == x1:
        raise ValueError("x0 = x1")
    return y0*(x-x1)/(x0-x1) + y1*(x-x0)/(x1-x0)

def line_ab(x0, y0, x1, y1):
    if x0 == x1:
        raise ValueError("x0 = x1")
    a = (y1 - y0) / (x1 - x0)
    b = (x1*y0 - x0*y1) / (x1 - x0)
    return a, b

def _theory():
    print("")
    print("=== MODO PROVA / SIMBOLOS (Q6) ===")
    print("Dados: (x0,y0), (x1,y1). Grau <= 1.")
    print("")
    print("Forma de Lagrange (m=1):")
    print("  P1(x) = y0 * (x-x1)/(x0-x1) + y1 * (x-x0)/(x1-x0)")
    print("")
    print("Forma equivalente (reta):")
    print("  P1(x) = a x + b")
    print("  a = (y1 - y0)/(x1 - x0)")
    print("  b = (x1*y0 - x0*y1)/(x1 - x0)")
    print("")
    print("Observacao: ambas sao a mesma reta que passa pelos 2 pontos.")
    print("")

def main():
    print("Q6 — Lagrange (grau <= 1)")
    print("1 - Modo prova/simbolos (sem numeros)")
    print("2 - Modo numerico")
    mode = input("Opcao = ").strip()

    if mode == "1":
        _theory()
        return

    if mode != "2":
        print("Opcao invalida.")
        return

    print("Introduza valores (aceita fracao tipo 3/4):")
    x0 = _to_num(input("x0 = "))
    y0 = _to_num(input("y0 = "))
    x1 = _to_num(input("x1 = "))
    y1 = _to_num(input("y1 = "))

    print("")
    print("Escolha:")
    print("1 - Calcular P1(x) num x")
    print("2 - Devolver reta y = a x + b")
    print("3 - Ambos")
    op = input("Opcao = ").strip()

    try:
        if op == "1" or op == "3":
            x = _to_num(input("x = "))
            print("P1(x) = {}".format(P1(x, x0, y0, x1, y1)))

        if op == "2" or op == "3":
            a, b = line_ab(x0, y0, x1, y1)
            print("a = {}".format(a))
            print("b = {}".format(b))
            print("Reta: y = ({})*x + ({})".format(a, b))

        if op not in ("1","2","3"):
            print("Opcao invalida.")

    except Exception as e:
        print("Erro:", e)

main()
