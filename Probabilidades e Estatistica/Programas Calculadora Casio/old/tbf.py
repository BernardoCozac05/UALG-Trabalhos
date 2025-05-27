
from math import sqrt

def ler_valor(valor):
    if "/" in valor:
        partes = valor.split("/")
        return float(partes[0]) / float(partes[1])
    else:
        return float(valor)

# Leitura dos valores possíveis
print("Digite os valores possíveis de X1 (separados por espaço):")
valores_x1 = [int(x) for x in input().split()]
print("Digite os valores possíveis de X2 (separados por espaço):")
valores_x2 = [int(x) for x in input().split()]

nx = len(valores_x1)
ny = len(valores_x2)

# Tabela P(X1, X2)
print("Digite a tabela de P(X1, X2) linha por linha (frações ou floats):")
P = []
for i in range(nx):
    linha = input(f"P(X1={valores_x1[i]}): ").split()
    P.append([ler_valor(v) for v in linha])

# Menu
print("\nEscolha o que deseja calcular:")
print("1 - Probabilidade com X1 (≥, ≤, =, intervalo)")
print("2 - Esperança e Variância de X1 ou X2")
print("3 - Esperança condicional")
print("4 - cov(X1, X2)")
op = int(input("Opção (1-4): "))

# Opção 1: Probabilidade com X1
if op == 1:
    print("Escolha o tipo:")
    print(" a - P(X1 ≥ k)")
    print(" b - P(X1 ≤ k)")
    print(" c - P(X1 = k)")
    print(" d - P(a ≤ X1 ≤ b)")
    tipo = input("Tipo (a-d): ")

    if tipo == "a":
        k = int(input("k: "))
        prob = sum(P[i][j] for i in range(nx) if valores_x1[i] >= k for j in range(ny))
        print(f"P(X1 ≥ {k}) = {round(prob, 6)}")

    elif tipo == "b":
        k = int(input("k: "))
        prob = sum(P[i][j] for i in range(nx) if valores_x1[i] <= k for j in range(ny))
        print(f"P(X1 ≤ {k}) = {round(prob, 6)}")

    elif tipo == "c":
        k = int(input("k: "))
        if k not in valores_x1:
            print("Valor inválido.")
        else:
            i = valores_x1.index(k)
            prob = sum(P[i][j] for j in range(ny))
            print(f"P(X1 = {k}) = {round(prob, 6)}")

    elif tipo == "d":
        a = int(input("a: "))
        b = int(input("b: "))
        prob = sum(P[i][j] for i in range(nx) if a <= valores_x1[i] <= b for j in range(ny))
        print(f"P({a} ≤ X1 ≤ {b}) = {round(prob, 6)}")

    else:
        print("Tipo inválido.")

# Opção 2: Esperança e variância
elif op == 2:
    print("Escolha a variável: 1 para X1, 2 para X2")
    var = int(input("Variável: "))
    E = 0
    E_quad = 0
    if var == 1:
        for i in range(nx):
            for j in range(ny):
                p = P[i][j]
                E += valores_x1[i] * p
                E_quad += valores_x1[i]**2 * p
        V = E_quad - E**2
        print("E[X1] =", round(E, 6))
        print("V[X1] =", round(V, 6))
    elif var == 2:
        for i in range(nx):
            for j in range(ny):
                p = P[i][j]
                E += valores_x2[j] * p
                E_quad += valores_x2[j]**2 * p
        V = E_quad - E**2
        print("E[X2] =", round(E, 6))
        print("V[X2] =", round(V, 6))
    else:
        print("Variável inválida.")

# Opção 3: Esperança condicional
elif op == 3:
    print("Escolha:")
    print(" a - E[X1 | X2 = y]")
    print(" b - E[X2 | X1 = x]")
    esc = input("Tipo (a/b): ")

    if esc == "a":
        y = int(input("y = "))
        if y not in valores_x2:
            print("Valor inválido.")
        else:
            j = valores_x2.index(y)
            total = sum(P[i][j] for i in range(nx))
            E = sum(valores_x1[i] * (P[i][j] / total) for i in range(nx))
            print(f"E[X1 | X2 = {y}] =", round(E, 6))

    elif esc == "b":
        x = int(input("x = "))
        if x not in valores_x1:
            print("Valor inválido.")
        else:
            i = valores_x1.index(x)
            total = sum(P[i][j] for j in range(ny))
            E = sum(valores_x2[j] * (P[i][j] / total) for j in range(ny))
            print(f"E[X2 | X1 = {x}] =", round(E, 6))
    else:
        print("Tipo inválido.")

# Opção 4: cov(X1, X2)
elif op == 4:
    E_X1 = E_X2 = E_X1X2 = 0
    for i in range(nx):
        for j in range(ny):
            p = P[i][j]
            E_X1 += valores_x1[i] * p
            E_X2 += valores_x2[j] * p
            E_X1X2 += valores_x1[i] * valores_x2[j] * p
    cov = E_X1X2 - E_X1 * E_X2
    print("cov(X1, X2) =", round(cov, 6))

else:
    print("Opção inválida.")
