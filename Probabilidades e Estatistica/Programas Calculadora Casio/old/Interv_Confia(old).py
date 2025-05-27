import scipy.stats as stats
import math

def intervalo_confianca_variancia(n, s2, alpha, tipo, parametro):
    df = n - 1

    if tipo == "B":
        chi2_inf = stats.chi2.ppf(1 - alpha/2, df)
        chi2_sup = stats.chi2.ppf(alpha/2, df)
        li = (df * s2) / chi2_inf
        ls = (df * s2) / chi2_sup

    elif tipo == "US":
        chi2 = stats.chi2.ppf(alpha, df)
        li = 0
        ls = (df * s2) / chi2

    elif tipo == "UI":
        chi2 = stats.chi2.ppf(1 - alpha, df)
        li = (df * s2) / chi2
        ls = float('inf')

    else:
        raise ValueError("Tipo inválido. Use: B, US ou UI.")

    if parametro == "DP":
        li = math.sqrt(li)
        ls = math.sqrt(ls)

    return li, ls

# --- Entrada reorganizada ---
print("🔍 Intervalo de Confiança para σ² ou σ\n")

print("Tipo de intervalo:")
print(" B  - Bilateral")
print(" US - Unilateral superior")
print(" UI - Unilateral inferior")
tipo = input("Escolha (B / US / UI): ").strip().upper()

print("\nParâmetro a estimar:")
print(" V  - Variância (σ²)")
print(" DP - Desvio padrão (σ)")
parametro = input("Escolha (V / DP): ").strip().upper()

print("\n--- Dados da amostra ---")
n = int(input("Tamanho da amostra (n): "))
s2 = float(input("Variância amostral (s²): "))
alpha = float(input("Nível de significância (ex: 0.01 para 99%): "))

# --- Cálculo ---
li, ls = intervalo_confianca_variancia(n, s2, alpha, tipo, parametro)

# --- Saída ---
parametro_nome = "σ²" if parametro == "V" else "σ"
conf = (1 - alpha) * 100

print(f"\n✅ Intervalo de confiança ({conf:.1f}%) para {parametro_nome}:")
if math.isinf(ls):
    print(f"({li:.4f}, ∞)")
elif li == 0:
    print(f"(0, {ls:.4f})")
else:
    print(f"({li:.4f}, {ls:.4f})")
