# P2.2 — Tabela de derivadas via integrais (padrao comum em fichas/exames)
# Tudo por INPUT.
#
# Usa as aproximacoes do TRAPEZIO:
#  1) f(x2)-f(x1) ≈ (h/2)(f'(x1)+f'(x2))
#  2) f'(x2)-f'(x1) ≈ (h/2)(f''(x1)+f''(x2))
#  3) f(x1)-f(x0) ≈ (h/2)(f'(x0)+f'(x1))
#  4) f'(x1)-f'(x0) ≈ (h/2)(f''(x0)+f''(x1))

def main():
    print("P2.2 — Tabela derivadas (trapézio)")
    h = float(input("h = "))

    f0 = float(input("f(x0) = "))
    f1 = float(input("f(x1) = "))
    f2 = float(input("f(x2) = "))

    fpp1 = float(input("f''(x1) = "))
    fpp2 = float(input("f''(x2) = "))

    # (1): soma de derivadas
    sum_p = 2.0*(f2 - f1)/h
    # (2): diferença de derivadas
    diff_p = (h/2.0)*(fpp1 + fpp2)

    fp1 = (sum_p - diff_p)/2.0
    fp2 = (sum_p + diff_p)/2.0

    # (3): obter f'(x0)
    sum_p01 = 2.0*(f1 - f0)/h
    fp0 = sum_p01 - fp1

    # (4): obter f''(x0)
    fpp0 = (2.0*(fp1 - fp0)/h) - fpp1

    print("")
    print("Resultados:")
    print("f'(x0)  =", fp0)
    print("f'(x1)  =", fp1)
    print("f'(x2)  =", fp2)
    print("f''(x0) =", fpp0)

main()
