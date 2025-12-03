# TRABALHO 2 - Método da Bisseção
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

from sympy import *  # type: ignore

MAX_ITERATIONS = 20
MAX_RELATIVE_TOLERANCE = 10 ** (-5)

def yes(prompt: str) -> bool:
    ans = input(prompt).strip().lower()
    return ans == "y"

# Validar se a expressão tem raízes reais
def vetrr(f, x):
    try:
        sol = solveset(f, x, domain=S.Reals)
        if sol is S.EmptySet:
            raise ValueError("A função não tem raízes reais (solveset devolveu conjunto vazio).")
    except Exception:
        pass

def _ensure_real_finite(val, where: str):
    if (hasattr(val, "is_real") and val.is_real is False) or (hasattr(val, "is_finite") and val.is_finite is False) or val.has(zoo, oo, -oo, nan):
        raise ArithmeticError(f"f({where}) é indefinida/não real (NaN/∞/complexo). Tente outro intervalo.")

def bolzanoTheorem(f, a: float, b: float, x):
    fa = f.evalf(subs={x: a})
    fb = f.evalf(subs={x: b})
    _ensure_real_finite(fa, "a")
    _ensure_real_finite(fb, "b")
    if fa == 0 or fb == 0:
        return True
    return fa * fb < 0

def solve(f, a: float, b: float, x):
    if a > b:
        raise ValueError("O limite inferior tem de ser menor ou igual ao superior.")
    if not bolzanoTheorem(f, a, b, x):
        raise ValueError("O intervalo [a,b] não confirma a existência de uma raiz (não há mudança de sinal).")

    for _ in range(MAX_ITERATIONS):
        r = (a + b) / 2
        denom = r if r != 0 else 1.0
        relative_tolerance = abs((r - a) / denom)
        if relative_tolerance < MAX_RELATIVE_TOLERANCE:
            break

        fa = f.evalf(subs={x: a})
        fb = f.evalf(subs={x: b})
        fr = f.evalf(subs={x: r})
        _ensure_real_finite(fa, "a")
        _ensure_real_finite(fb, "b")
        _ensure_real_finite(fr, "r")

        if fr == 0:
            break

        if fa * fr < 0:
            b = r
        else:
            a = r

    return float(r)

def userInput():
    while True:
        try:
            x = symbols("x")
            expr_str = input("Introduza a função (f(x)): ").strip()
            try:
                expression = sympify(expr_str, evaluate=True)
            except SympifyError:
                print("Expressão inválida. Exemplos válidos: sin(x) - x/2, x**2 - pi.")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

            try:
                vetrr(expression, x)
            except ValueError as e:
                print(f"Erro: {e}")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

            try:
                a = float(sympify(input("Introduza o limite inferior de x (a): ")))
                b = float(sympify(input("Introduza o limite superior de x (b): ")))
            except ValueError:
                print("Os limites têm de ser números reais.")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

            try:
                raiz = solve(expression, a, b, x)
                print(f"Raiz aproximada: {raiz}")
            except (ValueError, ArithmeticError) as e:
                print(f"Erro: {e}")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

        except KeyboardInterrupt:
            print("\nInterrompido pelo utilizador.")
            break

        if not yes("Pretende continuar com este método? (Y/N): "):
            break
