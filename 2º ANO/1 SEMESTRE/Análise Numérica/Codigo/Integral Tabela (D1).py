def to_float(s):
    return float(s.strip().replace(",", "."))

def decs(s):
    s = s.strip().replace(",", ".")
    if "e" in s or "E" in s:
        return 0
    p = s.find(".")
    return 0 if p == -1 else (len(s) - p - 1)

def divdiff(x, y):
    n = len(x)
    a = y[:]
    for k in range(1, n):
        for i in range(n - 1, k - 1, -1):
            a[i] = (a[i] - a[i - 1]) / (x[i] - x[i - k])
    return a

def newton_to_power(x, a):
    n = len(x)
    c = [0.0] * n
    c[0] = a[0]
    p = [1.0]
    for k in range(1, n):
        xk = x[k - 1]
        p2 = [0.0] * (len(p) + 1)
        for i in range(len(p)):
            p2[i] += -xk * p[i]
            p2[i + 1] += p[i]
        p = p2
        for i in range(len(p)):
            c[i] += a[k] * p[i]
    return c

def poly_int(c, A, B):
    s = 0.0
    pa = A
    pb = B
    for k in range(len(c)):
        s += c[k] * (pb - pa) / (k + 1)
        pa *= A
        pb *= B
    return s

n = int(input("n: "))
x = [0.0] * n
y = [0.0] * n
dmax = 0

for i in range(n):
    x[i] = to_float(input("x{}: ".format(i + 1)))

for i in range(n):
    ys = input("f{}: ".format(i + 1))
    y[i] = to_float(ys)
    d = decs(ys)
    if d > dmax:
        dmax = d

a = to_float(input("a: "))
b = to_float(input("b: "))

ok = True
for i in range(n):
    for j in range(i + 1, n):
        if x[i] == x[j]:
            ok = False

if not ok:
    print("Erro")
else:
    A = divdiff(x, y)
    c = newton_to_power(x, A)
    val = poly_int(c, a, b)
    fmt = "{:." + str(dmax) + "f}"
    print(fmt.format(val))
