def main():
    print("Insere valores separados por espaço:")
    s = input()
    valores = [float(x) for x in s.split()]
    
    n = len(valores)
    soma = 0
    for x in valores:
        soma += x
    media = soma / n
    
    soma_quad_dif = 0
    for x in valores:
        soma_quad_dif += (x - media) ** 2
    
    variancia = soma_quad_dif / (n - 1) if n > 1 else 0
    desvio = variancia ** 0.5

    minimo = min(valores)
    maximo = max(valores)
    amplitude = maximo - minimo

    # Mediana
    valores.sort()
    if n % 2 == 1:
        mediana = valores[n // 2]
    else:
        mediana = (valores[n // 2 - 1] + valores[n // 2]) / 2

    # Moda (caso simples)
    moda = valores[0]
    freq_max = valores.count(moda)
    for x in valores:
        f = valores.count(x)
        if f > freq_max:
            freq_max = f
            moda = x
    if freq_max == 1:
        moda = "Sem moda"

    print("n =", n)
    print("Soma =", soma)
    print("Media =", media)
    print("Soma(xi - x̄)e2 =", soma_quad_dif)
    print("Variancia (s2) =", variancia)
    print("Desvio padrao (s) =", desvio)
    print("Min =", minimo)
    print("Max =", maximo)
    print("Amplitude =", amplitude)
    print("Mediana =", mediana)
    print("Moda =", moda)

main()
