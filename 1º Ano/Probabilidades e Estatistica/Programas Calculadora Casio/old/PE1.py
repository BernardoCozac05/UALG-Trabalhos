import scipy.stats as stats

# --- DADOS DE TESTE ---
n = 20           # tamanho da amostra
s2 = 4.5         # variância amostral (exemplo)
alpha = 0.01     # para 99% de confiança

# --- CÁLCULO ---
df = n - 1
chi2_lower = stats.chi2.ppf(1 - alpha/2, df)
chi2_upper = stats.chi2.ppf(alpha/2, df)

limite_inferior = (df * s2) / chi2_lower
limite_superior = (df * s2) / chi2_upper

print(f"Intervalo de confiança a 99% para σ²: ({limite_inferior:.4f}, {limite_superior:.4f})")
