def sign(x):
    if x > 0: return 1
    if x < 0: return -1
    return 0

def count_changes(seq):
    prev = 0
    v = 0
    for x in seq:
        sx = sign(x)
        if sx == 0:
            continue
        if prev != 0 and sx != prev:
            v += 1
        prev = sx
    return v

def possible_counts(v):
    r = []
    k = v
    while k >= 0:
        r.append(k)
        k -= 2
    return r

n = int(input("Grau n: "))
a = [0.0] * (n + 1)

for p in range(n, -1, -1):
    a[n - p] = float(input("a{}: ".format(p)).replace(",", "."))

vpos = count_changes(a)

aneg = [0.0] * (n + 1)
for i in range(n + 1):
    p = n - i
    c = a[i]
    if p % 2 == 1:
        c = -c
    aneg[i] = c

vneg = count_changes(aneg)

print("V+ =", vpos)
print("Possiveis +:", possible_counts(vpos))
print("V- =", vneg)
print("Possiveis -:", possible_counts(vneg))
