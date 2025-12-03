# TRABALHO 2 - Método de Newton
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

def _validate_derivative(f, x):
    diff_f = f.diff(x)
    try:
        if simplify(diff_f) == 0:
            raise ValueError("A derivada é identicamente nula: método de Newton não é aplicável.")
    except Exception:
        pass
    return diff_f

def solve(f, r0: float, x):
    r = float(r0)
    diff_f = _validate_derivative(f, x)

    for _ in range(MAX_ITERATIONS):
        r_old = r
        fr_old = f.evalf(subs={x: r_old})
        diff_fr_old = diff_f.evalf(subs={x: r_old})

        if diff_fr_old == 0:
            raise ZeroDivisionError(f"Derivada nula em x = {r_old:.6g}; método de Newton não pode prosseguir.")
        if (hasattr(diff_fr_old, "is_finite") and diff_fr_old.is_finite is False) or (hasattr(diff_fr_old, "is_real") and diff_fr_old.is_real is False):
            raise ArithmeticError(f"Derivada indefinida/não real em x = {r_old:.6g}; tente outro palpite ou método.")

        r = float(r_old - (fr_old / diff_fr_old))

        denom = r if r != 0 else 1.0
        relative_tolerance = abs((r - r_old) / denom)
        if relative_tolerance < MAX_RELATIVE_TOLERANCE:
            break

        fr = f.evalf(subs={x: r})
        if fr == 0:
            break

    else:
        raise ArithmeticError(f"Não convergiu em {MAX_ITERATIONS} iterações; pode não existir raiz próxima do palpite dado.")

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
                r0 = float(sympify(input("Introduza uma aproximação inicial de x (r0): ")))
            except ValueError:
                print("A aproximação inicial tem de ser um número real.")
                if not yes("Pretende continuar com este método? (Y/N): "):
                    break
                continue

            try:
                raiz = solve(expression, r0, x)
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
