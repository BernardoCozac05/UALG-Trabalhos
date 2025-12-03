"""Método de Newton — versão simples e limpa.

Uso programático:
    from newton import solve
    r = solve(f, r0, x)

Notas:
- Convergência rápida mas requer boa aproximação inicial e derivada não nula.
- Critério de paragem: tolerância relativa em r OU |f(r)| muito pequeno.
"""
from __future__ import annotations
from typing import Any
import math
from sympy import Expr, diff, lambdify

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
    r0: float,
    x,
    *,
    tol: float = DEFAULT_TOL,
    max_iter: int = DEFAULT_MAX_ITER,
) -> float:
    """Aproxima uma raiz real de f a partir de r0 pelo método de Newton.

    Raises ValueError se a derivada for 0 ou avaliações não forem reais/infinitas.
    """
    # Preparar funções numéricas (rápidas) para f e f'
    df = diff(f, x)
    f_num = lambdify(x, f, "math")
    df_num = lambdify(x, df, "math")

    r = float(r0)
    for _ in range(max_iter):
        fr = f_num(r)
        dfr = df_num(r)
        if not (_is_finite_real(fr) and _is_finite_real(dfr)):
            raise ValueError("f(r) ou f'(r) não são reais/finitos.")
        if abs(float(dfr)) == 0.0:
            raise ValueError("Derivada nula em r — o método de Newton falha.")

        r_new = r - float(fr) / float(dfr)

        # Paragens: passo relativo pequeno ou resíduo muito pequeno
        if abs(r_new - r) <= tol * max(1.0, abs(r_new)):
            r = r_new
            break
        if abs(float(f_num(r_new))) <= 1e-14:
            r = r_new
            break

        r = r_new

    return r
