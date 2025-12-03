"""Método da Secante — versão simples e limpa.

Uso programático:
    from secant import solve
    r = solve(f, r_prev, r0, x)

Notas:
- Não requer derivada; usa dois palpites.
- Critério de paragem: tolerância relativa em r OU |f(r)| muito pequeno.
"""
from __future__ import annotations
from typing import Any
import math
from sympy import Expr, lambdify

DEFAULT_TOL = 1e-9
DEFAULT_MAX_ITER = 20


def _is_finite_real(value: Any) -> bool:
    try:
        v = float(value)
        return math.isfinite(v)
    except Exception:
        return False


def solve(
    f: Expr,
    r_prev: float,
    r0: float,
    x,
    *,
    tol: float = DEFAULT_TOL,
    max_iter: int = DEFAULT_MAX_ITER,
) -> float:
    """Aproxima uma raiz real de f usando a secante com dois palpites iniciais."""
    f_num = lambdify(x, f, "math")

    a, b = float(r_prev), float(r0)
    fa = f_num(a)
    fb = f_num(b)
    if not (_is_finite_real(fa) and _is_finite_real(fb)):
        raise ValueError("f(r_prev) e f(r0) devem ser reais e finitos.")

    for _ in range(max_iter):
        denom = float(fb) - float(fa)
        if denom == 0.0:
            raise ValueError("Denominador zero na fórmula da secante.")

        r = b - float(fb) * (b - a) / denom
        if abs(r - b) <= tol * max(1.0, abs(r)):
            b = r
            break

        fr = f_num(r)
        if not _is_finite_real(fr):
            raise ValueError("f(r) não é real/finito durante a secante.")
        if abs(float(fr)) <= 1e-14:
            b = r
            break

        # Deslizar a janela (a, b) ← (b, r)
        a, fa = b, fb
        b, fb = r, fr

    return b
