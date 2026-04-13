def min_degree_interpolating(x, y, n):
    eps = 1e-12
    dd = [0.0] * n
    for i in range(n):
        dd[i] = y[i]
    deg = 0
    for k in range(1, n):
        for i in range(n - k):
            den = x[i + k] - x[i]
            dd[i] = (dd[i + 1] - dd[i]) / den
        if abs(dd[0]) > eps:
            deg = k
    return deg

n = int(input("n pontos: "))
x = [0.0] * n
y = [0.0] * n

for i in range(n):
    x[i] = float(input("x{}: ".format(i+1)))
for i in range(n):
    y[i] = float(input("y{}: ".format(i+1)))

ok = True
for i in range(n):
    for j in range(i+1, n):
        if x[i] == x[j]:
            ok = False

if not ok:
    print("Erro")
else:
    print(min_degree_interpolating(x, y, n))
