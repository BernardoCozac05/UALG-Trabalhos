from math import sqrt, erf

# Função de distribuição acumulada normal padrão
def normal_cdf(z):
    return 0.5 * (1 + erf(z / sqrt(2)))

# Z crítico por alpha
def z_critico(alpha, tipo):
    if tipo == 1 or tipo == 2:  # Unilateral
        if alpha == 0.10:
            return 1.28
        elif alpha == 0.05:
            return 1.645
        elif alpha == 0.01:
            return 2.33
    elif tipo == 3:  # Bilateral
        if alpha == 0.10:
            return 1.645
        elif alpha == 0.05:
            return 1.96
        elif alpha == 0.01:
            return 2.576
    return -1  # Erro

# Entrada de dados
print("Tipo de teste:")
print("1 - UD (H1: mu > mu0)")
print("2 - UE (H1: mu < mu0)")
print("3 - B (H1: mu ≠ mu0)")
tipo = int(input("Tipo (1/2/3): "))

mu0 = float(input("MedH0: "))
mu1 = float(input("Medreal: "))
sigma = float(input("Dp: "))
n = int(input("n: "))
alpha = float(input("Alpha: "))

se = sigma / sqrt(n)
z_alpha = z_critico(alpha, tipo)

if z_alpha == -1:
    print("Alpha inválido.")
    exit()

# Cálculo conforme o tipo de teste
if tipo == 1:
    lim = mu0 + z_alpha * se
    z = (lim - mu1) / se
    power = 1 - normal_cdf(z)
elif tipo == 2:
    lim = mu0 - z_alpha * se
    z = (lim - mu1) / se
    power = normal_cdf(z)
elif tipo == 3:
    lim_inf = mu0 - z_alpha * se
    lim_sup = mu0 + z_alpha * se
    z1 = (lim_inf - mu1) / se
    z2 = (lim_sup - mu1) / se
    power = normal_cdf(z1) + (1 - normal_cdf(z2))
else:
    print("Tipo inválido.")
    exit()

# Resultado
print("----- RESULTADOS -----")
print("PrejH0: ", round(power, 6))
