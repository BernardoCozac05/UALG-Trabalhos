from math import sqrt, erf

# Função de distribuição acumulada normal padrão
def normal_cdf(z):
    return 0.5 * (1 + erf(z / sqrt(2)))

# Entradas dos dados populacionais e amostrais
print("=== Parâmetros da população ===")
mu = float(input("Média populacional (μ): "))
sigma = float(input("Desvio padrão populacional (σ): "))
n = int(input("Tamanho da amostra (n): "))

# Cálculo do erro padrão
se = sigma / sqrt(n)

# Menu de opções
print("\n=== Tipo de probabilidade desejada ===")
print("1 - P(média ≤ valor)")
print("2 - P(média ≥ valor)")
print("3 - P(valor1 ≤ média ≤ valor2)")
opcao = int(input("Escolha (1/2/3): "))

# Cálculo baseado na escolha
if opcao == 1:
    limite = float(input("Digite o limite superior: "))
    z = (limite - mu) / se
    p = normal_cdf(z)
    print("\nResultado: P(média ≤", limite, ") =", round(p, 6))

elif opcao == 2:
    limite = float(input("Digite o limite inferior: "))
    z = (limite - mu) / se
    p = 1 - normal_cdf(z)
    print("\nResultado: P(média ≥", limite, ") =", round(p, 6))

elif opcao == 3:
    a = float(input("Digite o limite inferior: "))
    b = float(input("Digite o limite superior: "))
    z1 = (a - mu) / se
    z2 = (b - mu) / se
    p = normal_cdf(z2) - normal_cdf(z1)
    print("\nResultado: P(", a, "≤ média ≤", b, ") =", round(p, 6))

else:
    print("Opção inválida.")
