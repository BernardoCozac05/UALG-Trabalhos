# TRABALHO 5 - Sistemas de Equações Não Lineares usando o Método de Newton
# João Andréz - 61066
# Carlos Ferreira - 71319
# Diogo Freitas - 90147
# Diogo Carvalho - 90247

import sympy as sp # type: ignore
import numpy as np # type: ignore


def norm_inf(vec: np.ndarray) -> float:
    """Infinity norm (NumPy)."""
    v = np.asarray(vec, dtype=float).reshape(-1)
    return float(np.max(np.abs(v)))


def is_finite_array(a: np.ndarray) -> bool:
    return bool(np.all(np.isfinite(a)))


def validate_symbols(expr: sp.Expr, allowed: set[sp.Symbol]) -> None:
    """Ensure expression only contains allowed symbols."""
    extra = set(expr.free_symbols) - allowed
    if extra:
        names = ", ".join(sorted(str(s) for s in extra))
        raise ValueError(f"A expressão contém símbolos não permitidos: {names}. Use apenas x0..x(n-1).")


def parse_numeric(expr_str: str, local_dict: dict) -> float:
    """Parse a numeric value from user input. Rejects symbols."""
    expr = sp.sympify(expr_str, locals=local_dict)
    if expr.free_symbols:
        raise ValueError("O valor inicial tem de ser numérico (não pode conter variáveis).")
    val = float(sp.N(expr))
    if not np.isfinite(val):
        raise ValueError("O valor tem de ser finito (não pode ser NaN/Inf).")
    return val


def newton_system(equations, vars, x0, max_iter: int, tol_abs: float):
    """
    Método de Newton para sistemas não lineares F(x)=0.

    Requisitos (professor):
      - o utilizador fornece x^(0) (k=0)
      - pedir N0 (máx. iterações) e TOL (tolerância absoluta) na norma infinito
      - critério de paragem: ||y||_∞ < TOL, onde y é o incremento na iteração
    """
    n = len(vars)
    if len(equations) != n:
        raise ValueError(f"Esperava {n} equações, mas recebeu {len(equations)}.")
    if len(x0) != n:
        raise ValueError(f"Esperava {n} valores em x^(0), mas recebeu {len(x0)}.")
    if max_iter <= 0:
        raise ValueError("O número máximo de iterações (N0) tem de ser positivo.")
    if tol_abs <= 0 or not np.isfinite(tol_abs):
        raise ValueError("A tolerância (TOL) tem de ser positiva e finita.")

    # Construção simbólica
    F = sp.Matrix(equations)            # (n x 1)
    J = F.jacobian(vars)                # (n x n)

    # Compilação numérica (eficiente)
    F_func = sp.lambdify(vars, F, modules="numpy")
    J_func = sp.lambdify(vars, J, modules="numpy")

    # x^(0)
    x = np.array(x0, dtype=float).reshape(n)

    # Output legível
    def fmt_sci(v: float, w: int = 12) -> str:
        return f"{v:{w}.3e}"

    def fmt_val(v: float, w: int = 12) -> str:
        return f"{v:{w}.7f}"

    x_headers = [f"x{i}" for i in range(n)]
    header = (
        f"{'k':>3} | " +
        " | ".join(f"{h:>12}" for h in x_headers) +
        f" | {'||x^k-x^(k-1)||∞':>18}"
    )
    print("\n" + header)
    print("-" * len(header))

    # Linha k=0 (x^0). Não existe ||x^0-x^-1||.
    print(
        f"{0:>3} | " +
        " | ".join(fmt_val(v) for v in x) +
        f" | {'-':>18}"
    )

    # Algoritmo: k = 1 ... N0
    for k in range(1, max_iter + 1):
        # Calcular F(x) e J(x) em x = x^(k-1)
        Fx = np.array(F_func(*x), dtype=float).reshape(n)
        Jx = np.array(J_func(*x), dtype=float).reshape(n, n)

        if not is_finite_array(Fx) or not is_finite_array(Jx):
            raise RuntimeError(f"Na iteração k={k}, F(x) ou J(x) contém NaN/Inf.")

        # Resolver J(x) y = -F(x)
        try:
            y = np.linalg.solve(Jx, -Fx).reshape(n)
        except np.linalg.LinAlgError as e:
            raise RuntimeError(
                f"Falha a resolver J(x) y = -F(x) na iteração k={k} "
                f"(Jacobiano singular/mal condicionado). Detalhe: {e}"
            )

        if not is_finite_array(y):
            raise RuntimeError(f"Na iteração k={k}, o incremento y contém NaN/Inf.")

        # Atualizar: x = x + y  -> x^(k)
        x_new = x + y
        if not is_finite_array(x_new):
            raise RuntimeError(f"Na iteração k={k}, x^(k) contém NaN/Inf após atualização.")

        # Critério de paragem: ||y||_∞ < TOL
        y_inf = norm_inf(y)

        # Imprimir linha k
        print(
            f"{k:>3} | " +
            " | ".join(fmt_val(v) for v in x_new) +
            f" | {fmt_sci(y_inf, 18)}"
        )

        if y_inf < tol_abs:
            print("-" * len(header))
            print(f"Convergência atingida em k={k} (||y||_∞ < TOL).")
            return x_new, k

        x = x_new

    print("-" * len(header))
    print(f"O método falhou após N0={max_iter} iterações (critério ||y||_∞ < TOL não foi satisfeito).")
    return x, max_iter


