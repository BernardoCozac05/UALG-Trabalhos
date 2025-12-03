"""Método da Bisseção — versão simples e limpa.

Uso programático:
    from bissection import solve
    r = solve(f, a, b, x)  # tolerância e iterações têm valores por defeito

Notas:
- Requer mudança de sinal em [a, b] (a menos que `ensure_bracket=False`).
- Critério de paragem: largura do intervalo <= tol OU |f(r)| muito pequeno.
"""
from __future__ import annotations
from typing import Any
import math
from sympy import Expr, lambdify

DEFAULT_TOL = 1e-5
DEFAULT_MAX_ITER = 50


def _is_finite_real(value: Any) -> bool:
    try:
        v = float(value)
        return math.isfinite(v)
    except Exception:
        return False


def solve(
    f: Expr,
    a: float,
    b: float,
    x,
    *,
    tol: float = DEFAULT_TOL,
    max_iter: int = DEFAULT_MAX_ITER,
    ensure_bracket: bool = True,
) -> float:
    """Aproxima uma raiz real de f em [a, b] pelo método da bisseção.

    Args:
        f: expressão SymPy para f(x)
        a, b: limites do intervalo (a < b)
        x: símbolo da variável
        tol: tolerância absoluta alvo (largura do intervalo)
        max_iter: máximo de iterações
        ensure_bracket: se True, exige mudança de sinal em [a, b]

    Returns:
        Aproximação da raiz.

    Raises:
        ValueError: intervalo inválido, avaliações não reais/infinitas, ou sem mudança de sinal.
    """
    if a >= b:
        raise ValueError("Em bisseção, é necessário 'a < b'.")

    f_num = lambdify(x, f, "math")
    fa = f_num(a)
    fb = f_num(b)
    if not (_is_finite_real(fa) and _is_finite_real(fb)):
        raise ValueError("f(a) e f(b) devem ser reais e finitos.")

    # Exigir mudança de sinal, salvo se o utilizador desativar
    if ensure_bracket and float(fa) * float(fb) > 0.0:
        raise ValueError("Sem mudança de sinal em [a, b]. Escolha outro intervalo.")

    left, right = float(a), float(b)
    r = (left + right) / 2.0

    for _ in range(max_iter):
        r = (left + right) / 2.0
        fr = f_num(r)
        if not _is_finite_real(fr):
            raise ValueError("f(r) não é real/finito durante a bisseção.")

        # Paragens: intervalo suficientemente pequeno ou valor de f muito perto de 0
        if (right - left) / 2.0 <= tol or abs(float(fr)) <= 1e-12:
            break

        fa = f_num(left)
        # Manter o intervalo que contém a raiz
        if float(fa) * float(fr) <= 0.0:
            right = r
        else:
            left = r

    return r
