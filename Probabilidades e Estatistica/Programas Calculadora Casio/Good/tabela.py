# Programa compatível com a Casio fx-CG50 - frações manuais

def mdc(a, b):
    while b != 0:
        a, b = b, a % b
    return a

def simplificar(n, d):
    g = mdc(n, d)
    return n // g, d // g

def somar(a1, b1, a2, b2):
    n = a1 * b2 + a2 * b1
    d = b1 * b2
    return simplificar(n, d)

def multiplicar(a1, b1, a2, b2):
    return simplificar(a1 * a2, b1 * b2)

def dividir(a1, b1, a2, b2):
    return simplificar(a1 * b2, b1 * a2)

def mostrar(nome, frac):
    print(nome + " = " + str(frac[0]) + "/" + str(frac[1]))

# Ler valores de X1 e X2
print("Valores de X1:")
valores_x1 = [int(x) for x in input().split()]
print("Valores de X2:")
valores_x2 = [int(x) for x in input().split()]

nx = len(valores_x1)
ny = len(valores_x2)

# Ler a tabela P(X1,X2)
print("Tabela de P(X1,X2), linha por linha (numerador/denominador):")
P = []
for i in range(nx):
    linha = input("Linha " + str(i+1) + ": ").split()
    linha_frac = []
    for val in linha:
        num, den = val.split("/")
        linha_frac.append((int(num), int(den)))
    P.append(linha_frac)

# Menu
print("1 - P(X1 >= k)")
print("2 - Esperanca e Variancia de X1 ou X2")
print("3 - Esperanca cond.")
print("4 - cov(X1, X2)")
op = int(input("Opcao: "))

# OPÇÃO 1
if op == 1:
    print(" a - P(X1 >= k)")
    print(" b - P(X1 <= k)")
    print(" c - P(X1 = k)")
    print(" d - P(a <= X1 <= b)")
    tipo = input("Tipo (a-d): ")
    prob = (0,1)

    if tipo == "a":
        k = int(input("k: "))
        for i in range(nx):
            if valores_x1[i] >= k:
                for j in range(ny):
                    prob = somar(prob[0], prob[1], P[i][j][0], P[i][j][1])
        mostrar("P(X1 >= " + str(k) + ")", prob)

    elif tipo == "b":
        k = int(input("k: "))
        for i in range(nx):
            if valores_x1[i] <= k:
                for j in range(ny):
                    prob = somar(prob[0], prob[1], P[i][j][0], P[i][j][1])
        mostrar("P(X1 <= " + str(k) + ")", prob)

    elif tipo == "c":
        k = int(input("k: "))
        if k in valores_x1:
            i = valores_x1.index(k)
            for j in range(ny):
                prob = somar(prob[0], prob[1], P[i][j][0], P[i][j][1])
            mostrar("P(X1 = " + str(k) + ")", prob)
        else:
            print("Valor inválido")

    elif tipo == "d":
        a = int(input("a: "))
        b = int(input("b: "))
        for i in range(nx):
            if a <= valores_x1[i] <= b:
                for j in range(ny):
                    prob = somar(prob[0], prob[1], P[i][j][0], P[i][j][1])
        mostrar("P(" + str(a) + " <= X1 <= " + str(b) + ")", prob)
    else:
        print("Tipo inválido")

# OPÇÃO 2
elif op == 2:
    print("Variável: 1 para X1, 2 para X2")
    var = int(input("Variável: "))
    E = (0,1)
    E2 = (0,1)

    if var == 1:
        for i in range(nx):
            for j in range(ny):
                p = P[i][j]
                x = valores_x1[i]
                prod = multiplicar(x,1, p[0], p[1])
                E = somar(E[0], E[1], prod[0], prod[1])

                prod2 = multiplicar(x*x,1, p[0], p[1])
                E2 = somar(E2[0], E2[1], prod2[0], prod2[1])
        # V = E2 - E^2
        E_quad = dividir(E[0]*E[0], E[1]*E[1], 1, 1)
        V = somar(E2[0], E2[1], -E_quad[0], E_quad[1])
        mostrar("E[X1]", E)
        mostrar("V[X1]", V)

    elif var == 2:
        for i in range(nx):
            for j in range(ny):
                p = P[i][j]
                y = valores_x2[j]
                prod = multiplicar(y,1, p[0], p[1])
                E = somar(E[0], E[1], prod[0], prod[1])

                prod2 = multiplicar(y*y,1, p[0], p[1])
                E2 = somar(E2[0], E2[1], prod2[0], prod2[1])
        E_quad = dividir(E[0]*E[0], E[1]*E[1], 1, 1)
        V = somar(E2[0], E2[1], -E_quad[0], E_quad[1])
        mostrar("E[X2]", E)
        mostrar("V[X2]", V)
    else:
        print("Variável inválida")

# OPÇÃO 3
elif op == 3:
    print(" a - E[X1 | X2 = y]")
    print(" b - E[X2 | X1 = x]")
    tipo = input("Tipo (a/b): ")

    if tipo == "a":
        y = int(input("y: "))
        if y not in valores_x2:
            print("Valor inválido")
        else:
            j = valores_x2.index(y)
            total = (0,1)
            E = (0,1)
            for i in range(nx):
                total = somar(total[0], total[1], P[i][j][0], P[i][j][1])
            for i in range(nx):
                if total[0] != 0:
                    frac = dividir(P[i][j][0], P[i][j][1], total[0], total[1])
                    termo = multiplicar(valores_x1[i], 1, frac[0], frac[1])
                    E = somar(E[0], E[1], termo[0], termo[1])
            mostrar("E[X1 | X2="+str(y)+"]", E)

    elif tipo == "b":
        x = int(input("x: "))
        if x not in valores_x1:
            print("Valor inválido")
        else:
            i = valores_x1.index(x)
            total = (0,1)
            E = (0,1)
            for j in range(ny):
                total = somar(total[0], total[1], P[i][j][0], P[i][j][1])
            for j in range(ny):
                if total[0] != 0:
                    frac = dividir(P[i][j][0], P[i][j][1], total[0], total[1])
                    termo = multiplicar(valores_x2[j], 1, frac[0], frac[1])
                    E = somar(E[0], E[1], termo[0], termo[1])
            mostrar("E[X2 | X1="+str(x)+"]", E)
    else:
        print("Tipo inválido")

# OPÇÃO 4
elif op == 4:
    E_X1 = (0,1)
    E_X2 = (0,1)
    E_X1X2 = (0,1)
    for i in range(nx):
        for j in range(ny):
            p = P[i][j]
            xi = valores_x1[i]
            yj = valores_x2[j]

            termo1 = multiplicar(xi,1, p[0], p[1])
            E_X1 = somar(E_X1[0], E_X1[1], termo1[0], termo1[1])

            termo2 = multiplicar(yj,1, p[0], p[1])
            E_X2 = somar(E_X2[0], E_X2[1], termo2[0], termo2[1])

            termo3 = multiplicar(xi*yj,1, p[0], p[1])
            E_X1X2 = somar(E_X1X2[0], E_X1X2[1], termo3[0], termo3[1])

    prodEX1EX2 = multiplicar(E_X1[0], E_X1[1], E_X2[0], E_X2[1])
    cov = somar(E_X1X2[0], E_X1X2[1], -prodEX1EX2[0], prodEX1EX2[1])
    mostrar("cov(X1,X2)", cov)

else:
    print("Opção inválida")
