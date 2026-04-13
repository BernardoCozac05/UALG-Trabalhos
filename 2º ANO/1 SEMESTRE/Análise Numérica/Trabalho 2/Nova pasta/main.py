"""Interface de consola, simples e humana, para os métodos de raízes.

- Entrada da função em SymPy (ex.: sin(x) - x/2).
- Escolha do método e parâmetros mínimos.
- Mensagens claras e validações amistosas.
"""
from __future__ import annotations
import sys
from typing import Optional
from sympy import symbols, sympify, diff

import bissection
import newton
import secant


X = symbols("x")


def _ask(prompt: str, *, allow_blank: bool = False) -> str:
    while True:
        try:
            s = input(prompt).strip()
        except (EOFError, KeyboardInterrupt):
            print("\nA sair…")
            sys.exit(0)
        if s or allow_blank:
            return s
        print("Por favor, introduza algum valor.")


def _ask_float(prompt: str, *, default: Optional[float] = None) -> float:
    while True:
        s = _ask(prompt, allow_blank=default is not None)
        if s == "" and default is not None:
            return float(default)
        try:
            return float(s)
        except ValueError:
            print("Valor inválido. Tente novamente.")


def _suggest_newton_r0(expr, a: float, b: float) -> float:
    """Sugestão de r0 para Newton: escolhe a ou b se f* f'' > 0; senão, o meio."""
    f1_a = diff(expr, X).subs(X, a)
    f2_a = diff(expr, X, 2).subs(X, a)
    f1_b = diff(expr, X).subs(X, b)
    f2_b = diff(expr, X, 2).subs(X, b)
    try:
        if float(f1_a) * float(f2_a) > 0:
            return a
        if float(f1_b) * float(f2_b) > 0:
            return b
    except Exception:
        pass
    return (a + b) / 2


def _menu() -> str:
    print("\nEscolha um método:")
    print("  1) Bisseção")
    print("  2) Newton")
    print("  3) Secante")
    print("  q) Sair")
    return _ask("→ ")


def main() -> None:
    print("Bem‑vindo! Vamos procurar raízes reais de f(x).\n")
    print("Dica: pode usar funções SymPy, p.ex., sin(x), cos(x), exp(x), etc.")

    while True:
        choice = _menu().lower()
        if choice in {"q", "4"}:
            print("Até já!")
            break
        if choice not in {"1", "2", "3"}:
            print("Opção inválida. Escolha 1, 2, 3 ou q.")
            continue

        # Ler a função
        try:
            expr = sympify(_ask("Introduza a função f(x): "))
        except Exception as e:
            print(f"Expressão inválida ({e}). Tente novamente.")
            continue

        # Parâmetros comuns (têm predefinidos; Enter aceita o default)
        tol = _ask_float(
            f"Tolerância [Enter para padrão {newton.DEFAULT_TOL if choice!='1' else bissection.DEFAULT_TOL}]: ",
            default=(newton.DEFAULT_TOL if choice != "1" else bissection.DEFAULT_TOL),
        )
        max_iter = int(
            _ask_float(
                f"Máx. iterações [Enter para padrão {newton.DEFAULT_MAX_ITER if choice!='1' else bissection.DEFAULT_MAX_ITER}]: ",
                default=(newton.DEFAULT_MAX_ITER if choice != "1" else bissection.DEFAULT_MAX_ITER),
            )
        )

        try:
            if choice == "1":  # Bisseção
                a = _ask_float("Limite inferior a: ")
                b = _ask_float("Limite superior b: ")
                if a >= b:
                    print("Requer-se a < b.")
                    continue
                try:
                    r = bissection.solve(expr, a, b, X, tol=tol, max_iter=max_iter, ensure_bracket=True)
                except ValueError as ve:
                    print(f"Aviso: {ve}")
                    cont = _ask("Continuar mesmo assim com ensure_bracket=False? (Y/N): ")
                    if cont.strip().upper() != "Y":
                        continue
                    r = bissection.solve(expr, a, b, X, tol=tol, max_iter=max_iter, ensure_bracket=False)
                print(f"\n≈ Raiz pela Bisseção: {r}\n")

            elif choice == "2":  # Newton
                a = _ask_float("Intervalo (sugestão para r0) — a: ")
                b = _ask_float("Intervalo (sugestão para r0) — b: ")
                r0 = _ask_float(f"Palpite inicial r0 [Enter p/ sugestão { _suggest_newton_r0(expr, a, b) } ]: ",
                                default=_suggest_newton_r0(expr, a, b))
                r = newton.solve(expr, r0, X, tol=tol, max_iter=max_iter)
                print(f"\n≈ Raiz por Newton: {r}\n")

            else:  # "3" Secante
                r_prev = _ask_float("Palpite r_{-1}: ")
                r0 = _ask_float("Palpite r_0: ")
                r = secant.solve(expr, r_prev, r0, X, tol=tol, max_iter=max_iter)
                print(f"\n≈ Raiz pela Secante: {r}\n")

        except ValueError as e:
            print(f"Erro: {e}")
        except Exception as e:
            print(f"Ocorreu um erro inesperado: {e}")

if __name__ == "__main__":
    main()
