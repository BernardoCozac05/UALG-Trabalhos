def to_float(s):
    return float(s.strip().replace(",", "."))

def divdiff(x, y):
    n = len(x)
    a = y[:]
    for k in range(1, n):
        for i in range(n - 1, k - 1, -1):
            a[i] = (a[i] - a[i - 1]) / (x[i] - x[i - k])
    return a

def deriv_at(x, a, xp):
    n = len(x)
    if n < 2:
        return 0.0

    prod = [0.0] * n
    prod[0] = 1.0
    for k in range(1, n):
        prod[k] = prod[k - 1] * (xp - x[k - 1])

    s = a[1] if n > 1 else 0.0

    if n > 2:
        s += a[2] * ((xp - x[0]) + (xp - x[1]))

    for k in range(3, n):
        total = 0.0
        for m in range(k):
            p = 1.0
            for j in range(k):
                if j != m:
                    p *= (xp - x[j])
            total += p
        s += a[k] * total

    return s

n = int(input("n+1 pontos (ex: 4): "))
x = [0.0] * n
y = [0.0] * n

for i in range(n):
    x[i] = to_float(input("x{}: ".format(i + 1)))
for i in range(n):
    y[i] = to_float(input("f{}: ".format(i + 1)))

xp = to_float(input("x* (onde quer f'(x*)): "))

ok = True
for i in range(n):
    for j in range(i + 1, n):
        if x[i] == x[j]:
            ok = False

if not ok or n < 2:
    print("Erro")
else:
    a = divdiff(x, y)
    print(deriv_at(x, a, xp))