def main():
    while True:
        try:
            n = int(input("Número de equações/incógnitas (n): ").strip())
            if n <= 0:
                raise ValueError("n tem de ser positivo.")
        except Exception as e:
            print(f"Entrada inválida para n. Detalhe: {e}")
            continue

        vars = sp.symbols("x0:" + str(n))
        allowed = set(vars)

        local_dict = {str(v): v for v in vars}
        local_dict.update({"pi": sp.pi, "E": sp.E})

        print(f"\nIntroduza {n} equações (use x0, x1, ..., x{n-1}).")
        equations = []
        for i in range(n):
            while True:
                s = input(f"Equação {i+1}: ").strip()
                try:
                    expr = sp.sympify(s, locals=local_dict)
                    validate_symbols(expr, allowed)
                    equations.append(expr)
                    break
                except Exception as e:
                    print(f"Equação inválida. Tente novamente. Detalhe: {e}")

        print("\nAproximação inicial x^(0) (k=0):")
        x0 = []
        for i in range(n):
            while True:
                s = input(f"x{i}(0): ").strip()
                try:
                    x0.append(parse_numeric(s, local_dict))
                    break
                except Exception as e:
                    print(f"Valor inválido. Tente novamente. Detalhe: {e}")

        while True:
            try:
                max_iter = int(input("\nN0 (número máximo de iterações): ").strip())
                if max_iter <= 0:
                    raise ValueError("N0 tem de ser positivo.")
                break
            except Exception as e:
                print(f"Entrada inválida para N0. Detalhe: {e}")

        while True:
            try:
                tol_abs = float(sp.N(sp.sympify(
                    input("TOL (tolerância absoluta, norma infinito): ").strip(),
                    locals=local_dict
                )))
                if tol_abs <= 0 or not np.isfinite(tol_abs):
                    raise ValueError("TOL tem de ser positiva e finita.")
                break
            except Exception as e:
                print(f"Entrada inválida para TOL. Detalhe: {e}")

        try:
            x_sol, k_final = newton_system(equations, vars, x0, max_iter, tol_abs)
            print("\nSolução aproximada:")
            for i, v in enumerate(x_sol):
                print(f"  x{i} ≈ {v:.12g}")
        except Exception as e:
            print(f"\nErro durante a execução: {e}")

        again = input("\nDeseja resolver outro sistema? (y/n): ").strip().lower()
        if again != "y":
            print("Programa terminado.")
            break
        print("\n" + "=" * 70 + "\n")


if __name__ == "__main__":
    main()
