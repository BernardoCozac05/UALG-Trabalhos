# TRABALHO 2 - Método da Secante
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

from sympy import *  # type: ignore

MAX_ITERATIONS = 10
MAX_RELATIVE_TOLERANCE = 10 ** (-9)

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
        raise ArithmeticError(f"f({where}) é indefinida/não real (NaN/∞/complexo). Tente outros palpites.")

def solve(f, r_minus1: float, r0: float, x):
    for _ in range(MAX_ITERATIONS):
        fr_minus1 = f.evalf(subs={x: r_minus1})
        fr0 = f.evalf(subs={x: r0})

        _ensure_real_finite(fr_minus1, "r-1")
        _ensure_real_finite(fr0, "r0")

        denom = (fr0 - fr_minus1)
        if denom == 0:
            raise ZeroDivisionError("f(r0) - f(r-1) = 0; método da secante não pode prosseguir.")

        r = float(r0 - fr0 * (r0 - r_minus1) / denom)

        denom_rt = r if r != 0 else 1.0
        relative_tolerance = abs((r - r0) / denom_rt)
        if relative_tolerance < MAX_RELATIVE_TOLERANCE:
            break

        fr = f.evalf(subs={x: r})
        _ensure_real_finite(fr, "r")

        if fr == 0:
            break

        r_minus1 = r0
        r0 = r
    else:
        raise ArithmeticError(f"Não convergiu em {MAX_ITERATIONS} iterações; pode não existir raiz próxima dos palpites dados.")

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
                r_1 = float(sympify(input("Introduza uma aproximação inicial de x (r-1): ")))
                r0 = float(sympify(input("Introduza uma aproximação inicial de x (r0): ")))
            except ValueError:
                print("As aproximações iniciais têm de ser números reais.")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

            try:
                raiz = solve(expression, r_1, r0, x)
                print(f"Raiz aproximada: {raiz}")
            except (ZeroDivisionError, ArithmeticError, ValueError) as e:
                print(f"Erro: {e}")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

        except KeyboardInterrupt:
            print("\nInterrompido pelo utilizador.")
            break

        if not yes("Pretende continuar com este método? (Y/N): "):
            break
