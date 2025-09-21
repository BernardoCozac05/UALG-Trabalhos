import math

# =====================================================
#  Funções estatísticas auxiliares (sem bibliotecas externas)
# =====================================================

def _erf_approx(x):
    """Aproximação da função erro (erf) usando apenas `math`.

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
    """Função distribuição acumulada da Normal padrão Φ(z)."""
    return 0.5 * (1.0 + _erf_approx(z / math.sqrt(2.0)))


# =====================================================
#  Funções específicas do teste de hipótese H0 × H1
# =====================================================

def _z_critical(alpha, tails):
    """Valor crítico z para α = 0.10, 0.05 ou 0.01.

    `tails` = 1 → teste unilateral; 2 → bilateral.
    Retorna None se α não for suportado.
    """
    table = {
        (0.10, 1): 1.2815515655446004,
        (0.05, 1): 1.6448536269514722,
        (0.01, 1): 2.3263478740408408,
        (0.10, 2): 1.6448536269514722,
        (0.05, 2): 1.959963984540054,
        (0.01, 2): 2.5758293035489004,
    }
    return table.get((alpha, tails))


# =====================================================
#  Aplicação interativa: cálculo do erro β e do poder 1‑β
# =====================================================

def main():
    print("Tipos de teste:")
    print("1 - Unilateral direita  (H1: μ > μ0)")
    print("2 - Unilateral esquerda (H1: μ < μ0)")
    print("3 - Bilateral          (H1: μ ≠ μ0)")
    try:
        tipo = int(input("Escolha o tipo (1/2/3): "))
    except ValueError:
        print("Entrada inválida.")
        return

    if tipo not in (1, 2, 3):
        print("Tipo de teste inválido.")
        return

    try:
        mu0   = float(input("Media sob H0 (μ0): "))
        mu1   = float(input("Media real(μ1): "))
        sigma = float(input("DP: "))
        n     = int(input("n: "))
        alpha = float(input("alpha [0.10/0.05/0.01]: "))
    except ValueError:
        print("Entrada inválida.")
        return

    tails = 1 if tipo in (1, 2) else 2
    z_alpha = _z_critical(alpha, tails)
    if z_alpha is None:
        print("α nao suportado. Use 0.10, 0.05 ou 0.01.")
        return

    se = sigma / math.sqrt(n)

    # Limiares e cálculo do poder conformes o tipo de teste
    if tipo == 1:  # UD
        lim = mu0 + z_alpha * se
        z_val = (lim - mu1) / se
        power = 1.0 - normal_cdf(z_val)
    elif tipo == 2:  # UE
        lim = mu0 - z_alpha * se
        z_val = (lim - mu1) / se
        power = normal_cdf(z_val)
    else:  # Bilateral
        lim_inf = mu0 - z_alpha * se
        lim_sup = mu0 + z_alpha * se
        z1 = (lim_inf - mu1) / se
        z2 = (lim_sup - mu1) / se
        power = normal_cdf(z1) + (1.0 - normal_cdf(z2))

    beta = 1.0 - power

    print("\n----- RESULTADOS -----")
    print("Erro Tipo II: {0:.6f}".format(beta))
    print("Poder do teste(este): {0:.6f}".format(power))


# =====================================================
#  Auto‑execução
#  ---------------------------------------------------
#  A fx‑CG50 executa o arquivo como script quando você
#  pressiona F6 (RUN). Esta chamada garante que o
#  programa também rode caso seja importado no SHELL.
# =====================================================

main()
