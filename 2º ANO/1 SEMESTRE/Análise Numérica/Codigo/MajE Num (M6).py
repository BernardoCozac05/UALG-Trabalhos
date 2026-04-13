def round_sig(x, n, mode):
    if x == 0:
        return 0.0
    if x < 0:
        x = -x

    k = 0
    m = x
    while m >= 10.0:
        m /= 10.0
        k += 1
    while m < 1.0:
        m *= 10.0
        k -= 1

    scale = 10.0 ** (n - 1 - k)
    y = x * scale

    iy = int(y)
    if mode == 1 and y != iy:
        iy += 1

    return iy / scale

xbar_str = input("x: ").strip().replace(",", ".")
xbar = float(xbar_str)

m = int(input("Erro: 1=num, 2=perc: "))

if m == 1:
    dx = float(input("erro: ").strip().replace(",", "."))
    if dx < 0: dx = -dx
    den = abs(xbar) - dx
    if den <= 0:
        print("Erro")
    else:
        eps = dx / den
        t = int(input("Arred: 1=exce, 2=def: "))
        n = int(input("Alg. rel.: "))
        if t not in (1, 2) or n < 1:
            print("Erro")
        else:
            print(round_sig(eps, n, t))

elif m == 2:
    p = float(input("Erro % (ex: 0.8): ").strip().replace(",", "."))
    if p < 0: p = -p
    r = p / 100.0
    if r >= 1.0:
        print("Erro")
    else:
        eps = r / (1.0 - r)
        t = int(input("Arred: 1=exce, 2=def: "))
        n = int(input("Alg. rel.: "))
        if t not in (1, 2) or n < 1:
            print("Erro")
        else:
            print(round_sig(eps, n, t))

else:
    print("Erro")
