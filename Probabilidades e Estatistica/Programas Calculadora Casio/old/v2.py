import math

# Função que devolve valores críticos de chi² da tabela
def chi2_tabela(df, alpha):
    # dicionário com valores de chi² (df: {alpha: valor})
    tabela = {
        19: {
            0.005: 38.582,
            0.01: 36.191,
            0.025: 33.408,
            0.05: 30.144,
            0.95: 10.117,
            0.975: 8.907,
            0.99: 6.844,
            0.995: 5.892
        },
        20: {
            0.005: 40.289,
            0.01: 37.566,
            0.025: 34.170,
            0.05: 31.410,
            0.95: 10.851,
            0.975: 9.591,
            0.99: 7.434,
            0.995: 6.265
        },
        # adiciona mais df se quiseres
    }

    # Se df não existir, dá erro
    if df in tabela and alpha in tabela[df]:
        return tabela[df][alpha]
    else:
        print("Valor de chi² não disponível para df =", df, "e alpha =", alpha)
        return -1

def intervalo_confianca_variancia(n, s2, tipo, parametro, alpha):
    df = n - 1

    if tipo == "B":
        chi_inf = chi2_tabela(df, 1 - alpha / 2)
        chi_sup = chi2_tabela(df, alpha / 2)
        li = (df * s2) / chi_sup
        ls = (df * s2) / chi_inf

    elif tipo == "US":
        chi = chi2_tabela(df, alpha)
        li = 0
        ls = (df * s2) / chi

    elif tipo == "UI":
        chi = chi2_tabela(df, 1 - alpha)
        li = (df * s2) / chi
        ls = 999999

    else:
        print("Tipo inválido. Use: B, US ou UI.")
        return 0, 0

    if parametro == "DP":
        li = math.sqrt(li)
        ls = math.sqrt(ls)

    return li, ls

# --- Entrada reorganizada ---
print("Intervalo de Confiança para σ² ou σ")

print("Tipo de intervalo:")
print(" B  - Bilateral")
print(" US - Unilateral superior")
print(" UI - Unilateral inferior")
tipo = input("Escolha (B / US / UI): ")

print("Parâmetro a estimar:")
print(" V  - Variância")
print(" DP - Desvio padrão")
parametro = input("Escolha (V / DP): ")

print("Dados da amostra:")
n = int(input("Tamanho da amostra (n): "))
s2 = float(input("Variância amostral (s²): "))
alpha = float(input("Nível de significância (ex: 0.01 para 99%): "))

# --- Cálculo ---
li, ls = intervalo_confianca_variancia(n, s2, tipo, parametro, alpha)

# --- Saída ---
if parametro == "V":
    nome = "σ²"
else:
    nome = "σ"

print("Intervalo de confiança para", nome)
if ls == 999999:
    print("(", li, ", infinito )")
elif li == 0:
    print("( 0 ,", ls, ")")
else:
    print("(", li, ",", ls, ")")
