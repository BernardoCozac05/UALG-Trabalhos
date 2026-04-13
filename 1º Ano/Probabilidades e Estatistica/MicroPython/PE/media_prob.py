import math

# ================================
#  Funções estatísticas auxiliares
# ================================

def _erf_approx(x):
    """Aproximação numérica da função erro (erf) usando apenas `math`.

    Fórmula de Abramowitz & Stegun 7.1.26 (erro máx. ≈ 1,5 × 10⁻⁷).
    Compatível com MicroPython 1.9.4 (fx‑CG50).
    """
    sign = 1 if x >= 0 else -1
    x = abs(x)
    a1, a2, a3, a4, a5 = 0.254829592, -0.284496736, 1.421413741, -1.453152027, 1.061405429
    p = 0.3275911

    t = 1.0 / (1.0 + p * x)
    poly = (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t
    y = 1.0 - poly * math.exp(-x * x)
    return sign * y


def normal_cdf(z):
    """Distribuição acumulada normal padrao Φ(z)."""
    return 0.5 * (1.0 + _erf_approx(z / math.sqrt(2.0)))


# =====================================================
#  Aplicação interativa para probabilidades da média amostral
# =====================================================

def main():
    print("=== Parâmetros da população ===")
    try:
        mu = float(input("u: "))
        sigma = float(input("DP: "))
        n = int(input("n: "))
    except ValueError:
        print("Entrada invalida.")
        return

    se = sigma / math.sqrt(n)  # erro‑padrão da média

    print("\n=== Tipo de probabilidade desejada ===")
    print("1 - P(media ≤ valor)")
    print("2 - P(media ≥ valor)")
    print("3 - P(valor1 ≤ media ≤ valor2)")
    try:
        opcao = int(input("Escolha (1/2/3): "))
    except ValueError:
        print("Entrada invalida.")
        return

    if opcao == 1:
        limite = float(input("limite superior: "))
        z = (limite - mu) / se
        p = normal_cdf(z)
        print("\nResultado: P(media ≤ {0}) = {1:.6f}".format(limite, p))

    elif opcao == 2:
        limite = float(input("limite inferior: "))
        z = (limite - mu) / se
        p = 1.0 - normal_cdf(z)
        print("\nResultado: P(media ≥ {0}) = {1:.6f}".format(limite, p))

    elif opcao == 3:
        a = float(input("limite inferior: "))
        b = float(input("limite superior: "))
        if b < a:
            a, b = b, a  # garante a ≤ b
        z1 = (a - mu) / se
        z2 = (b - mu) / se
        p = normal_cdf(z2) - normal_cdf(z1)
        print("\nResultado: P({0} ≤ media ≤ {1}) = {2:.6f}".format(a, b, p))
    else:
        print("Opção invalida.")


# =====================================================
#  Auto‑execução
#  ---------------------------------------------------
#  A fx‑CG50 executa o arquivo como script quando você
#  pressiona F6 (RUN). A chamada a seguir garante que o
#  programa também rode se o módulo for importado.
# =====================================================

main()
